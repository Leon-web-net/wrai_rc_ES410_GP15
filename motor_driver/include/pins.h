#ifndef PINS_H
#define PINS_H

// encoder pins
#define ENCA_PWR 5
#define ENCA_GND 4
#define ENCA_A   2 // hardware interrupt
#define ENCA_B   3 // hardware interrupt

#define ENCB_PWR 17
#define ENCB_GND 16
#define ENCB_A   18 // hardware interrupt
#define ENCB_B   19 // hardware interrupt

#define ENCC_PWR 22
#define ENCC_GND 23
#define ENCC_A   20 // hardware interrupt
#define ENCC_B   21 // hardware interrupt

// motor driver pins
#define MTRA_PWR  26
#define MTRA_GND  27
#define MTRA_LEN  28
#define MTRA_REN  29
#define MTRA_LPWM  8 // pwm
#define MTRA_RPWM  9 // pwm
#define MTRA_LIS  30
#define MTRA_RIS  31

#define MTRB_PWR  32
#define MTRB_GND  33
#define MTRB_LEN  34
#define MTRB_REN  35
#define MTRB_LPWM 10 // pwm
#define MTRB_RPWM 11 // pwm
#define MTRB_LIS  36
#define MTRB_RIS  37

#define MTRC_PWR  38
#define MTRC_GND  39
#define MTRC_LEN  40
#define MTRC_REN  41
#define MTRC_LPWM 12 // pwm
#define MTRC_RPWM 13 // pwm
#define MTRC_LIS  42
#define MTRC_RIS  43

#endif // PINS_H