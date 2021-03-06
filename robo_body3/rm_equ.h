// -------------------------------------------------------------------------------------
// file = "rm_equ.h" (Robot Mitya equipment header file)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2017
//   This static class initializes and controls Robot Mitya's equipment:
//   DC motors, head and tail servos, headlights etc.
// -------------------------------------------------------------------------------------

#ifndef rm_equ_h
#define rm_equ_h

#include <single_pin_encoder.h>
//#include "rm_timer.h"

namespace robot_mitya {
  class Equipment {
    public:
      static void initialize();
      static void zero();
      static void update(unsigned long currentMicros);

      static void rebootController();

      static void motorLeft(int speed); //speed: -100..+100
      static void motorRight(int speed);//speed: -100..+100
      static void motorBoth(int speed); //speed: -100..+100
      static void motorStop();

      static bool tailRotate(int degree, int speed); //degree: -90..+90, speed: degree per second
      static bool tailSwing(int period, int amplitude, int halfperiods); // period in millis, amplitude in degrees
      static bool tailSwingDown(int period, int amplitude, int halfperiods); // period in millis, amplitude in degrees
      static void tailFreeze();

      static void led1(bool turnOn);
      static bool getLed1();
      static void switchLed1();
      static void led2(bool turnOn);
      static bool getLed2();
      static void switchLed2();

      static void updateMicronsPerEncoderStep(int micronsPerStep);

      static void clearLeftEncoderSteps();
      static void clearRightEncoderSteps();
      static long getLeftEncoderSteps();
      static long getRightEncoderSteps();

      static void clearDistance();
      static long getDistance();
      static void clearStepsHandler();
      static void setStepsHandler(unsigned long periodInMillis);
      
      static void leftEncoderStepsHandler(long steps);
      static void rightEncoderStepsHandler(long steps);
      
      static void getSpeed();
      static void clearSpeedHandler();
      static void setSpeedHandler(unsigned long periodInMillis);

      static void leftEncoderSpeedHandler(Speed speed);
      static void rightEncoderSpeedHandler(Speed speed);

      static int getBatteryVoltage();
      static void clearBatteryVoltageHandler();
      static void setBatteryVoltageHandler(unsigned long periodInMillis);

      static int getDcDcVoltage();
      static void clearDcDcVoltageHandler();
      static void setDcDcVoltageHandler(unsigned long periodInMillis);

      static void updateBatteryVoltageFactor(long factor);
      static void updateDcDcVoltageFactor(long factor);
    private:
      static void processSpeedAndDirection(int &speed, unsigned char &direction);
      static long getSpeedValueInMetersPerHour(Speed speed);

      static void batteryVoltageHandler(unsigned long phase1000);
      static void dcDcVoltageHandler(unsigned long phase1000);
  };
}

#endif
