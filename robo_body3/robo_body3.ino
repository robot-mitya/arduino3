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

void commandHandler(String command, int param1, int param2, int param3)
{
  //Message::send(command + "(" + param1 + "," + param2 + "," + param3 + ")\r\n");
  if (command.equals("?"))
  {
    Message::send(Message::RET_OK);
  }
  else if (command.equals("ML"))
  {
    Equipment::motorLeft(param1);
  }
  else if (command.equals("MR"))
  {
    Equipment::motorRight(param1);
  }
  else if (command.equals("MB"))
  {
    Equipment::motorBoth(param1);
  }
  else
  {
    Message::send(Message::RET_BAD_COMMAND);
  }
}

