#ifndef WRIST_h
#define WRIST_h

#include <DrivenMotor.h>

class Wrist {
    private:
        DrivenMotor *motor1, *motor2;

    public:
        Wrist(DrivenMotor *motor1, DrivenMotor *motor2) :
        motor1(motor1), motor2(motor2) {}

        Wrist(uint8_t step1, uint8_t dir1, uint8_t step2, uint8_t dir2) :
        motor1(new DrivenMotor(step1, dir1, 0b00, &DrivenMotor::DEFAULT_CONFIGS)),
        motor2(new DrivenMotor(step2, dir2, 0b11, &DrivenMotor::DEFAULT_CONFIGS)) {}

        // idek which way is which just try
        void set(float yawSpeed, float pitchSpeed) {
            motor1->setSpeed(yawSpeed + pitchSpeed);
            motor2->setSpeed(yawSpeed - pitchSpeed);
        }

        void run() {
            motor1->run();
            motor2->run();
        }
};

#endif