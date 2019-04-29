#include "mbed.h"
#include "Motor.h"

//InterruptIn turn_signal(p19);
AnalogIn left_turn_signal(p19); // this pin checks if signal high or low for turning

Motor left_wheel(p21, p7, p8); // pwm, fwd, rev
Motor right_wheel(p22, p9, p10); // pwm, fwd, rev

PwmOut turn_left(p26); // testing only

float speed = 0.4;
float duration = 1.0;
float turn_slices = 6.0;

void turn() {
    left_wheel.speed(-speed);
    right_wheel.speed(-speed);
}

// Currently whenever the robot detects a signal high it will turn in small
// increments, until it detects a signal low (Pixy has detected a pattern)
int main() {
    //turn_signal.rise(&turn);
    while(1) {
        if (left_turn_signal > 0.5) {
            left_wheel.speed(-speed);
            right_wheel.speed(-speed);
            //wait(0.05);
        } else {
            left_wheel.speed(0.0);
            right_wheel.speed(0.0);
            //wait(0.05);
        }
    }
}
