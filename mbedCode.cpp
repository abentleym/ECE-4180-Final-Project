#include "mbed.h"
#include "Motor.h"

AnalogIn left_turn_signal(p19); // this pin checks if signal high or low for turning

Motor left_wheel(p21, p7, p8); // pwm, fwd, rev
Motor right_wheel(p22, p9, p10); // pwm, fwd, rev

float speed = 0.4;

// Currently whenever the robot detects a signal high it will turn in small
// increments, until it detects a signal low (Pixy has detected a pattern)
int main() {
    while(1) {
        if (left_turn_signal > 0.5) { // if signal high is detected from Pi
            left_wheel.speed(-speed); // turn robot
            right_wheel.speed(-speed);
        } else {
            left_wheel.speed(0.0);
            right_wheel.speed(0.0);
        }
    }
}
