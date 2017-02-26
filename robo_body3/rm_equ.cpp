#include "rm_equ.h"
#include "motordriver_4wd.h"

using namespace robot_mitya;

void Equipment::initialize()
{
  MOTOR.init();
}

void Equipment::zero()
{
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

