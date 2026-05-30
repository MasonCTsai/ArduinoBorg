#ifndef DRIVEN_MOTOR_h
#define DRIVEN_MOTOR_h

#include <AccelStepper.h>
#include <TMCStepper.h>

struct DrivenMotorConfiguration {
    bool pdn_disable; // true = use PDN/UART pin for communication
    int toff; // [1..15] enable driver_p in software
    int blank_time; // [16, 24, 36, 54]
    bool mstep_reg_select;
    int microsteps; //note that MicroPlyer will interpolate to 256
    bool stealthchop;

    DrivenMotorConfiguration(bool pdn_disable, int toff, int blank_time, bool mstep_reg_select, int microsteps, bool stealthchop)
        : pdn_disable(pdn_disable), toff(toff), blank_time(blank_time), mstep_reg_select(mstep_reg_select),
          microsteps(microsteps), stealthchop(stealthchop) {}
};

class DrivenMotor {
    private:
        AccelStepper *stepper;
        TMC2209Stepper *driver;
        const DrivenMotorConfiguration *configs;

    public:
        static const DrivenMotorConfiguration DEFAULT_CONFIGS;
        static const uint8_t HOMING_THRESHOLD;

        DrivenMotor(uint8_t pinSTP, uint8_t pinDIR, uint8_t address, const DrivenMotorConfiguration *configs) :
            stepper(new AccelStepper(AccelStepper::DRIVER, pinSTP, pinDIR)),
            driver(new TMC2209Stepper(&Serial2, 0.11f, address)),
            configs(configs) {
            initializeDriver(configs);
            stepper->setMaxSpeed(1000);
        }
    
        // https://forum.arduino.cc/t/using-a-tmc2209-silent-stepper-motor-driver-with-an-arduino/666992/111
        // https://forum.arduino.cc/t/esp32-tmc2209-and-uart-control-of-microstepping/1237112/20
        void initializeDriver(const DrivenMotorConfiguration *configs) {
            driver->begin();
            driver->pdn_disable(configs->pdn_disable);      // Use PDN/UART pin for communication
            driver->toff(configs->toff);                // [1..15] enable driver_p in software
            driver->blank_time(configs->blank_time);         // [16, 24, 36, 54]
            driver->mstep_reg_select(configs->mstep_reg_select);
            driver->microsteps(configs->microsteps);  //note that MicroPlyer will interpolate to 256
            driver->pwm_autoscale(configs->stealthchop);    // Needed for stealthChop
            driver->en_spreadCycle(!configs->stealthchop);  // false = StealthChop / true = SpreadCycle, Spreadcycle can have higher RPM but is louder
        }

        void setSpeed(float speed) {
            stepper->setSpeed(speed);
        }

        void run() {
            stepper->runSpeed();
        }

        void moveTo(long position) {
            stepper->moveTo(position);
        }

        bool runToTarget() {
            return stepper->run();  // true if still moving to target
        }

        long currentPosition() {
            return stepper->currentPosition();
        }

        void setCurrentPosition(long position) {
            stepper->setCurrentPosition(position);
        }

        void setAcceleration(float accel) {
            stepper->setAcceleration(accel);
        }

        uint8_t sg_result() { return driver->SG_RESULT(); }
};

const DrivenMotorConfiguration DrivenMotor::DEFAULT_CONFIGS = DrivenMotorConfiguration(true, 3, 24, true, 4, true);
const uint8_t DrivenMotor::HOMING_THRESHOLD = 10;

#endif