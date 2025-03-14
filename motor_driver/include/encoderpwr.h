#ifndef ENCODERPWR_H
#define ENCODERPWR_H

#include <Arduino.h>
#include <QuadratureEncoder.h>

class EncodersPwr : public Encoders
{
public:
    EncodersPwr( uint8_t pwr, uint8_t gnd, uint8_t a, uint8_t b ) : 
        Encoders( a, b )
    {
        pinMode( pwr, OUTPUT );
        digitalWrite( pwr, HIGH );

        pinMode( gnd, OUTPUT );
        digitalWrite( gnd, LOW );
    }

    ~EncodersPwr()
    {
        //~Encoders();
    }
};

#endif // ENCODERPWR_H