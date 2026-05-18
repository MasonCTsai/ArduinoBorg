#ifndef ELEVATOR_h
#define ELEVATOR_h

#include <DrivenMotor.h>

class Elevator {
    private:
        DrivenMotor *motor1, *motor2;
        uint8_t homeSwitchPin;

    public:
        Elevator(DrivenMotor *motor1, DrivenMotor *motor2, uint8_t homeSwitchPin) :
        motor1(motor1), motor2(motor2), homeSwitchPin(homeSwitchPin) {}

        Elevator(uint8_t step1, uint8_t dir1, uint8_t step2, uint8_t dir2, uint8_t homeSwitchPin) :
        Elevator(new DrivenMotor(step1, dir1, 0b00, &DrivenMotor::DEFAULT_CONFIGS),
        new DrivenMotor(step2, dir2, 0b01, &DrivenMotor::DEFAULT_CONFIGS),
        homeSwitchPin) {}

        void manualSetUp() { // manual homing sequence
            pinMode(this->homeSwitchPin, INPUT_PULLUP);

            this->motor1->setSpeed(-200);
            this->motor2->setSpeed(200);

            while (digitalRead(homeSwitchPin) == HIGH) {
                this->motor1->run();
                this->motor2->run();
            }
            
            reset();
        }
        
        void reset() {
            this->motor1->setSpeed(0);
            this->motor2->setSpeed(0);
    
            this->motor1->setCurrentPosition(0);
            this->motor2->setCurrentPosition(0);
            this->motor1->setAcceleration(500);
            this->motor2->setAcceleration(500);
        }

        void setTarget(long steps) {
            this->motor1->moveTo(steps);
            this->motor2->moveTo(-steps);
        }

        void run() {
            this->motor1->runToTarget();
            this->motor2->runToTarget();
        }

        void print() {
            Serial.println("Elevator position: ");
            Serial.println(motor1->currentPosition());
            Serial.println(motor2->currentPosition());
        }
};

#endif