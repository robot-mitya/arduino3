#include "rm_cfg.h"
#include "rm_equ.h"
#include "rm_msg.h"
#include "motordriver_4wd.h"
#include <single_pin_encoder.h>
#include <Arduino.h>

using namespace robot_mitya;

#define STEP_MICRONS Cfg::STEP_MICRONS

SinglePinEncoder leftEncoder(Cfg::PIN_LEFT_ENCODER);
SinglePinEncoder rightEncoder(Cfg::PIN_RIGHT_ENCODER);

static long leftEncoderDistance;
static long rightEncoderDistance;

void Equipment::initialize()
{
  pinMode(Cfg::PIN_HEADLIGHTS, OUTPUT);
  MOTOR.init();
}

void Equipment::zero()
{
  Equipment::headlights(false);
  Equipment::motorStop();
  Equipment::clearLeftEncoderSteps();
  Equipment::clearRightEncoderSteps();
}

void Equipment::update(unsigned long currentMicros) {
  // Warning! Due to optimization, the order is important.
  // Fisrt leftEncoder.update, then rightEncoder.update:
  leftEncoder.update(currentMicros);
  rightEncoder.update(currentMicros);
}

void Equipment::motorLeft(int speed)
{
  unsigned char direction;
  processSpeedAndDirection(speed, direction);
  leftEncoder.setDirection(direction == DIRF ? POSITIVE : NEGATIVE);
  MOTOR.setSpeedDir1(speed, direction);
}

void Equipment::motorRight(int speed)
{
  unsigned char direction;
  processSpeedAndDirection(speed, direction);
  rightEncoder.setDirection(direction == DIRF ? POSITIVE : NEGATIVE);
  MOTOR.setSpeedDir2(speed, direction);
}

void Equipment::motorBoth(int speed)
{
  unsigned char direction;
  processSpeedAndDirection(speed, direction);
  leftEncoder.setDirection(direction == DIRF ? POSITIVE : NEGATIVE);
  rightEncoder.setDirection(direction == DIRF ? POSITIVE : NEGATIVE);
  MOTOR.setSpeedDir1(speed, direction);
  MOTOR.setSpeedDir2(speed, direction);
}

void Equipment::motorStop()
{
  MOTOR.setSpeedDir1(0, DIRF);
  MOTOR.setSpeedDir2(0, DIRF);
}

void Equipment::processSpeedAndDirection(int &speed, unsigned char &direction)
{
  if (speed < 0)
  {
    direction = DIRR;
    speed = -speed;
  }
  else
  {
    direction = DIRF;
  }
}

void Equipment::headlights(bool turnOn)
{
  digitalWrite(Cfg::PIN_HEADLIGHTS, turnOn);
}

void Equipment::clearLeftEncoderSteps() {
  leftEncoder.clearSteps();
}

void Equipment::clearRightEncoderSteps() {
  rightEncoder.clearSteps();
}

long Equipment::getLeftEncoderSteps() {
  return leftEncoder.getSteps();
}

long Equipment::getRightEncoderSteps() {
  return rightEncoder.getSteps();
}

void Equipment::clearEncoderDistance() {
  leftEncoder.clearSteps();
  rightEncoder.clearSteps();
  leftEncoderDistance = 0;
  rightEncoderDistance = 0;
}

int Equipment::getEncoderDistance() {
  leftEncoderDistance = STEP_MICRONS * leftEncoder.getSteps();
  rightEncoderDistance = STEP_MICRONS * rightEncoder.getSteps();
  return (leftEncoderDistance + rightEncoderDistance) / 2;
}

void Equipment::leftEncoderStepsHandler(long steps) {
  leftEncoderDistance = STEP_MICRONS * steps;
}

void Equipment::rightEncoderStepsHandler(long steps) {
  rightEncoderDistance = STEP_MICRONS * steps;

  // According to Equipment::update rightEncoderStepsHandler is called
  // after leftEncoderStepsHandler. So leftEncoderDistance is initialized.
  Message::sendDistance((leftEncoderDistance + rightEncoderDistance) / 2);
}

void clearStepsHandler() {
  leftEncoder.clearStepsHandler();
  rightEncoder.clearStepsHandler();
}

void setStepsHandler(unsigned long periodInMillis) {
  leftEncoder.setStepsHandlerPeriod(periodInMillis * 1000);
  rightEncoder.setStepsHandlerPeriod(periodInMillis * 1000);
  
  leftEncoder.setStepsHandler(Equipment::leftEncoderStepsHandler);
  rightEncoder.setStepsHandler(Equipment::rightEncoderStepsHandler);
}

