#ifndef BTS7960_H
#define BTS7960_H

#include <Arduino.h>

class BTS7960
{
private:
    const uint8_t _len, _ren, _lpwm, _rpwm;

public:
    BTS7960( uint8_t len, uint8_t ren, uint8_t lpwm, uint8_t rpwm )
        : _len( len ), _ren( ren ), _lpwm( lpwm ), _rpwm( rpwm )
    {
        pinMode( _len, OUTPUT );
        pinMode( _ren, OUTPUT );
        pinMode( _lpwm, OUTPUT );
        pinMode( _rpwm, OUTPUT );

        digitalWrite( _len, LOW );
        digitalWrite( _ren, LOW );
        analogWrite( _lpwm, 0 );
        analogWrite( _rpwm, 0 );
    }

    void setSpeed( int speed )
    {
        speed = constrain( speed, -255, 255 );

        if( speed > 0 )
        {
            analogWrite( _rpwm, 0 );
            digitalWrite( _len, HIGH );
            digitalWrite( _ren, HIGH );            
            analogWrite( _lpwm, speed );

        }
        else if( speed < 0 )
        {
            analogWrite( _lpwm, 0 );
            digitalWrite( _len, HIGH );
            digitalWrite( _ren, HIGH );
            analogWrite( _rpwm, -speed );
        }
        else
        {
            analogWrite( _lpwm, 0 );
            analogWrite( _rpwm, 0 );
            digitalWrite( _len, LOW );
            digitalWrite( _ren, LOW );            
        }
    }
};

#endif // BTS7960_H
