#ifndef single_pin_encoder_h
#define single_pin_encoder_h

enum Direction { POSITIVE, NEGATIVE };

struct Speed {
  unsigned long Duration;
  long Steps;
};

typedef void (*SpeedHandler)(Speed speed);

class SinglePinEncoder
{
  public:
    static const unsigned long DEFAULT_CHECK_PIN_PERIOD = 500;
    static const unsigned long DEFAULT_SPEED_HANDLER_PERIOD = 1000000;
  
    SinglePinEncoder();
    SinglePinEncoder(int pin);
    SinglePinEncoder(int pin, unsigned long checkPinPeriod);

    void init(int pin);
    void init(int pin, unsigned long checkPinPeriod);
    void update();
    void update(unsigned long currentMicros);
    
    void setDirection(Direction direction);
    Direction getDirection();

    void setCheckPinPeriod(unsigned long periodInMicros);
    unsigned long getCheckPinPeriod();

    long getPositiveSteps();
    long getNegativeSteps();
    long getSteps();
    void clearSteps();

    Speed getSpeed(unsigned long currentMicros);
    void setSpeedHandler(SpeedHandler speedHandler);
    void clearSpeedHandler();
    void setSpeedHandlerPeriod(unsigned long periodInMicros);
    unsigned long getSpeedHandlerPeriod();
  private:
    int pin;
    unsigned long checkPinPeriod;
    unsigned long previousCheckPinMicros;
    unsigned long previousMeasureSpeedMicros;
    int previousPinValue;
    bool checkingPinValue;

    Direction direction;
    long positiveSteps;
    long negativeSteps;

    void onPinValueChanged(bool pinValue);

    long previousPositiveSteps;
    long previousNegativeSteps;

    SpeedHandler speedHandler;
    unsigned long speedHandlerPeriod;
    void processSpeedHandler(unsigned long currentMicros);
};

#endif
