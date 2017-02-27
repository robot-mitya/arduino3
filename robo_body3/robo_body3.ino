#include "rm_equ.h"
#include "rm_msg.h"

using namespace robot_mitya;

void setup() {
  Equipment::initialize();
  Equipment::zero();

  Message::initialize();
  Message::send(Message::RET_OK);
}

void loop() {
  Message::processInput(commandHandler);
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

