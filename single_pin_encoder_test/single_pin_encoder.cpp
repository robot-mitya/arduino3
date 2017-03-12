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
  init(pin, DEFAULT_CHECK_PIN_PERIOD);
}

void SinglePinEncoder::init(int pin, unsigned long checkPinPeriod) {
  this->pin = pin;
  this->checkPinPeriod = checkPinPeriod;
  previousPinValue = LOW;
  speedHandler = NULL;
  speedHandlerPeriod = DEFAULT_SPEED_HANDLER_PERIOD;
  direction = POSITIVE;
  clearSteps();
  pinMode(pin, INPUT);
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

long SinglePinEncoder::getPositiveSteps() {
  return positiveSteps;
}

long SinglePinEncoder::getNegativeSteps() {
  return negativeSteps;
}

long SinglePinEncoder::getSteps() {
  return positiveSteps - negativeSteps;
}

void SinglePinEncoder::clearSteps() {
  checkingPinValue = false;
  positiveSteps = 0;
  negativeSteps = 0;
  previousPositiveSteps = 0;
  previousNegativeSteps = 0;
}

void SinglePinEncoder::onPinValueChanged(bool pinValue) {
  if (direction == POSITIVE && pinValue == HIGH) positiveSteps++;
  else if (direction == NEGATIVE && pinValue == LOW) negativeSteps++;
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

