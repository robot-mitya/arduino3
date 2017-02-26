#ifndef rm_msg_h
#define rm_msg_h

#include <Arduino.h>

namespace robot_mitya
{
  class Message
  {
    public:
      static const int RET_OK = 0;
      static const int RET_BAD_PARAMETER = 1;
      static const int RET_TOO_MANY_WORDS = 2;
      static const int RET_BAD_COMMAND = 3;
    
      static void initialize();
      static void processInput(void (*handler)(String, int, int, int));
      static void send(String message);
      static void send(int status);
    private:
      static const char WORD_SEPARATOR = ' ';
      static const char COMMAND_SEPARATOR = ';';
      static int charToInt(char ch);
      static bool getParam(String text, int &value);
  };
}

#endif
