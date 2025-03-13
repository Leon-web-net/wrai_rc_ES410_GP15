#include <Arduino.h>
#include <QuadratureEncoder.h>

#include "bts7960.h"
#include "pid.h"


// motor driver pins
#define L_EN 7
#define R_EN 8
#define RPWM 5
#define LPWM 6

// hall encoder pins
#define ENC_A 2
#define ENC_B 3

double input = 0, output = 0, setpoint = 0;
double Kp=2, Ki=5, Kd=1;

Encoders encoder( ENC_A, ENC_B );
BTS7960 motor( L_EN, R_EN, LPWM, RPWM );

const float ticks = 64.f;
const float ratio = ( 67.f/12.f * 54.f/8.f * 51.f/10.f * ticks ) / 360.f;

void setup() 
{
  Serial.begin(9600);
  Serial.println( "Setup" );

  setpoint = 180 * ratio;
}

int calculate( long setpoint, long position )
{
  const long ticks = encoder.getEncoderCount();
  long error = setpoint - ticks;
  long proportional = Kp * error;
  long pwm = constrain( proportional, -255, 255 );

  return abs( error ) < 10 ? 0 : pwm;
}

void loop() 
{
  static String buffer;
  while( Serial.available() )
  {
    char c = Serial.read();
    if( c == '\n' ) 
    {
      setpoint = buffer.toInt();
      buffer = "";
    }
    else
    {
      buffer += c;
    }
  }

  
  motor.setSpeed( calculate( setpoint, encoder.getEncoderCount() ) );

  Serial.print( "Setpoint: " );
  Serial.println( setpoint );
  Serial.print( "Encoder: " );
  Serial.println( encoder.getEncoderCount() );


  if( abs( encoder.getEncoderCount() - setpoint ) < 10 )
  {
    setpoint *= -1;
  }

  //delay( 10 );
}
