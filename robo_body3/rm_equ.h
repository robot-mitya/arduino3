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

namespace robot_mitya {
  class Equipment {
    public:
      static void initialize();
      static void zero();
      static void update(unsigned long currentMicros);

      static void motorLeft(int speed); //speed: -100..+100
      static void motorRight(int speed);//speed: -100..+100
      static void motorBoth(int speed); //speed: -100..+100
      static void motorStop();

      static void headlights(bool turnOn);
      static bool getHeadlights();

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
    private:
      static void processSpeedAndDirection(int &speed, unsigned char &direction);
      static long getSpeedValueInMetersPerHour(Speed speed);
  };
}

#endif
