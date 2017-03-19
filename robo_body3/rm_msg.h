#ifndef rm_msg_h
#define rm_msg_h

namespace robot_mitya {
  enum Command {
      CMD_UNKNOWN = 0,
      CMD_STATUS_REQUEST = 10,
      CMD_STATUS_RESPONSE = 20,
      CMD_MOTOR_LEFT = 30,
      CMD_MOTOR_RIGHT = 40,
      CMD_MOTOR_BOTH = 50,
      CMD_LED = 60,
      CMD_ENCL_REQUEST = 70,
      CMD_ENCR_REQUEST = 80,
      CMD_ENCB_REQUEST = 90,
      CMD_ENCL_RESPONSE = 100,
      CMD_ENCR_RESPONSE = 110,
      CMD_DIST_REQUEST = 120,
      CMD_DIST_RESPONSE = 130,
      CMD_SPD_REQUEST = 140,
      CMD_SPD_RESPONSE = 150,
      CMD_MCPS_REQUEST = 160,
      CMD_MCPS_RESPONSE = 170
  };
  
  class Message {
    public:
      static const int RET_OK = 0;
      static const int RET_BAD_PARAMETER = 1;
      static const int RET_TOO_MANY_WORDS = 2;
      static const int RET_BAD_COMMAND = 3;
    
      static void initialize();
      static void processInput(void (*handler)(Command, int, int, int));
      static void send(char* message);
      static void send(int status);
      static void sendENCL(long steps);
      static void sendENCR(long steps);
      static void sendDistance(long distanceInMicrons);
      static void sendSpeed(int speedInMetersPerHour);
      static void sendMicronsPerStep(int micronsPerStep);
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
