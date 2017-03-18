#include "rm_equ.h"
#include "rm_msg.h"

using namespace robot_mitya;

void setup() {
  Equipment::initialize();

  Message::initialize();
  Message::send(Message::RET_OK);
}

void loop() {
  Message::processInput(commandHandler);
  unsigned long currentMicros = micros();
  Equipment::update(currentMicros);
}

void commandHandler(Command command, int param1, int param2, int param3)
{
  switch (command)
  {
    case CMD_STATUS_REQUEST:
      Message::send(Message::RET_OK);
      break;
    case CMD_STATUS_RESPONSE:
      break;
    case CMD_MOTOR_LEFT:
      //sendEcho("!ML", param1, param2, param3);
      Equipment::motorLeft(param1);
      break;
    case CMD_MOTOR_RIGHT:
      //sendEcho("!MR", param1, param2, param3);
      Equipment::motorRight(param1);
      break;
    case CMD_MOTOR_BOTH:
      //sendEcho("!MB", param1, param2, param3);
      Equipment::motorBoth(param1);
      break;
    case CMD_LED:
      Equipment::headlights(param1 != 0);
      break;
    case CMD_ENCL_REQUEST:
      if (param1 == 1)
        Equipment::clearLeftEncoderSteps();
      Message::sendENCL(Equipment::getLeftEncoderSteps());
      break;
    case CMD_ENCR_REQUEST:
      if (param1 == 1)
        Equipment::clearRightEncoderSteps();
      Message::sendENCR(Equipment::getRightEncoderSteps());
      break;
    case CMD_ENCB_REQUEST:
      if (param1 == 1)
      {
        Equipment::clearLeftEncoderSteps();
        Equipment::clearRightEncoderSteps();
      }
      Message::sendENCL(Equipment::getLeftEncoderSteps());
      Message::sendENCR(Equipment::getRightEncoderSteps());
      break;
    case CMD_DIST_REQUEST:
      if (param1 == -2) {
        Equipment::clearDistance();
        Message::sendDistance(Equipment::getDistance());
      } else if (param1 == -1) {
        Equipment::clearStepsHandler();
        Message::send(Message::RET_OK);
      } else if (param1 == 0) {
        Message::sendDistance(Equipment::getDistance());
      } else if (param1 > 0) {
        Equipment::setStepsHandler(param1);
        Message::send(Message::RET_OK);
      } else {
        Message::send(Message::RET_BAD_PARAMETER);
      }
      break;
    case CMD_SPD_REQUEST:
      if (param1 == -1) {
        Equipment::clearSpeedHandler();
        Message::send(Message::RET_OK);
      } else if (param1 == 0) {
        Equipment::getSpeed();
      } else if (param1 > 0) {
        Equipment::setSpeedHandler(param1);
        Message::send(Message::RET_OK);
      } else {
        Message::send(Message::RET_BAD_PARAMETER);
      }
      break;
    default:
      Message::send(Message::RET_BAD_COMMAND);
  }
}

char* intToString(int value)
{
  char result[7];
  itoa(value, result, 10);
  return result;
}

//void sendEcho(char commandText[25], int param1, int param2, int param3)
//{
//  Message::send(commandText);
//  Message::send(" ");
//  Message::send(intToString(param1));
//  Message::send(" ");
//  Message::send(intToString(param2));
//  Message::send(" ");
//  Message::send(intToString(param3));
//  Message::send(";\r\n");
//}

