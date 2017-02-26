#include "rm_msg.h"
#include <HardwareSerial.h>

using namespace robot_mitya;

#define ROBO_SERIAL Serial
#define SERIAL_BAUD_RATE 9600

static String commandText = "";
static String param1Text = "";
static String param2Text = "";
static String param3Text = "";
static int wordCounter = 0;
static bool hasError = false;
static bool inWordSeparator = false;

void Message::initialize()
{
  ROBO_SERIAL.begin(SERIAL_BAUD_RATE);
}

void Message::send(String message)
{
  ROBO_SERIAL.print(message);
}

void Message::send(int status)
{
  ROBO_SERIAL.print("!");
  ROBO_SERIAL.print(WORD_SEPARATOR);
  ROBO_SERIAL.print(status);
  ROBO_SERIAL.print(COMMAND_SEPARATOR);
}

void Message::processInput(void (*handler)(String, int, int, int))
{
  char ch;
  int value1, value2, value3;
  while (ROBO_SERIAL.available() > 0)
  {
    ch = (char)ROBO_SERIAL.read();

    if (ch == COMMAND_SEPARATOR && handler != NULL)
    {
      if (getParam(param1Text, value1) && getParam(param2Text, value2) && getParam(param3Text, value3))
      {
        if (!hasError)
        {
          if (!commandText.equals("") || value1 != 0 || value2 != 0 || value3 != 0)
          {
            handler(commandText, value1, value2, value3);
          }
        }
      }
      else
      {
        send(RET_BAD_PARAMETER);
      }
      commandText = "";
      param1Text = "";
      param2Text = "";
      param3Text = "";
      wordCounter = 0;
      hasError = false;
      inWordSeparator = false;
      continue;
    }

    if (ch <= 32) //(white space)
    {
      if (!inWordSeparator)
      {
        inWordSeparator = true;
        wordCounter++;
      }
      continue;
    }

    inWordSeparator = false;
    switch (wordCounter)
    {
      case 0:
        commandText += ch;
        break;
      case 1:
        param1Text += ch;
        break;
      case 2:
        param2Text += ch;
        break;
      case 3:
        param3Text += ch;
        break;
      default:
        if (!hasError) send(RET_TOO_MANY_WORDS);
        hasError = true;
    }
  }
  
}

int Message::charToInt(char ch)
{
  switch (ch)
  {
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

bool Message::getParam(String text, int &value)
{
  int length = text.length();
  if (length == 0)
  {
    value = 0;
    return true;
  }
  bool isNegative = text.charAt(0) == '-';
  if (isNegative) text.setCharAt(0, '0');
  int last = length - 1;
  value = 0;
  int digit;
  int factor = 1;
  for (int i = last; i >= 0; i--)
  {
    digit = charToInt(text.charAt(i));
    if (digit < 0)
    {
      value = 0;
      return false;
    }
    if (i < last) factor *= 10;
    value += digit * factor;
  }
  if (isNegative) value = -value;
  return true;
}

