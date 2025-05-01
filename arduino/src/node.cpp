#include <Arduino.h>
#include "encoderpwr.h"
#include "bts7960pwr.h"
#include "pins.h"
#include "state_manager.h"

// editable parameters
const int statusHz = 50;
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
EncodersPwr lenc( ENCA_PWR, ENCA_GND, ENCA_A, ENCA_B );
EncodersPwr renc( ENCB_PWR, ENCB_GND, ENCB_A, ENCB_B );
EncodersPwr senc( ENCC_PWR, ENCC_GND, ENCC_A, ENCC_B );

BTS7960Pwr lmotor( MTRA_PWR, MTRA_GND, MTRA_LEN, MTRA_REN, MTRA_LPWM, MTRA_RPWM );
BTS7960Pwr rmotor( MTRB_PWR, MTRB_GND, MTRB_LEN, MTRB_REN, MTRB_LPWM, MTRB_RPWM );
BTS7960Pwr smotor( MTRC_PWR, MTRC_GND, MTRC_LEN, MTRC_REN, MTRC_LPWM, MTRC_RPWM );


void setup() 
{
    Serial.begin(9600);
}

void send_status()
{
    static unsigned long statusTimer = 0;
    static unsigned long statusInterval = 1000 / statusHz;
    
    const unsigned long now = millis();
    
    if( now < statusTimer )
        return;

    statusTimer += statusInterval;
    
    Serial.print( minEnc );
    Serial.print( ' ' );
    Serial.print( maxEnc );
    Serial.print( ' ' );
    Serial.print( lenc.getEncoderCount() );
    Serial.print( ' ' );
    Serial.print( renc.getEncoderCount() );
    Serial.print( ' ' );
    Serial.println( senc.getEncoderCount() );
}

bool __get_serial()
{
    static String buffer;
    const char c = Serial.read();
    // complete message
    if( c == '\n' ) 
    {
        // Expecting input format: "l r s"
        int firstSpaceIndex = buffer.indexOf(' ');
        int secondSpaceIndex = buffer.indexOf(' ', firstSpaceIndex + 1);
        if (firstSpaceIndex != -1 && secondSpaceIndex != -1) {
            // Parse values from serial input
            l = buffer.substring(0, firstSpaceIndex).toInt();
            r = buffer.substring(firstSpaceIndex + 1, secondSpaceIndex).toInt();
            s = buffer.substring(secondSpaceIndex + 1).toInt();
        }
        buffer = "";

        return true;
    } 
    else 
    {
        buffer += c;
    }

    return false;
}

bool process_serial( const int maxIterations = 50 )
{   
    bool ret = false;

    // while serial is available but no blocking loops
    for( int i=0; i!=maxIterations && Serial.available(); ++i )
    {
        ret = ret || __get_serial();
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
            //Serial.println( "Uncalibrated" );
            maxEnc = senc.getEncoderCount();
            sm.state( LEFT );
            break;
        }

        /* drive the motor left (or right until the min/max encoder
            values are found */
        case LEFT:
        {
            //Serial.println( "Left" );
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
            //Serial.println( "Right" );
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
            //Serial.println( "Center" );
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
            //Serial.println( "Calibrated" );
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
