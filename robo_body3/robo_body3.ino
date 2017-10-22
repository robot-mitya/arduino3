#include "rm_equ.h"
#include "rm_msg.h"
#include "rm_eeprom.h"

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

void commandHandler(Command command, int param1, int param2, int param3) {
  switch (command) {
    case CMD_STATUS_REQUEST:
      Message::send(Message::RET_OK);
      break;
    case CMD_STATUS_RESPONSE:
      break;
    case CMD_RESET:
      Equipment::rebootController();
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
    case CMD_TAIL_ROTATE:
      if (!Equipment::tailRotate(param1, param2))
        Message::send(Message::RET_BAD_PARAMETER);
      break;
    case CMD_TAIL_SWING:
      if (!Equipment::tailSwing(param1, param2, param3))
        Message::send(Message::RET_BAD_PARAMETER);
      break;
    case CMD_TAIL_SWING_A:
      if (!Equipment::tailSwingDown(param1, param2, param3))
        Message::send(Message::RET_BAD_PARAMETER);
      break;
    case CMD_TAIL_FREEZE:
      Equipment::tailFreeze();
      break;
    case CMD_LED_1:
      Equipment::led1(param1 != 0);
      break;
    case CMD_LED_1_REQUEST:
      Message::sendLed1(Equipment::getLed1());
      break;
    case CMD_LED_2:
      Equipment::led2(param1 != 0);
      break;
    case CMD_LED_2_REQUEST:
      Message::sendLed2(Equipment::getLed2());
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
      if (param1 == 1) {
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
    case CMD_BV_REQUEST:
      if (param1 == -1) {
        Equipment::clearBatteryVoltageHandler();
        Message::send(Message::RET_OK);
      } else if (param1 == 0) {
        Message::sendBatteryVoltage(Equipment::getBatteryVoltage());
      } else if (param1 > 0) {
        Equipment::setBatteryVoltageHandler(param1);
        Message::send(Message::RET_OK);
      } else {
        Message::send(Message::RET_BAD_PARAMETER);
      }
      break;
    case CMD_DV_REQUEST:
      if (param1 == -1) {
        Equipment::clearDcDcVoltageHandler();
        Message::send(Message::RET_OK);
      } else if (param1 == 0) {
        Message::sendDcDcVoltage(Equipment::getDcDcVoltage());
      } else if (param1 > 0) {
        Equipment::setDcDcVoltageHandler(param1);
        Message::send(Message::RET_OK);
      } else {
        Message::send(Message::RET_BAD_PARAMETER);
      }
      break;
    case CMD_MCPS_REQUEST:
      if (param1 < 0) {
        Message::send(Message::RET_BAD_PARAMETER);
      } else {
        if (param1 > 0) {
          Eeprom::writeMicronsPerEncoderStep(param1);
        }
        int micronsPerStep = Eeprom::readMicronsPerEncoderStep();
        Equipment::updateMicronsPerEncoderStep(micronsPerStep);
        Message::sendMicronsPerStep(micronsPerStep);
      }
      break;
    case CMD_BVF_REQUEST:
      if (param1 < 0) {
        Message::send(Message::RET_BAD_PARAMETER);
      } else {
        if (param1 > 0) {
          Eeprom::writeBatteryVoltageFactor(param1);
        }
        long batteryVoltageFactor = Eeprom::readBatteryVoltageFactor();
        Equipment::updateBatteryVoltageFactor(batteryVoltageFactor);
        Message::sendBatteryVoltageFactor(batteryVoltageFactor);
      }
      break;
    case CMD_DVF_REQUEST:
      if (param1 < 0) {
        Message::send(Message::RET_BAD_PARAMETER);
      } else {
        if (param1 > 0) {
          Eeprom::writeDcDcVoltageFactor(param1);
        }
        long dcDcVoltageFactor = Eeprom::readDcDcVoltageFactor();
        Equipment::updateDcDcVoltageFactor(dcDcVoltageFactor);
        Message::sendDcDcVoltageFactor(dcDcVoltageFactor);
      }
      break;
    default:
      Message::send(Message::RET_BAD_COMMAND);
  }
}

char* intToString(int value) {
  static char result[7];
  itoa(value, result, 10);
  return result;
}

//void sendEcho(char commandText[25], int param1, int param2, int param3) {
//  Message::send(commandText);
//  Message::send(" ");
//  Message::send(intToString(param1));
//  Message::send(" ");
//  Message::send(intToString(param2));
//  Message::send(" ");
//  Message::send(intToString(param3));
//  Message::send(";\r\n");
//}

