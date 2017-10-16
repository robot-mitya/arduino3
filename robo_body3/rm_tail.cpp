#include <Arduino.h>
#include "rm_tail.h"

using namespace robot_mitya;

static unsigned long commandCounter = 0;

Tail::Tail(int pin) {
  this->pin = pin;
  command = NULL;
  previousCommandId = 0;
  finishCommandMicros = 0;
  currentDegree = MID_DEGREE;
  isAttached = false;
}

Tail::~Tail() {
  detach();
}

void Tail::detach() {
  if (isAttached) {
    servo.detach();
    isAttached = false;
  }
}

void Tail::attach() {
  if (isAttached) return;
  servo.attach(pin);
  isAttached = true;
}

void Tail::stop() {
  if (command == NULL) return;
  attach();
  delete command;
  command = NULL;
  servo.write(currentDegree);
}

bool Tail::rotate(int degree, int speed) {
  stop();

  if (degree < MIN_DEGREE) degree = MIN_DEGREE;
  else if (degree > MAX_DEGREE) degree = MAX_DEGREE;
  if (speed < 0) return false;

  attach();
  command = new Rotate(currentDegree, degree, speed);
  return true;
}

bool Tail::swing(int period, int amplitude, int halfperiods) {
  stop();
  
  if (period < 0 || halfperiods < 0)
    return false;
  
  if (period == 0) {
    return true;
  }

  int a = currentDegree - MIN_DEGREE;
  if (MAX_DEGREE - currentDegree < a) a = MAX_DEGREE - currentDegree;
  if (amplitude < a) a = amplitude;
  amplitude = a;

  attach();
  command = new Swing(currentDegree, amplitude, period, halfperiods);
  return true;  
}

bool Tail::swingDown(int period, int startAmplitude, int halfperiods) {
  stop();

  if (period < 0 || halfperiods < 0)
    return false;

  if (period == 0) {
    return true;
  }

  int a = currentDegree - MIN_DEGREE;
  if (MAX_DEGREE - currentDegree < a) a = MAX_DEGREE - currentDegree;
  if (startAmplitude < a) a = startAmplitude;
  startAmplitude = a;

  attach();
  command = new SwingDown(currentDegree, startAmplitude, period, halfperiods);
  return true;  
}

void Tail::update() {
  update(micros());
}

void Tail::update(unsigned long currentMicros) {
  if (command != NULL && command->id != previousCommandId) { //(first command iteration)
    command->setStartMicros(currentMicros);
  }
  if (command == NULL && previousCommandId != 0) {
    finishCommandMicros = currentMicros;
  }
  previousCommandId = command == NULL ? 0 : command->id;

  if (command != NULL) {
    currentDegree = command->getDegree(currentMicros);
    servo.write(currentDegree);
    if (command->isFinished()) {
      delete command;
      command = NULL;
    }
  } else if (isAttached && (currentMicros - finishCommandMicros > DETACH_TIMEOUT_MICROS)) {
    detach();
  }
}


ServoCommand::ServoCommand() {
  id = ++commandCounter;
}

Rotate::Rotate(int startDegree, int stopDegree, int speed) {
  this->startDegree = startDegree;
  this->stopDegree = stopDegree;
  completed = false;

  if (speed == 0) {
    totalDuration = 0;
  } else {
    int deltaDegree = stopDegree - startDegree;
    if (deltaDegree < 0) deltaDegree = -deltaDegree;
    totalDuration = deltaDegree * 1000L / speed;
  }
}

int Rotate::getDegree(unsigned long currentMicros) {
  if (totalDuration == 0) {
    completed = true;
    return stopDegree;
  }
  unsigned long factor = (currentMicros - startMicros) / totalDuration;
  completed = factor > 1000L;
  if (completed) {
    factor = 1000L;
  }
  long deltaDegree = stopDegree - startDegree;
  deltaDegree *= factor;
  deltaDegree /= 1000;
  //return startDegree + (stopDegree - startDegree) * factor / 1000L;
  return startDegree + (int) deltaDegree;
}

long Swing::mysin(long amplitude, long period, long t) {
  long p1 = period / 4;
  long p3 = period - p1;
  t = t % period;
  if (t < p1)
    return t * amplitude / p1;
  if (t < p3)
    return 2 * amplitude - t * amplitude / p1;
  return t * amplitude / p1 - 4 * amplitude;
}

Swing::Swing(int startDegree, int amplitude, unsigned long period, int halfperiods) {
  this->startDegree = startDegree;
  this->amplitude = amplitude;
  this->period = period;
  totalDuration = period / 2 * halfperiods;
  completed = false;
}

int Swing::getDegree(unsigned long currentMicros) {
  unsigned long duration = (currentMicros - startMicros) / 1000L;
  if (totalDuration != 0 && duration > totalDuration) {
    completed = true;
    return startDegree;
  }
  //return startDegree + amplitude * sin(2.0f * PI * duration / period);
  return startDegree + mysin(amplitude, period, duration);
}

int SwingDown::getDegree(unsigned long currentMicros) {
  unsigned long progress = totalDuration == 0 ? 1000 : (currentMicros - startMicros) / totalDuration;
  if (progress > 1000) {
    completed = true;
    return startDegree;
  } else {
    unsigned long duration = (currentMicros - startMicros) / 1000L;
    const int breakPointA = 500;
    int currentAmplitude = progress < breakPointA || totalDuration == 0 ? amplitude : amplitude * (1000 - progress) / (1000 - breakPointA);
    int currentPeriod = period;
    //const int breakPointP = 600;
    //int currentPeriod = progress < breakPointP ? period : period * (1000 - progress) * 2 / (1000 - breakPointP);
    //return startDegree + currentAmplitude * sin(2.0f * PI * duration / currentPeriod);
    return startDegree + mysin(currentAmplitude, currentPeriod, duration);
  }
}

