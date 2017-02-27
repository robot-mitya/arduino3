#ifndef rm_msg_h
#define rm_msg_h

namespace robot_mitya
{
  enum Command {
      CMD_UNKNOWN = 0,
      CMD_STATUS_REQUEST = 10,
      CMD_STATUS_RESPONSE = 20,
      CMD_MOTOR_LEFT = 30,
      CMD_MOTOR_RIGHT = 40,
      CMD_MOTOR_BOTH = 50
  };
  
  class Message
  {
    public:
      static const int RET_OK = 0;
      static const int RET_BAD_PARAMETER = 1;
      static const int RET_TOO_MANY_WORDS = 2;
      static const int RET_BAD_COMMAND = 3;
    
      static void initialize();
      static void processInput(void (*handler)(Command, int, int, int));
      static void send(char* message);
      static void send(int status);
    private:
      static const char WORD_SEPARATOR = ' ';
      static const char COMMAND_SEPARATOR = ';';
      static int charToInt(char ch);
      static char* getCommandText(Command command);
      static bool getCommand(char *text, Command &command);
      static bool getParam(char *text, int &value);
  };
}

#endif
