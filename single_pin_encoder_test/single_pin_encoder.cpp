#include <Arduino.h>
#include "single_pin_encoder.h"

SinglePinEncoder::SinglePinEncoder() {
}

SinglePinEncoder::SinglePinEncoder(int pin) {
  init(pin);
}

SinglePinEncoder::SinglePinEncoder(int pin, unsigned long checkPinPeriod) {
  init(pin, checkPinPeriod);
}


void SinglePinEncoder::init(int pin) {
  init(pin, DEFAULT_CHECK_PIN_PERIOD_IN_MICROS);
}

void SinglePinEncoder::init(int pin, unsigned long checkPinPeriod) {
  this->pin = pin;
  this->checkPinPeriod = checkPinPeriod;
  previousPinValue = LOW;
  stepsHandler = NULL;
  speedHandler = NULL;
  direction = POSITIVE;
  clearSteps();
  pinMode(pin, INPUT);
  firstStepsHandlerIteration = false;
  firstSpeedHandlerIteration = false;
}


void SinglePinEncoder::update() {
  update(micros());
}

void SinglePinEncoder::update(unsigned long currentMicros) {
  if (currentMicros - previousCheckPinMicros >= checkPinPeriod) {
    previousCheckPinMicros = currentMicros;
    
    int currentPinValue = digitalRead(pin);

    if (checkingPinValue) {
      if (currentPinValue == previousPinValue) {
        checkingPinValue = false;
        onPinValueChanged(currentPinValue);
      } else {
        previousPinValue = currentPinValue;
      }
    } else {
      if (currentPinValue != previousPinValue) {
        previousPinValue = currentPinValue;
        checkingPinValue = true;
      }
    }

    processStepsHandler(currentMicros);
    processSpeedHandler(currentMicros);
  }  
}


void SinglePinEncoder::setDirection(Direction direction) {
  this->direction = direction;
}

Direction SinglePinEncoder::getDirection() {
  return direction;
}

void SinglePinEncoder::setCheckPinPeriod(unsigned long periodInMicros) {
  checkPinPeriod = periodInMicros;
}

unsigned long SinglePinEncoder::getCheckPinPeriod() {
  return checkPinPeriod;
}

void SinglePinEncoder::onPinValueChanged(bool pinValue) {
  if (direction == POSITIVE && pinValue == HIGH) positiveSteps++;
  else if (direction == NEGATIVE && pinValue == LOW) negativeSteps++;
}

void SinglePinEncoder::clearSteps() {
  checkingPinValue = false;
  positiveSteps = 0;
  negativeSteps = 0;
  previousPositiveSteps = 0;
  previousNegativeSteps = 0;
}

long SinglePinEncoder::getPositiveSteps() {
  return positiveSteps;
}

long SinglePinEncoder::getNegativeSteps() {
  return negativeSteps;
}

long SinglePinEncoder::getSteps() {
  return positiveSteps - negativeSteps;
}

void SinglePinEncoder::processStepsHandler(unsigned long currentMicros) {
  if (stepsHandler == NULL) return;
  
  if (firstStepsHandlerIteration) {
    firstStepsHandlerIteration = false;
    previousMeasureStepsMicros = currentMicros;
  }

  if (currentMicros - previousMeasureStepsMicros >= stepsHandlerPeriod) {
    stepsHandler(positiveSteps - negativeSteps);
    previousMeasureStepsMicros = currentMicros;
  }
}

void SinglePinEncoder::setStepsHandler(StepsHandler stepsHandler) {
  this->stepsHandler = stepsHandler;
  firstStepsHandlerIteration = true;
}

void SinglePinEncoder::clearStepsHandler() {
  stepsHandler = NULL;
}

void SinglePinEncoder::setStepsHandlerPeriod(unsigned long periodInMicros) {
  stepsHandlerPeriod = periodInMicros;
}

unsigned long SinglePinEncoder::getStepsHandlerPeriod() {
  return stepsHandlerPeriod;
}

Speed SinglePinEncoder::getSpeed(unsigned long currentMicros) {
  Speed result;

  if (speedHandler != NULL) {
    result.Duration = 0;
    result.Steps = 0;
    return result;
  }
  
  result.Duration = currentMicros - previousMeasureSpeedMicros;
  result.Steps = positiveSteps - previousPositiveSteps - (negativeSteps - previousNegativeSteps);

  previousPositiveSteps = positiveSteps;
  previousNegativeSteps = negativeSteps;
  previousMeasureSpeedMicros = currentMicros;

  return result;
}

void SinglePinEncoder::processSpeedHandler(unsigned long currentMicros) {
  if (speedHandler == NULL) return;

  if (firstSpeedHandlerIteration) {
    firstSpeedHandlerIteration = false;
    previousPositiveSteps = positiveSteps;
    previousNegativeSteps = negativeSteps;
    previousMeasureSpeedMicros = currentMicros;
  }

  if (currentMicros - previousMeasureSpeedMicros >= speedHandlerPeriod) {
    Speed speed;    
    speed.Duration = currentMicros - previousMeasureSpeedMicros;
    speed.Steps = positiveSteps - previousPositiveSteps - (negativeSteps - previousNegativeSteps);
    speedHandler(speed);
  
    previousPositiveSteps = positiveSteps;
    previousNegativeSteps = negativeSteps;
    previousMeasureSpeedMicros = currentMicros;
  }
}

void SinglePinEncoder::setSpeedHandler(SpeedHandler speedHandler) {
  this->speedHandler = speedHandler;
  firstSpeedHandlerIteration = true;
}

void SinglePinEncoder::clearSpeedHandler() {
  speedHandler = NULL;
}

void SinglePinEncoder::setSpeedHandlerPeriod(unsigned long periodInMicros) {
  speedHandlerPeriod = periodInMicros;
}

unsigned long SinglePinEncoder::getSpeedHandlerPeriod() {
  return speedHandlerPeriod;
}

