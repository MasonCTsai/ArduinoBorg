#ifndef WRIST_h
#define WRIST_h

#include <DrivenMotor.h>
#include "PIDController.h"

class Wrist {
    private:
        DrivenMotor *motor1, *motor2;
        long zeroRoll, zeroPitch, targetRoll, targetPitch;
        bool running_pos = false;

    public:
        Wrist(DrivenMotor *motor1, DrivenMotor *motor2) :
        motor1(motor1), motor2(motor2), zeroRoll(0), zeroPitch(0),
        targetRoll(0), targetPitch(0) {}

        Wrist(uint8_t step1, uint8_t dir1, uint8_t step2, uint8_t dir2) :
        Wrist(new DrivenMotor(step1, dir1, 0b00, &DrivenMotor::DEFAULT_CONFIGS), 
        (new DrivenMotor(step2, dir2, 0b11, &DrivenMotor::DEFAULT_CONFIGS))) {}

        void home() {
            Serial.print(motor1->sg_result());
            Serial.print(" ");
            Serial.println(motor2->sg_result());

            // set(0, -50);
            // while (motor1->sg_result() >= DrivenMotor::HOMING_THRESHOLD && motor2->sg_result() >= DrivenMotor::HOMING_THRESHOLD) {
            //     run();
            // }
            // zeroRoll = getRoll();
            // zeroPitch = getPitch();
        }

        // idek which way is which just try
        void set(float rollSpeed, float pitchSpeed) {
            motor1->setSpeed(rollSpeed + pitchSpeed);
            motor2->setSpeed(rollSpeed - pitchSpeed);
        }

        void setPos(float rollPos, float pitchPos) {
            this->running_pos = true;
            this->targetRoll = rollPos;
            this->targetPitch = pitchPos;
            float rollSpeed = constrain(rollPos - getRoll(), -50, 50);
            if (abs(rollSpeed) < 20) rollSpeed = 0;
            float pitchSpeed = constrain(pitchPos - getPitch(), -50, 50);
            if (abs(pitchSpeed) < 20) pitchSpeed = 0;
            set(rollSpeed, pitchSpeed);
            if(pitchSpeed == 0 && rollSpeed == 0) {
                this->running_pos = false;
                set(0,0);
            }
        }

        long getRoll() {
            return this->getAbsoluteRoll() - zeroRoll;
        }
        long getAbsoluteRoll() {
            return (motor1->currentPosition() - motor2->currentPosition()) / 2;
        }

        long getPitch() {
            return this->getAbsolutePitch() - zeroPitch;
        }
        long getAbsolutePitch() {
            return (motor1->currentPosition() + motor2->currentPosition()) / 2;
        }

        void run() {
            if (running_pos) setPos(targetRoll, targetPitch);
            motor1->run();
            motor2->run();
        }
};

#endif