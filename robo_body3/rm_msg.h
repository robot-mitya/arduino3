#ifndef rm_msg_h
#define rm_msg_h

namespace robot_mitya {
  enum Command {
      CMD_UNKNOWN = 0,
      CMD_STATUS_REQUEST = 10,
      CMD_STATUS_RESPONSE = 20,
      CMD_RESET = 30,
      CMD_MOTOR_LEFT = 40,
      CMD_MOTOR_RIGHT = 50,
      CMD_MOTOR_BOTH = 60,
      CMD_TAIL_ROTATE = 70,
      CMD_TAIL_SWING = 80,
      CMD_TAIL_SWING_A = 90,
      CMD_TAIL_FREEZE = 100,
      CMD_LED_1 = 110,
      CMD_LED_1_REQUEST = 120,
      CMD_LED_1_RESPONSE = 130,
      CMD_LED_2 = 140,
      CMD_LED_2_REQUEST = 150,
      CMD_LED_2_RESPONSE = 160,
      CMD_ENCL_REQUEST = 170,
      CMD_ENCR_REQUEST = 180,
      CMD_ENCB_REQUEST = 190,
      CMD_ENCL_RESPONSE = 200,
      CMD_ENCR_RESPONSE = 210,
      CMD_DIST_REQUEST = 220,
      CMD_DIST_RESPONSE = 230,
      CMD_SPD_REQUEST = 240,
      CMD_SPD_RESPONSE = 250,
      CMD_BV_REQUEST = 260,
      CMD_BV_RESPONSE = 270,
      CMD_DV_REQUEST = 280,
      CMD_DV_RESPONSE = 290,      
      CMD_MCPS_REQUEST = 300,
      CMD_MCPS_RESPONSE = 310,
      CMD_BVF_REQUEST = 320,
      CMD_BVF_RESPONSE = 330,
      CMD_DVF_REQUEST = 340,
      CMD_DVF_RESPONSE = 350
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
      static void sendLed1(bool isTurnedOn);
      static void sendLed2(bool isTurnedOn);
      static void sendENCL(long steps);
      static void sendENCR(long steps);
      static void sendDistance(long distanceInMicrons);
      static void sendSpeed(int speedInMetersPerHour);
      static void sendBatteryVoltage(int batteryVoltage);
      static void sendDcDcVoltage(int dcDcVoltage);
      static void sendMicronsPerStep(int micronsPerStep);
      static void sendBatteryVoltageFactor(long batteryVoltageFactor);
      static void sendDcDcVoltageFactor(long dcDcVoltageFactor);
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
