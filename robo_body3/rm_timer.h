// -------------------------------------------------------------------------------------
// file = "rm_timer.h" (Robot Mitya timer helper header file)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2017
//   This class is used to call some handler periodically.
// -------------------------------------------------------------------------------------

#ifndef rm_timer_h
#define rm_timer_h

namespace robot_mitya {
  typedef void (*TimerHandler)(unsigned long phase1000); // (phase from 0 to 999)
  
  class TimerHelper {
    public:
      static const unsigned long DEFAULT_PERIOD_IN_MICROS = 2000;
    
      TimerHelper();

      void update();
      void update(unsigned long currentMicros);
  
      void setHandler(TimerHandler timerHandler);
      void clearHandler();
      void setHandlerPeriod(unsigned long periodInMicros);
      unsigned long getHandlerPeriod();
    private:
      int pin;

      TimerHandler handler;
      bool firstHandlerIteration;
      unsigned long handlerPeriod;
      unsigned long previousMicros;
  };
}

#endif
