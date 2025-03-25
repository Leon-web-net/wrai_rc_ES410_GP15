#ifndef PID_H
#define PID_H

class PID
{
public:

    PID( float kp, float ki, float kd )
        : _kp( kp ), _ki( ki ), _kd( kd )
    {
        _integral = _last_error = 0.f;
    }

    float calculate( float setpoint, float current );

private:
    float _kp, _ki, _kd;
    float _integral, _last_error;
};

float PID::calculate( float setpoint, float current )
{
    const float error = setpoint - current;

    Serial.println( error );

    // proportional term
    const float proportional = _kp * error;
    
    // integral term
    _integral = (_integral*0.99) + error;
    const float integral = _ki * _integral;

    // derivative term
    const float derivative = _kd * (error - _last_error);

    _last_error = error;

    Serial.println( proportional );
    Serial.println( integral );
    Serial.println( derivative );

    return proportional + integral + derivative;
}

#endif // PID_H
