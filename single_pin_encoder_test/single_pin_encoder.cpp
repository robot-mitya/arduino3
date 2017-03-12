#include <Arduino.h>
#include "single_pin_encoder.h"

SinglePinEncoder::SinglePinEncoder() {
}

SinglePinEncoder::SinglePinEncoder(int pin) {
  init(pin);
}

SinglePinEncoder::SinglePinEncoder(int pin, unsigned long checkPinPeriod, unsigned long measureSpeedPeriod) {
  init(pin, checkPinPeriod, measureSpeedPeriod);
}


void SinglePinEncoder::init(int pin) {
  init(pin, DEFAULT_CHECK_PIN_PERIOD, DEFAULT_MEASURE_SPEED_PERIOD);
}

void SinglePinEncoder::init(int pin, unsigned long checkPinPeriod, unsigned long measureSpeedPeriod) {
  this->pin = pin;
  this->checkPinPeriod = checkPinPeriod;
  this->measureSpeedPeriod = measureSpeedPeriod;
  previousPinValue = LOW;
  direction = POSITIVE;
  clear();
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

    calculateSpeed(currentMicros);
  }  
}


void SinglePinEncoder::clear() {
  checkingPinValue = false;
  positiveSteps = 0;
  negativeSteps = 0;
  previousPositiveSteps = 0;
  previousNegativeSteps = 0;
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

void SinglePinEncoder::setMeasureSpeedPeriod(unsigned long periodInMicros) {
  measureSpeedPeriod = periodInMicros;
}

unsigned long SinglePinEncoder::getMeasureSpeedPeriod() {
  return measureSpeedPeriod;
}

long SinglePinEncoder::GetPositiveSteps() {
  return positiveSteps;
}

long SinglePinEncoder::GetNegativeSteps() {
  return negativeSteps;
}

long SinglePinEncoder::GetSteps() {
  return positiveSteps - negativeSteps;
}

void SinglePinEncoder::onPinValueChanged(bool pinValue) {
  if (direction == POSITIVE && pinValue == HIGH) positiveSteps++;
  else if (direction == NEGATIVE && pinValue == LOW) negativeSteps++;
}

void SinglePinEncoder::GetSpeed(unsigned long &duration, long &steps) {
  duration = speedMeasureDuration;
  steps = speedMeasureSteps;
}

void SinglePinEncoder::calculateSpeed(unsigned long currentMicros) {
  if (currentMicros - previousMeasureSpeedMicros >= measureSpeedPeriod) {
    speedMeasureDuration = currentMicros - previousMeasureSpeedMicros;
    speedMeasureSteps = positiveSteps - previousPositiveSteps - (negativeSteps - previousNegativeSteps);
    
//    Serial.print(positiveSteps);
//    Serial.print('\t');
//    Serial.print(previousPositiveSteps);
//    Serial.print('\t');
//    Serial.print(negativeSteps);
//    Serial.print('\t');
//    Serial.print(previousNegativeSteps);
//    Serial.print('\t');
//    Serial.print(speedMeasureSteps);
//    Serial.print('\t');
//    Serial.print(speedMeasureDuration);
//    Serial.println();

    previousPositiveSteps = positiveSteps;
    previousNegativeSteps = negativeSteps;
    previousMeasureSpeedMicros = currentMicros;
  }
}

