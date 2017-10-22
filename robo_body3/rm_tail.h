// -------------------------------------------------------------------------------------
// file = "rm_tail.h" (Robot Mitya tail helper header file)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2017
//   This class is used to control Mitya's tail servo.
// -------------------------------------------------------------------------------------

#ifndef rm_tail_h
#define rm_tail_h

#include <ServoTimer2.h>
//#include <SoftwareServo.h>

namespace robot_mitya {

  #define MIN_DEGREE 0
  #define MAX_DEGREE 180
  #define MID_DEGREE 90
  
  // Servo will be detached after 1 second of idle state.
  #define DETACH_TIMEOUT_MICROS 1000000
  
  class ServoCommand {
    public:
      unsigned long id;
      ServoCommand();
      virtual int getDegree(unsigned long currentMicros) = 0;
      virtual bool isFinished() = 0;
      void setStartMicros(unsigned long startMicros) { this->startMicros = startMicros; }
    protected:
      unsigned long startMicros;
  };

  class Rotate : public ServoCommand {
    public:
      Rotate(int startDegree, int stopDegree, int speed);
      int getDegree(unsigned long currentMicros);
      bool isFinished() { return completed; }
    private:
      int startDegree;
      int stopDegree;
      unsigned long totalDuration;
      bool completed;
  };

  class Swing : public ServoCommand {
    public:
      Swing(int startDegree, int amplitude, unsigned long period, int halfperiods);
      int getDegree(unsigned long currentMicros);
      bool isFinished() { return completed; }
    protected:
      int startDegree;
      int amplitude;
      unsigned long period;
      unsigned long totalDuration;
      bool completed;
      long mysin(long amplitude, long period, long t);
  };

  class SwingDown : public Swing {
    public:
      SwingDown(int startDegree, int amplitude, unsigned long period, int halfperiods) : Swing(startDegree, amplitude, period, halfperiods) {}
      int getDegree(unsigned long currentMicros);
  };

  class Tail {
    public:
      Tail(int pin);
      ~Tail();
      void update();
      void update(unsigned long currentMicros);
      bool isIdle() { return command == NULL; }
      void stop();
      bool rotate(int degree, int speed); //speed in degrees per second
      bool swing(int period, int amplitude, int halfperiods); //period in millis, amplitude in degrees
      bool swingDown(int period, int startAmplitude, int halfperiods); //period in millis, amplitude in degrees
    private:
      ServoTimer2 servo;
      int pin;
      ServoCommand *command;
      unsigned long previousCommandId;
      int currentDegree;
      bool isAttached;
      unsigned long finishCommandMicros;
      void attach();
      void detach();
      void servoWrite(int degree);
  };
}

#endif
