#include <Arduino.h>
#include "encoderpwr.h"
#include "bts7960pwr.h"
#include "pins.h"
#include "state_manager.h"

// editable parameters
const int statusHz = 100;
const int watchdogMs = 1000;

// global variables
int minEnc = 0, maxEnc = 0;
int l = 0, r = 0, s = 0;

enum Calibration
{
    UNCALIBRATED,
    LEFT,
    RIGHT,
    CENTER,
    CALIBRATED,
    ACTIVE
};

enum Watchdog
{
    DEAD,
    ALIVE
};

// Motor and encoder objects
// lenc and renc and disabled while we test possible interrupt handling limitations
//EncodersPwr lenc( ENCA_PWR, ENCA_GND, ENCA_A, ENCA_B ); 
//EncodersPwr renc( ENCB_PWR, ENCB_GND, ENCB_A, ENCB_B );
EncodersPwr senc( ENCC_PWR, ENCC_GND, ENCC_A, ENCC_B );

BTS7960Pwr lmotor( MTRA_PWR, MTRA_GND, MTRA_LEN, MTRA_REN, MTRA_LPWM, MTRA_RPWM );
BTS7960Pwr rmotor( MTRB_PWR, MTRB_GND, MTRB_LEN, MTRB_REN, MTRB_LPWM, MTRB_RPWM );
BTS7960Pwr smotor( MTRC_PWR, MTRC_GND, MTRC_LEN, MTRC_REN, MTRC_LPWM, MTRC_RPWM );


void setup() 
{
    Serial.begin(115200);
}

void send_status()
{
    static unsigned long statusTimer = 0;
    static unsigned long statusInterval = 1000 / statusHz;
    
    const unsigned long now = millis();
    
    if( now < statusTimer )
        return;

    statusTimer += statusInterval;
    
    /*Serial.print( minEnc );
    Serial.print( ' ' );
    Serial.print( maxEnc );
    Serial.print( ' ' );
    Serial.print( lenc.getEncoderCount() );
    Serial.print( ' ' );
    Serial.print( renc.getEncoderCount() );
    Serial.print( ' ' );
    Serial.println( senc.getEncoderCount() );*/
    // Use a single buffer to format the status message, then send it in one Serial.write
    char buf[64];
    const long temp = 0;
    int len = snprintf(buf, sizeof(buf), "%d %d %ld %ld %ld\n",
        minEnc, maxEnc,
        temp, //lenc.getEncoderCount(),
        temp, //renc.getEncoderCount(),
        senc.getEncoderCount()
    );
    Serial.write(buf, len);
}

bool process_serial(const int maxIterations = 50)
{
    static char buffer[1024];
    static char* idx = buffer;
    bool ret = false;

    int avail = Serial.available();

    while (avail > 0)
    {
        int spaceLeft = sizeof(buffer) - (idx - buffer) - 1; // Reserve space for null terminator

        // If buffer is full and no newline, reset buffer
        if (spaceLeft <= 0)
        {
            idx = buffer;
            buffer[0] = '\0';
            spaceLeft = sizeof(buffer) - 1;
        }

        // Read available bytes into the buffer
        // Ensure we do not read more than the available space in the buffer
        int toRead = (spaceLeft < avail) ? spaceLeft : avail;
        int bytesRead = Serial.readBytes(idx, toRead);
        avail -= bytesRead;
        idx += bytesRead;
        *idx = '\0';

        // Process all complete lines in the buffer
        char* start = buffer;
        char* newline;
        while ((newline = strchr(start, '\n')) != nullptr)
        {
            *newline = '\0'; // Null-terminate the current line
            int _l = 0, _r = 0, _s = 0;
            if (sscanf(start, "%d %d %d", &_l, &_r, &_s) == 3)
            {
                l = _l;
                r = _r;
                s = _s;
                ret = true; // At least one complete message processed
            }
            start = newline + 1;
        }

        // Move any remaining partial line to the start of the buffer
        int remaining = idx - start;
        if (remaining > 0 && start != buffer)
        {
            memmove(buffer, start, remaining);
        }
        idx = buffer + remaining;
        buffer[remaining] = '\0';
    }

    return ret;
}

void loop() 
{
    static StateManager<Calibration> sm( UNCALIBRATED );
    static StateManager<Watchdog> wd( DEAD );
    int _l = 0, _r = 0, _s = 0;

    // process serial input and set watchdog accordingly
    if( process_serial() )
    {
        wd.state( ALIVE, true );
    }
    else if( wd.duration() > watchdogMs )
    {
        wd.state( DEAD );
    }

    switch( sm.state() )
    {
        /* reset encoder min/max and set transition timer to
            0.5s in the future */
        case UNCALIBRATED:
        {
            Serial.println( "Uncalibrated" );
            maxEnc = senc.getEncoderCount();
            sm.state( LEFT );
            break;
        }

        /* drive the motor left (or right until the min/max encoder
            values are found */
        case LEFT:
        {
            Serial.println( "Left" );
            _s = 255;

            const long enc = senc.getEncoderCount();
            if( enc > maxEnc )
            {
                maxEnc = enc;
                sm.reset_time();
            }

            // been at 0.5s since min/max changed, mo
            if( sm.duration() >= 500 )
            {
                minEnc = enc;
                sm.state( RIGHT );
            }
            break;
        }
        case RIGHT:
        {
            Serial.println( "Right" );
            _s = -255;

            const long enc = senc.getEncoderCount();
            if( enc < minEnc )
            {
                minEnc = enc;
                sm.reset_time();
            }

            // been at 0.5s since min/max changed, mo
            if( sm.duration() >= 500 )
            {
                sm.state( minEnc == maxEnc ? UNCALIBRATED : CENTER );
            }
            break;
        }

        case CENTER:
        {
            Serial.println( "Center" );
            const int midpoint = (maxEnc + minEnc) / 2;
            const int error = midpoint - senc.getEncoderCount();
            _s = error *2;

            if( abs(error) < 10 )
            {
                sm.state( CALIBRATED );
            }
            break;
        }

        case CALIBRATED:
        {
            Serial.println( "Calibrated" );
            if( sm.duration() > 1000 )
            {
                sm.state( ACTIVE );
            }

            break;
        }

        case ACTIVE:
        {
            send_status();

            /* the default values for _l, _r and _s are 0.
                only override them if the watchdog timer has not
                expired */
            if( wd.state() == ALIVE )
            {
                _l = l; 
                _r = r;
                _s = s;
            }

            //_l = _r = 255;
            break;
        }
    }

    lmotor.setSpeed( -_l );
    rmotor.setSpeed( -_r );
    smotor.setSpeed( _s );
}
