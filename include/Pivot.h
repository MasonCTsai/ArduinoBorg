#ifndef PIVOT_h
#define PIVOT_h

#include <AS5600.h>

#define DEGREES_PER_TICK 360.0/4096.0;

class Pivot {
    private:
        uint8_t motorPinA1, motorPinA2, motorPinB1, motorPinB2;
        int enc_sda, enc_scl;
        AS5600 *encoder;

        void setMotor(bool A, int speed) {
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
        Pivot(uint8_t pinA1, uint8_t pinA2, uint8_t pinB1, uint8_t pinB2, int enc_sda, int enc_scl, AS5600 *encoder) :
        motorPinA1(pinA1), motorPinA2(pinA2), motorPinB1(pinB1), motorPinB2(pinB2), enc_sda(enc_sda), 
        enc_scl(enc_scl), encoder(encoder) {}

        void setup() {
            pinMode(motorPinA1, OUTPUT);
            pinMode(motorPinA2, OUTPUT);
            pinMode(motorPinB1, OUTPUT);
            pinMode(motorPinB2, OUTPUT);

            Wire.begin(enc_sda, enc_scl);
            encoder->begin();
        }

        void set(int speed) {
            setMotor(true, speed);
            setMotor(false, -speed);
        }

        float get() {
            return encoder->readAngle() * DEGREES_PER_TICK;
        }

        // do pid outside the object homie
};

#endif