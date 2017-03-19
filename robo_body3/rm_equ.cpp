#include "rm_cfg.h"
#include "rm_equ.h"
#include "rm_msg.h"
#include "rm_eeprom.h"
#include "motordriver_4wd.h"
#include <Arduino.h>

using namespace robot_mitya;

SinglePinEncoder leftEncoder(Cfg::PIN_LEFT_ENCODER);
SinglePinEncoder rightEncoder(Cfg::PIN_RIGHT_ENCODER);

static long micronsPerEncoderStep;

static long leftEncoderDistance;
static long rightEncoderDistance;

static long leftEncoderSpeedInMetersPerHour;
static long rightEncoderSpeedInMetersPerHour;

static int speedHandlerPeriodInMillis;
static bool waitingSpeedData;

void Equipment::initialize()
{
  pinMode(Cfg::PIN_HEADLIGHTS, OUTPUT);
  MOTOR.init();

  micronsPerEncoderStep = Eeprom::readMicronsPerEncoderStep();

  // "Prewarm" encoders' pins. First pin reading contain garbage data.
  // Before regular use of Equipment::update we simulate it for 0.5 sec
  // in initialize section. After that we call Equipment::zero method.
  unsigned long startMicros = micros();
  unsigned long currentMicros = startMicros;
  do {
    currentMicros = micros();
    leftEncoder.update(currentMicros);
    rightEncoder.update(currentMicros);
  } while (currentMicros - startMicros < 500000);
  Equipment::zero();
}

void Equipment::zero()
{
  Equipment::headlights(false);
  Equipment::motorStop();
  Equipment::clearLeftEncoderSteps();
  Equipment::clearRightEncoderSteps();
  Equipment::clearStepsHandler();
  Equipment::clearSpeedHandler();
  speedHandlerPeriodInMillis = -1;
  waitingSpeedData = false;
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

void Equipment::updateMicronsPerEncoderStep(int micronsPerStep) {
  micronsPerEncoderStep = micronsPerStep;
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

void Equipment::clearDistance() {
  leftEncoder.clearSteps();
  rightEncoder.clearSteps();
  leftEncoderDistance = 0;
  rightEncoderDistance = 0;
}

long Equipment::getDistance() {
  leftEncoderDistance = micronsPerEncoderStep * leftEncoder.getSteps();
  rightEncoderDistance = micronsPerEncoderStep * rightEncoder.getSteps();
  return (leftEncoderDistance + rightEncoderDistance) / 2;
}

void Equipment::leftEncoderStepsHandler(long steps) {
  leftEncoderDistance = micronsPerEncoderStep * steps;
}

void Equipment::rightEncoderStepsHandler(long steps) {
  rightEncoderDistance = micronsPerEncoderStep * steps;

  // According to Equipment::update rightEncoderStepsHandler is called after
  // leftEncoderStepsHandler. So leftEncoderDistance has been initialized.
  Message::sendDistance((leftEncoderDistance + rightEncoderDistance) / 2);
}

void Equipment::clearStepsHandler() {
  leftEncoder.clearStepsHandler();
  rightEncoder.clearStepsHandler();
}

void Equipment::setStepsHandler(unsigned long periodInMillis) {
  leftEncoder.setStepsHandlerPeriod(periodInMillis * 1000);
  rightEncoder.setStepsHandlerPeriod(periodInMillis * 1000);
  
  leftEncoder.setStepsHandler(Equipment::leftEncoderStepsHandler);
  rightEncoder.setStepsHandler(Equipment::rightEncoderStepsHandler);
}

long Equipment::getSpeedValueInMetersPerHour(Speed speed) {
  // That's a tricky part. We have to calculate speed without integer overflow.
  // "* 36" and "/ 100" are 36000 seconds in an hour.
  long result = speed.Steps * micronsPerEncoderStep * 36;
  long duration = speed.Duration / 100;
  result /= duration;
  return result;
}

void Equipment::getSpeed() {
  if (waitingSpeedData) return;
  waitingSpeedData = true;

  leftEncoder.setSpeedHandlerPeriod(1000000); // (one second)
  rightEncoder.setSpeedHandlerPeriod(1000000);// (one second)
  
  leftEncoder.setSpeedHandler(Equipment::leftEncoderSpeedHandler);
  rightEncoder.setSpeedHandler(Equipment::rightEncoderSpeedHandler);
}

void Equipment::leftEncoderSpeedHandler(Speed speed) {
  leftEncoderSpeedInMetersPerHour = getSpeedValueInMetersPerHour(speed);
}

void Equipment::rightEncoderSpeedHandler(Speed speed) {
  rightEncoderSpeedInMetersPerHour = getSpeedValueInMetersPerHour(speed);
  
  // According to Equipment::update rightEncoderSpeedHandler is called after
  // leftEncoderSpeedHandler. So leftEncoderSpeedInMetersPerHour has been initialized.
  long averageSpeed = (leftEncoderSpeedInMetersPerHour + rightEncoderSpeedInMetersPerHour) / 2;
  Message::sendSpeed((int) averageSpeed);

  if (waitingSpeedData) {
    waitingSpeedData = false;
    leftEncoder.clearSpeedHandler();
    rightEncoder.clearSpeedHandler();
    Equipment::setSpeedHandler(speedHandlerPeriodInMillis);
  }
}

void Equipment::clearSpeedHandler() {
  leftEncoder.clearSpeedHandler();
  rightEncoder.clearSpeedHandler();
  speedHandlerPeriodInMillis = -1;
}

void Equipment::setSpeedHandler(unsigned long periodInMillis) {
  if (periodInMillis < 0) return;
  speedHandlerPeriodInMillis = periodInMillis;

  leftEncoder.setSpeedHandlerPeriod(periodInMillis * 1000);
  rightEncoder.setSpeedHandlerPeriod(periodInMillis * 1000);
  
  leftEncoder.setSpeedHandler(Equipment::leftEncoderSpeedHandler);
  rightEncoder.setSpeedHandler(Equipment::rightEncoderSpeedHandler);
}

