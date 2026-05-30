#ifndef CLAW_h
#define CLAW_h

#include <ESP32Servo.h>

class Claw {
    private:
        int min_pos, max_pos, current_pos;
        Servo *servo;

    public:
        Claw(int min_pos, int max_pos, int init_pos, uint8_t pin) 
        : min_pos(min_pos), max_pos(max_pos) {
            servo = new Servo();
            servo->attach(pin);
            current_pos = init_pos;
        }
        Claw(int min_pos, int max_pos, uint8_t pin) : Claw(min_pos, max_pos, min_pos, pin) {}

        void moveToPos(int pos) {
            pos = constrain(pos, this->min_pos, this->max_pos);
            servo->write(pos);
            current_pos = pos;
        }

        void move(int amt) { moveToPos(this->current_pos + amt); }
        void closeClaw() { moveToPos(this->max_pos); }
        void openClaw() { moveToPos(this->min_pos); }
};

#endif