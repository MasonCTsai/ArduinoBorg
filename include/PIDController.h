#ifndef PID_CONTROLLER_h
#define PID_CONTROLLER_h

struct PIDConfigs {
    float kP;
    float kI;
    float kD;
    float delta;
    float integral_limit;

    PIDConfigs(float kP, float kD, float delta) :
        kP(kP), kD(kD), delta(delta) { 
        this->kI = 0.0;
        this->integral_limit = 0.0; 
    }

    PIDConfigs* withI(float kI, float limit) { 
        this->kI = kI;
        this->integral_limit = limit; 
        return this;
    }
};

struct ToleranceConfigs {
    float errorTolerance, derivative_tolerance;

    ToleranceConfigs(float errorTolerance, float derivativeTolerance) :
        errorTolerance(errorTolerance), derivative_tolerance(derivativeTolerance) {}
};

class PIDController {
    private:
        float setPoint, prevError;
        float integral;
        float derivative;

        const PIDConfigs* configs;
        const ToleranceConfigs* tolConfigs;
    public:
        static const ToleranceConfigs DEFAULT_TOLERANCES;

        PIDController(const PIDConfigs *configs, const ToleranceConfigs *tolConfigs) : configs(configs), tolConfigs(tolConfigs) {
            this->setPoint = 0.0;
            reset();
        }

        PIDController(const PIDConfigs *configs) {
            PIDController(configs, &DEFAULT_TOLERANCES);
        }

        void reset() {
            this->prevError = 0.0;
            this->integral = 0.0;
        }

        PIDController* withConfigs(PIDConfigs *configs) { this->configs = configs; return this; }

        float calculate(float measurement) {
            float error = this->setPoint - measurement;
            float P = this->configs->kP * error;
            
            this->integral += error * this->configs->delta;
            if (this->integral > this->configs->integral_limit) this->integral = this->configs->integral_limit;
            if (this->integral < -this->configs->integral_limit) this->integral = -this->configs->integral_limit;
            float I = this->configs->kI * this->integral;

            this->derivative = (error - this->prevError) / this->configs->delta;
            float D = this->configs->kD * this->derivative;
            this->prevError = error;

            return P + I + D;
        }

        float calculate(float measurement, float setPoint) {
            this->setPoint = setPoint;
            return calculate(measurement);
        }

        bool atSetPoint() {
            return this->derivative <= this->tolConfigs->derivative_tolerance &&
                    this->prevError <= this->tolConfigs->errorTolerance;
        }
};

const ToleranceConfigs PIDController::DEFAULT_TOLERANCES = ToleranceConfigs(0.05, 0.05);

#endif