#ifndef single_pin_encoder_h
#define single_pin_encoder_h

enum Direction { POSITIVE, NEGATIVE };

class SinglePinEncoder
{
  public:
    static const unsigned long DEFAULT_CHECK_PIN_PERIOD = 500;
    static const unsigned long DEFAULT_MEASURE_SPEED_PERIOD = 500000 * 2;
  
    SinglePinEncoder();
    SinglePinEncoder(int pin);
    SinglePinEncoder(int pin, unsigned long checkPinPeriod, unsigned long measureSpeedPeriod);

    void init(int pin);
    void init(int pin, unsigned long checkPinPeriod, unsigned long measureSpeedPeriod);
    void update();
    void update(unsigned long currentMicros);
    void clear();
    
    void setDirection(Direction direction);
    Direction getDirection();

    void setCheckPinPeriod(unsigned long periodInMicros);
    unsigned long getCheckPinPeriod();

    void setMeasureSpeedPeriod(unsigned long periodInMicros);
    unsigned long getMeasureSpeedPeriod();

    long GetPositiveSteps();
    long GetNegativeSteps();
    long GetSteps();

    void GetSpeed(unsigned long &duration, long &steps);
  private:
    int pin;
    unsigned long checkPinPeriod;
    unsigned long previousCheckPinMicros;
    unsigned long measureSpeedPeriod;
    unsigned long previousMeasureSpeedMicros;
    int previousPinValue;
    bool checkingPinValue;

    Direction direction;
    long positiveSteps;
    long negativeSteps;

    void onPinValueChanged(bool pinValue);

    unsigned long speedMeasureDuration;
    long speedMeasureSteps;
    long previousPositiveSteps;
    long previousNegativeSteps;
    void calculateSpeed(unsigned long currentMicros);
};

#endif
