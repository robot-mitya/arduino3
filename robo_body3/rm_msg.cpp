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

static char charArray[2] = "\0"; // + null terminating (2 chars)
static char commandText[11] = "";
static char param1Text[7] = "";
static char param2Text[7] = "";
static char param3Text[7] = "";
static int wordCounter = 0;
static bool hasError = false;
static bool inWordSeparator = false;

void Message::initialize() {
  ROBO_SERIAL.begin(Cfg::SERIAL_BAUD_RATE);
}

void Message::send(char *message) {
  ROBO_SERIAL.print(message);
}

void Message::send(int status) {
  ROBO_SERIAL.print("!");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(status);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);
}

void Message::sendLED(bool isTurnedOn) {
  ROBO_SERIAL.print("!LED");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(isTurnedOn ? 1 : 0);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);
}

void Message::sendLEDA(int brightness) {
  ROBO_SERIAL.print("!LEDA");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  if (brightness < 0) brightness = 0;
  else if (brightness > 255) brightness = 255;
  ROBO_SERIAL.print(brightness);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);
}


void Message::sendENCL(long steps) {
  ROBO_SERIAL.print("!ENCL");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(steps);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);
}

void Message::sendENCR(long steps) {
  ROBO_SERIAL.print("!ENCR");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(steps);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);
}

void Message::sendDistance(long distanceInMicrons) {
  long meters = distanceInMicrons / 1000000;
  long millimeters = (distanceInMicrons - meters * 1000000) / 1000;
  ROBO_SERIAL.print("!DIST");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print((int) meters);
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print((int) millimeters);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);
}

void Message::sendSpeed(int speedInMetersPerHour) {
  ROBO_SERIAL.print("!SPD");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(speedInMetersPerHour);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);  
}

void Message::sendBatteryVoltage(int batteryVoltage) {
  ROBO_SERIAL.print("!BV");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(batteryVoltage);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);  
}

void Message::sendDcDcVoltage(int dcDcVoltage) {
  ROBO_SERIAL.print("!DV");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(dcDcVoltage);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);  
}

void Message::sendMicronsPerStep(int micronsPerStep) {
  ROBO_SERIAL.print("!MCPS");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(micronsPerStep);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);  
}

void Message::sendBatteryVoltageFactor(long batteryVoltageFactor) {
  ROBO_SERIAL.print("!BVF");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(batteryVoltageFactor);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);  
}

void Message::sendDcDcVoltageFactor(long dcDcVoltageFactor) {
  ROBO_SERIAL.print("!DVF");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(dcDcVoltageFactor);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);  
}

void Message::processInput(void (*handler)(Command, int, int, int)) {
  char ch;
  Command command;
  int value1, value2, value3;
  while (ROBO_SERIAL.available() > 0) {
    ch = (char)ROBO_SERIAL.read();

    if (ch == '\n' || ch == '\r') {
      continue;
    }

    if (ch == COMMAND_SEPARATOR && handler != NULL) {
      if (getCommand(commandText, command)) {
        if (getParam(param1Text, value1) && 
            getParam(param2Text, value2) && 
            getParam(param3Text, value3)) {
          if (!hasError) {
            if (command != CMD_UNKNOWN || value1 != 0 || value2 != 0 || value3 != 0) {
              handler(command, value1, value2, value3);
            }
          }
        } else {
          send(RET_BAD_PARAMETER);
        }
      } else {
        send(RET_BAD_COMMAND);
      }
      strcpy(commandText, "");
      strcpy(param1Text, "");
      strcpy(param2Text, "");
      strcpy(param3Text, "");
      wordCounter = 0;
      hasError = false;
      inWordSeparator = false;
      continue;
    }

    //(white space)
    if (ch <= 32) {
      if (!inWordSeparator) {
        inWordSeparator = true;
        wordCounter++;
      }
      continue;
    }

    inWordSeparator = false;
    charArray[0] = ch;
    switch (wordCounter) {
      case 0:
        strcat(commandText, charArray);
        break;
      case 1:
        strcat(param1Text, charArray);
        break;
      case 2:
        strcat(param2Text, charArray);
        break;
      case 3:
        strcat(param3Text, charArray);
        break;
      default:
        if (!hasError) send(RET_TOO_MANY_WORDS);
        hasError = true;
    }
  }  
}

int Message::charToInt(char ch) {
  switch (ch) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    default: return -1;
  }
}

bool Message::getCommand(char *text, Command &command) {
  int length = strlen(text);
  if (length == 0) {
    command = CMD_UNKNOWN;
    return false;
  }
  if (strcmp(text, "?") == 0) {
    command = CMD_STATUS_REQUEST;
    return true;
  }
  if (strcmp(text, "!") == 0) {
    command = CMD_STATUS_RESPONSE;
    return true;
  }
  if (strcmp(text, "ML") == 0) {
    command = CMD_MOTOR_LEFT;
    return true;
  }
  if (strcmp(text, "MR") == 0) {
    command = CMD_MOTOR_RIGHT;
    return true;
  }
  if (strcmp(text, "MB") == 0) {
    command = CMD_MOTOR_BOTH;
    return true;
  }
  if (strcmp(text, "TLR") == 0) {
    command = CMD_TAIL_ROTATE;
    return true;
  }
  if (strcmp(text, "TLS") == 0) {
    command = CMD_TAIL_SWING;
    return true;
  }
  if (strcmp(text, "TLSA") == 0) {
    command = CMD_TAIL_SWING_A;
    return true;
  }
  if (strcmp(text, "TLF") == 0) {
    command = CMD_TAIL_FREEZE;
    return true;
  }
  if (strcmp(text, "LED") == 0) {
    command = CMD_LED;
    return true;
  }
  if (strcmp(text, "?LED") == 0) {
    command = CMD_LED_REQUEST;
    return true;
  }
  if (strcmp(text, "!LED") == 0) {
    command = CMD_LED_RESPONSE;
    return true;
  }
  if (strcmp(text, "LEDA") == 0) {
    command = CMD_LEDA;
    return true;
  }
  if (strcmp(text, "?LEDA") == 0) {
    command = CMD_LEDA_REQUEST;
    return true;
  }
  if (strcmp(text, "!LEDA") == 0) {
    command = CMD_LEDA_RESPONSE;
    return true;
  }  
  if (strcmp(text, "?ENCL") == 0) {
    command = CMD_ENCL_REQUEST;
    return true;
  }
  if (strcmp(text, "?ENCR") == 0) {
    command = CMD_ENCR_REQUEST;
    return true;
  }
  if (strcmp(text, "?ENCB") == 0) {
    command = CMD_ENCB_REQUEST;
    return true;
  }
  if (strcmp(text, "!ENCL") == 0) {
    command = CMD_ENCL_RESPONSE;
    return true;
  }
  if (strcmp(text, "!ENCR") == 0) {
    command = CMD_ENCR_RESPONSE;
    return true;
  }
  if (strcmp(text, "?DIST") == 0) {
    command = CMD_DIST_REQUEST;
    return true;
  }
  if (strcmp(text, "!DIST") == 0) {
    command = CMD_DIST_RESPONSE;
    return true;
  }
  if (strcmp(text, "?SPD") == 0) {
    command = CMD_SPD_REQUEST;
    return true;
  }
  if (strcmp(text, "!SPD") == 0) {
    command = CMD_SPD_RESPONSE;
    return true;
  }
  if (strcmp(text, "?BV") == 0) {
    command = CMD_BV_REQUEST;
    return true;
  }
  if (strcmp(text, "!BV") == 0) {
    command = CMD_BV_RESPONSE;
    return true;
  }
  if (strcmp(text, "?DV") == 0) {
    command = CMD_DV_REQUEST;
    return true;
  }
  if (strcmp(text, "!DV") == 0) {
    command = CMD_DV_RESPONSE;
    return true;
  }
  if (strcmp(text, "?MCPS") == 0) {
    command = CMD_MCPS_REQUEST;
    return true;
  }
  if (strcmp(text, "!MCPS") == 0) {
    command = CMD_MCPS_RESPONSE;
    return true;
  }
  if (strcmp(text, "?BVF") == 0) {
    command = CMD_BVF_REQUEST;
    return true;
  }
  if (strcmp(text, "!BVF") == 0) {
    command = CMD_BVF_RESPONSE;
    return true;
  }  
  if (strcmp(text, "?DVF") == 0) {
    command = CMD_DVF_REQUEST;
    return true;
  }
  if (strcmp(text, "!DVF") == 0) {
    command = CMD_DVF_RESPONSE;
    return true;
  }
  command = CMD_UNKNOWN;
  return false;
}

bool Message::getParam(char *text, int &value) {
  int length = strlen(text);
  if (length == 0) {
    value = 0;
    return true;
  }
  bool isNegative = text[0] == '-';
  if (isNegative) text[0] = '0';
  int last = length - 1;
  value = 0;
  int digit;
  int factor = 1;
  for (int i = last; i >= 0; i--) {
    digit = charToInt(text[i]);
    if (digit < 0) {
      value = 0;
      return false;
    }
    if (i < last) factor *= 10;
    value += digit * factor;
  }
  if (isNegative) value = -value;
  return true;
}

