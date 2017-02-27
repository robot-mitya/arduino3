#include "rm_equ.h"
#include "motordriver_4wd.h"
#include <Arduino.h>

using namespace robot_mitya;

#define PIN_HEADLIGHTS 13

void Equipment::initialize()
{
  pinMode(PIN_HEADLIGHTS, OUTPUT);
  MOTOR.init();
}

void Equipment::zero()
{
  Equipment::headlights(false);
  Equipment::motorStop();
}

void Equipment::motorLeft(int speed)
{
  unsigned char direction;
  processSpeedAndDirection(speed, direction);
  MOTOR.setSpeedDir1(speed, direction);
}

void Equipment::motorRight(int speed)
{
  unsigned char direction;
  processSpeedAndDirection(speed, direction);
  MOTOR.setSpeedDir2(speed, direction);
}

void Equipment::motorBoth(int speed)
{
  unsigned char direction;
  processSpeedAndDirection(speed, direction);
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
  digitalWrite(PIN_HEADLIGHTS, turnOn);
}

