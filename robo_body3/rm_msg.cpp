#include "rm_cfg.h"
#include "rm_msg.h"

#if defined(__AVR_ATmega32U4__)
  // Arduino Leonardo:
  #include <Arduino.h>
#else
  #include <HardwareSerial.h>
#endif

using namespace robot_mitya;

#define ROBO_SERIAL Serial

#define LOBYTE(v)        ((unsigned char) (v))
#define HIBYTE(v)        ((unsigned char) (((unsigned int) (v)) >> 8))
#define MAKEWORD(lo, hi) ((uint16_t) (lo) | ((uint16_t) (hi) << 8))

static const int NO_PARAMS_MESSAGE_SIZE = 6;
static const int ONE_PARAM_MESSAGE_SIZE = 8;
static const int TWO_PARAMS_MESSAGE_SIZE = 10;
static const int THREE_PARAMS_MESSAGE_SIZE = 12;

static const int MAX_MESSAGE_SIZE = THREE_PARAMS_MESSAGE_SIZE;
static uint8_t data[MAX_MESSAGE_SIZE];

static const int BUFFER_SIZE = 256;
static uint8_t buffer[BUFFER_SIZE];

static uint8_t message[MAX_MESSAGE_SIZE];
static int messagePos = 0;
static int messageSize;
static int errorStatus = 0; // 0 means no error.

static const int HEADER_INDEX_0 = 0;
static const int HEADER_INDEX_1 = 1;
static const int SIZE_INDEX = 2;
static const int COMMAND_INDEX = 3;
static const int CS_1_INDEX = 4;
static const int CS_2_INDEX = 5;
static const int PARAM_1_LSB_INDEX = 6;
static const int PARAM_1_MSB_INDEX = 7;
static const int PARAM_2_LSB_INDEX = 8;
static const int PARAM_2_MSB_INDEX = 9;
static const int PARAM_3_LSB_INDEX = 10;
static const int PARAM_3_MSB_INDEX = 11;

int Message::checksum1(uint8_t* data, int size)
{
  return 0x00;
}

int Message::checksum2(int cs1)
{
  return 0x00;
}

void Message::initialize() {
  data[0] = 0xFF;
  data[1] = 0xFF;
  ROBO_SERIAL.begin(Cfg::SERIAL_BAUD_RATE);
  ROBO_SERIAL.setTimeout(0);
}

void Message::send(int status) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_STATUS_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(status);
  data[PARAM_1_MSB_INDEX] = HIBYTE(status);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendLed1(bool isTurnedOn) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_LED_1_RESPONSE;
  data[PARAM_1_LSB_INDEX] = isTurnedOn ? 0x01 : 0x00;
  data[PARAM_1_MSB_INDEX] = 0x00;
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendLed2(bool isTurnedOn) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_LED_2_RESPONSE;
  data[PARAM_1_LSB_INDEX] = isTurnedOn ? 0x01 : 0x00;
  data[PARAM_1_MSB_INDEX] = 0x00;
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}


void Message::sendENCL(long steps) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_ENCL_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(steps);
  data[PARAM_1_MSB_INDEX] = HIBYTE(steps);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendENCR(long steps) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_ENCR_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(steps);
  data[PARAM_1_MSB_INDEX] = HIBYTE(steps);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendDistance(long distanceInMicrons) {
  long meters = distanceInMicrons / 1000000;
  long millimeters = (distanceInMicrons - meters * 1000000) / 1000;
  int length = TWO_PARAMS_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_DIST_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(meters);
  data[PARAM_1_MSB_INDEX] = HIBYTE(meters);
  data[PARAM_1_LSB_INDEX] = LOBYTE(millimeters);
  data[PARAM_1_MSB_INDEX] = HIBYTE(millimeters);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendSpeed(int speedInMetersPerHour) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_SPD_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(speedInMetersPerHour);
  data[PARAM_1_MSB_INDEX] = HIBYTE(speedInMetersPerHour);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendBatteryVoltage(int batteryVoltage) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_BV_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(batteryVoltage);
  data[PARAM_1_MSB_INDEX] = HIBYTE(batteryVoltage);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendDcDcVoltage(int dcDcVoltage) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_DV_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(dcDcVoltage);
  data[PARAM_1_MSB_INDEX] = HIBYTE(dcDcVoltage);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendMicronsPerStep(int micronsPerStep) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_MCPS_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(micronsPerStep);
  data[PARAM_1_MSB_INDEX] = HIBYTE(micronsPerStep);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendBatteryVoltageFactor(long batteryVoltageFactor) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_BVF_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(batteryVoltageFactor);
  data[PARAM_1_MSB_INDEX] = HIBYTE(batteryVoltageFactor);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::sendDcDcVoltageFactor(long dcDcVoltageFactor) {
  int length = ONE_PARAM_MESSAGE_SIZE;
  data[SIZE_INDEX] = length;
  data[COMMAND_INDEX] = CMD_DVF_RESPONSE;
  data[PARAM_1_LSB_INDEX] = LOBYTE(dcDcVoltageFactor);
  data[PARAM_1_MSB_INDEX] = HIBYTE(dcDcVoltageFactor);
  data[CS_1_INDEX] = checksum1(data, length);
  data[CS_2_INDEX] = checksum2(data[CS_1_INDEX]);
  ROBO_SERIAL.write(data, length);
}

void Message::processInput(void (*handler)(Command, int, int, int)) {
  while (ROBO_SERIAL.available() > 0) {
    int bufferSize = ROBO_SERIAL.readBytes(buffer, BUFFER_SIZE);
    
    for (int bufferPos = 0; bufferPos < bufferSize; bufferPos++) {

      if ((messagePos == 0) && (buffer[bufferPos] != 0xFF)) {
        errorStatus = RET_NOISE_RECEIVED;
        continue;
      } else if ((messagePos == 1) && (buffer[bufferPos] != 0xFF)) {
        errorStatus = RET_NOISE_RECEIVED;
        continue;
      }

      if (messagePos == 2) {
        messageSize = buffer[bufferPos];
        if ((messageSize != NO_PARAMS_MESSAGE_SIZE) &&
            (messageSize != ONE_PARAM_MESSAGE_SIZE) &&
            (messageSize != TWO_PARAMS_MESSAGE_SIZE) &&
            (messageSize != THREE_PARAMS_MESSAGE_SIZE)) {
          errorStatus = RET_WRONG_PARAMS_COUNT;
          continue;
        }
      }

      if (errorStatus != 0) {
        send(errorStatus);
        errorStatus = 0;
      }
      message[messagePos++] = buffer[bufferPos];

      if (messagePos == messageSize) {
        uint8_t cs1 = checksum1(message, messageSize);
        uint8_t cs2 = checksum2(cs1);
        if ((message[CS_1_INDEX] == cs1) && (message[CS_2_INDEX] == cs2)) {
          int param1 = messageSize > NO_PARAMS_MESSAGE_SIZE ? MAKEWORD(message[PARAM_1_LSB_INDEX], message[PARAM_1_MSB_INDEX]) : 0;
          int param2 = messageSize > ONE_PARAM_MESSAGE_SIZE ? MAKEWORD(message[PARAM_2_LSB_INDEX], message[PARAM_2_MSB_INDEX]) : 0;
          int param3 = messageSize > TWO_PARAMS_MESSAGE_SIZE ? MAKEWORD(message[PARAM_3_LSB_INDEX], message[PARAM_3_MSB_INDEX]) : 0;
          handler(message[COMMAND_INDEX], param1, param2, param3);
        } else {
          send(RET_CS_ERROR);
        }
        messagePos = 0;
      }
    }
  }
}

