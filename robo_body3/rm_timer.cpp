#include <Arduino.h>
#include "rm_timer.h"

using namespace robot_mitya;

TimerHelper::TimerHelper() {
  handler = NULL;
  firstHandlerIteration = true;
  handlerPeriod = DEFAULT_PERIOD_IN_MICROS;
}

void TimerHelper::update() {
  update(micros());
}

void TimerHelper::update(unsigned long currentMicros) {
  if (handler == NULL) return;

  unsigned long deltaTime = currentMicros - previousMicros;
  if (firstHandlerIteration || (deltaTime >= handlerPeriod)) {
    previousMicros = currentMicros;
    unsigned long phase1000;
    if (firstHandlerIteration) {
      phase1000 = 0;
      firstHandlerIteration = false;
    } else {
      phase1000 = deltaTime * 1000 / handlerPeriod;
      if (phase1000 >= 1000) phase1000 = 0;
    }
    handler(phase1000);
  }  
}

void TimerHelper::setHandler(TimerHandler timerHandler) {
  handler = timerHandler;
  firstHandlerIteration = true;
}

void TimerHelper::clearHandler() {
  handler = NULL;
  firstHandlerIteration = true;
}

void TimerHelper::setHandlerPeriod(unsigned long periodInMicros) {
  handlerPeriod = periodInMicros;
}

unsigned long TimerHelper::getHandlerPeriod() {
  return handlerPeriod;
}

