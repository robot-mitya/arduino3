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
      CMD_TAIL_ROTATE = 60,
      CMD_TAIL_SWING = 70,
      CMD_TAIL_SWING_A = 80,
      CMD_TAIL_FREEZE = 90,
      CMD_LED = 100,
      CMD_LED_REQUEST = 110,
      CMD_LED_RESPONSE = 120,
      CMD_LEDA = 130,
      CMD_LEDA_REQUEST = 140,
      CMD_LEDA_RESPONSE = 150,
      CMD_ENCL_REQUEST = 160,
      CMD_ENCR_REQUEST = 170,
      CMD_ENCB_REQUEST = 180,
      CMD_ENCL_RESPONSE = 190,
      CMD_ENCR_RESPONSE = 200,
      CMD_DIST_REQUEST = 210,
      CMD_DIST_RESPONSE = 220,
      CMD_SPD_REQUEST = 230,
      CMD_SPD_RESPONSE = 240,
      CMD_BV_REQUEST = 250,
      CMD_BV_RESPONSE = 260,
      CMD_DV_REQUEST = 270,
      CMD_DV_RESPONSE = 280,      
      CMD_MCPS_REQUEST = 290,
      CMD_MCPS_RESPONSE = 300,
      CMD_BVF_REQUEST = 310,
      CMD_BVF_RESPONSE = 320,
      CMD_DVF_REQUEST = 330,
      CMD_DVF_RESPONSE = 340
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
      static void sendLED(bool isTurnedOn);
      static void sendLEDA(int brightness);
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
