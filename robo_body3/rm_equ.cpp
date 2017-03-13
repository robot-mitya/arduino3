#include "rm_cfg.h"
#include "rm_equ.h"
#include "motordriver_4wd.h"
#include <single_pin_encoder.h>
#include <Arduino.h>

using namespace robot_mitya;

SinglePinEncoder leftEncoder(Cfg::PIN_LEFT_ENCODER);
SinglePinEncoder rightEncoder(Cfg::PIN_RIGHT_ENCODER);

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

