#pragma once

#ifndef PIVOT_h
#define PIVOT_h

#include <AS5600.h>

#define DEGREES_PER_TICK 360.0/4096.0

class Pivot {
    private:
        uint8_t motorPinA1, motorPinA2, motorPinB1, motorPinB2;
        int enc_sda, enc_scl;
        AS5600 *encoderA, *encoderB;

        void setMotor(int A, int speed) {
            if (speed > 0) {
                analogWrite(A ? this->motorPinA1 : this->motorPinB1, speed);
                analogWrite(A ? this->motorPinA2 : this->motorPinB2, 0);
            } else if (speed < 0) {
                analogWrite(A ? this->motorPinA1 : this->motorPinB1, 0);
                analogWrite(A ? this->motorPinA2 : this->motorPinB2, -speed);
            } else {
                analogWrite(A ? this->motorPinA1 : this->motorPinB1, 0);
                analogWrite(A ? this->motorPinA2 : this->motorPinB2, 0);
            }
        }

    public:
        Pivot(uint8_t pinA1, uint8_t pinA2, uint8_t pinB1, uint8_t pinB2, int enc1_sda, int enc1_scl, int enc2_sda, int enc2_scl, AS5600 *encoderA, AS5600 *encoderB) :
        motorPinA1(pinA1), motorPinA2(pinA2), motorPinB1(pinB1), motorPinB2(pinB2), enc_sda(enc1_sda), 
        enc_scl(enc1_scl), encoderA(encoderA), encoderB(encoderB) {}

        void setup() {
            pinMode(motorPinA1, OUTPUT);
            pinMode(motorPinA2, OUTPUT);
            pinMode(motorPinB1, OUTPUT);
            pinMode(motorPinB2, OUTPUT);

            Wire.begin(enc1_sda, enc1_scl);
            Wire.begin(enc2_sda, enc2_scl);
            encoderA->begin();
            encoderB->begin();
        }

        void setMotorA(int speed) {
            setMotor(1, speed);
        }

        void setMotorB(int speed) {
            setMotor(0, -speed);
        }

        float getA() {
            return encoderA->readAngle() * DEGREES_PER_TICK;
        }

        float getB() {
            return encoderB->readAngle() * DEGREES_PER_TICK;
        }

        // do pid outside the object homie
};

#endif
