#include <Arduino.h>
#include "encoderpwr.h"
#include "bts7960pwr.h"
#include "pid.h"
#include "pins.h"


// PID variables
double Kp = 2, Ki = 5, Kd = 1;

// Motor and encoder objects
//EncodersPwr lenc( ENCA_PWR, ENCA_GND, ENCA_A, ENCA_B );
//EncodersPwr renc( ENCB_PWR, ENCB_GND, ENCB_A, ENCB_B );
//EncodersPwr senc( ENCC_PWR, ENCC_GND, ENCC_A, ENCC_B );

BTS7960Pwr lmotor( MTRA_PWR, MTRA_GND, MTRA_LEN, MTRA_REN, MTRA_LPWM, MTRA_RPWM );
BTS7960Pwr rmotor( MTRB_PWR, MTRB_GND, MTRB_LEN, MTRB_REN, MTRB_LPWM, MTRB_RPWM );
BTS7960Pwr smotor( MTRC_PWR, MTRC_GND, MTRC_LEN, MTRC_REN, MTRC_LPWM, MTRC_RPWM );
// Constants
const float ticks = 64.f;
const float ratio = (64.f / 12.f * 55.f / 8.f * 43.f / 10.f * ticks) / 360.f;

int count = 0;

// State variables
int steer_setpoint = 0; // Desired steering angle
int motor_setpoint = 0; // Desired speed of front motors

void setup() {
    Serial.begin(9600);
    Serial.println("Setup");

    lmotor.setSpeed(0);
    rmotor.setSpeed(0);
    smotor.setSpeed(0);

    pinMode( ENCC_A, INPUT );
}

// int calculate( long setpoint, long position )
// {
//   const long ticks = encoder.getEncoderCount();
//   long error = setpoint - ticks;
//   long proportional = Kp * error;
//   long pwm = constrain( proportional, -255, 255 );

//   return abs( error ) < 10 ? 0 : pwm;
// }

void loop() {
    // Read user input from Serial
    static String buffer;
    static uint8_t l = 0, r = 0, s = 0;
    static unsigned long watchdog = 0;
    const unsigned long timeout = 1000;

    Serial.println( digitalRead( ENCC_A ));

    
    /*lmotor.setSpeed(count-255);
    rmotor.setSpeed(count-255);
    smotor.setSpeed(count-255);
    count = (count+1)%(255*2);
    delay(10);*/
    // while (Serial.available()) {
    //     char c = Serial.read();
    //     if (c == '\n') {
    //         // Expecting input format: "l r s"
    //         int firstSpaceIndex = buffer.indexOf(' ');
    //         int secondSpaceIndex = buffer.indexOf(' ', firstSpaceIndex + 1);
    //         if (firstSpaceIndex != -1 && secondSpaceIndex != -1) {
    //             // Parse values from serial input
    //             l = buffer.substring(0, firstSpaceIndex).toInt();
    //             r = buffer.substring(firstSpaceIndex + 1, secondSpaceIndex).toInt();
    //             s = buffer.substring(secondSpaceIndex + 1).toInt();
    //         }
    //         buffer = "";

    //         watchdog = millis() + timeout;
    //         lmotor.setSpeed(l);
    //         rmotor.setSpeed(r);
    //         smotor.setSpeed(s);
    //     } else {
    //         buffer += c;
    //     }
    // }

    // Read encoder values
/*    Serial.print( lenc.getEncoderCount() );
    Serial.print( ' ' );
    Serial.print( renc.getEncoderCount() );
    Serial.print( ' ' );
    Serial.println( senc.getEncoderCount() );*/

    // // Check if watchdog timer has expired
    // if (millis() > watchdog) {
    //     lmotor.setSpeed(0);
    //     rmotor.setSpeed(0);
    //     smotor.setSpeed(0);
    // }
}
