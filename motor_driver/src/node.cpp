#include <Arduino.h>
#include <QuadratureEncoder.h>
#include "bts7960.h"
#include "pid.h"

// Motor driver pins
#define L_EN_1 7   // Left motor enable
#define R_EN_1 8   // Right motor enable
#define RPWM_1 5   // Right motor PWM
#define LPWM_1 6   // Left motor PWM

#define L_EN_2 9   // Second front motor enable
#define R_EN_2 10  // Second front motor enable
#define RPWM_2 11  // Second front motor PWM
#define LPWM_2 12  // Second front motor PWM

// Steering motor driver pins
#define L_EN_STEER 4   // Steering motor enable
#define R_EN_STEER 3
#define RPWM_STEER 2
#define LPWM_STEER 13

// Encoder pins
#define ENC_A 2
#define ENC_B 3
#define ENC_STEER_A 18  // Steering encoder
#define ENC_STEER_B 19

// PID variables
double Kp = 2, Ki = 5, Kd = 1;

// Motor and encoder objects
Encoders encoder(ENC_A, ENC_B);
Encoders encoderSteer(ENC_STEER_A, ENC_STEER_B);
BTS7960 motor1(L_EN_1, R_EN_1, LPWM_1, RPWM_1);
BTS7960 motor2(L_EN_2, R_EN_2, LPWM_2, RPWM_2);
BTS7960 steerMotor(L_EN_STEER, R_EN_STEER, LPWM_STEER, RPWM_STEER);

// Constants
const float ticks = 64.f;
const float gear_ratio = (67.f / 12.f * 54.f / 8.f * 51.f / 10.f * ticks) / 360.f;

// State variables
int steer_setpoint = 0; // Desired steering angle
int motor_setpoint = 0; // Desired speed of front motors

void setup() {
    Serial.begin(9600);
    Serial.println("Setup");

    // Set pin high to power the encoder board
    pinMode(12, OUTPUT);
    digitalWrite(12, HIGH);
    steer_setpoint = 180 * ratio;
    motor_setpoint = 180 * ratio;
}

int calculate( long setpoint, long position )
{
  const long ticks = encoder.getEncoderCount();
  long error = setpoint - ticks;
  long proportional = Kp * error;
  long pwm = constrain( proportional, -255, 255 );

  return abs( error ) < 10 ? 0 : pwm;
}

void loop() {
    // Read user input from Serial
    static String buffer;
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            // Expecting input format: "steer_angle motor_speed"
            int spaceIndex = buffer.indexOf(' ');
            if (spaceIndex != -1) {
                // Parse values from serial input
                steer_setpoint = buffer.substring(0, spaceIndex).toInt();
                motor_setpoint = buffer.substring(spaceIndex + 1).toInt();
            }
            buffer = "";
        } else {
            buffer += c;
        }
    }

    // Convert inputs to encoder values
    int steer_target = steer_setpoint * gear_ratio;
    int motor_target = motor_setpoint * gear_ratio;

    // Apply control to motors
    motor1.setSpeed(calculate(motor_target, encoder.getEncoderCount()));
    motor2.setSpeed(calculate(motor_target, encoder.getEncoderCount())); // Sync both front motors
    steerMotor.setSpeed(calculate(steer_target, encoderSteer.getEncoderCount()));

    // Print debug info every 1 second
    static unsigned long watchdog = 0;
    const unsigned long duration = 1000;
    unsigned long now = millis();



    if (now >= watchdog) {
        Serial.print("Steer Setpoint: ");
        Serial.println(steer_setpoint);
        Serial.print("Steer Encoder: ");
        Serial.println(encoderSteer.getEncoderCount());

        Serial.print("Motor Setpoint: ");
        Serial.println(motor_setpoint);
        Serial.print("Motor Encoder: ");
        Serial.println(encoder.getEncoderCount());

        watchdog = now + duration;
    }

    if( abs( encoder.getEncoderCount() - setpoint ) < 10 )
    {
      setpoint *= -1;
    }
}
