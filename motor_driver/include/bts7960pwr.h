#ifndef BTS7960PWR_H
#define BTS7960PWR_H

#include <Arduino.h>
#include "bts7960.h"

class BTS7960Pwr : public BTS7960
{
public:
  BTS7960Pwr( uint8_t pwr, uint8_t gnd, 
                uint8_t len, uint8_t ren, uint8_t lpwm, uint8_t rpwm ) : 
            BTS7960( len, ren, lpwm, rpwm )
  {
    pinMode( pwr, OUTPUT );
    digitalWrite( pwr, HIGH );

    pinMode( gnd, OUTPUT );
    digitalWrite( gnd, LOW );
  }

  ~BTS7960Pwr()
  {
    //~BTS7960();
  }
};

#endif // BTS7960PWR_H