#ifndef rm_msg_h
#define rm_msg_h

namespace robot_mitya {
  enum Command {
      CMD_UNKNOWN         = 0x00,
      CMD_MOTOR_LEFT      = 0x01,
      CMD_MOTOR_RIGHT     = 0x02,
      CMD_MOTOR_BOTH      = 0x03,
      
      CMD_TAIL_ROTATE     = 0x08,
      CMD_TAIL_SWING      = 0x09,
      CMD_TAIL_SWING_A    = 0x0A,
      CMD_TAIL_FREEZE     = 0x0F,

      CMD_LED_1           = 0x10,
      CMD_LED_1_REQUEST   = 0x11,
      CMD_LED_1_RESPONSE  = 0x12,
      CMD_LED_2           = 0x13,
      CMD_LED_2_REQUEST   = 0x14,
      CMD_LED_2_RESPONSE  = 0x15,
      
      CMD_ENCL_REQUEST    = 0x40,
      CMD_ENCR_REQUEST    = 0x41,
      CMD_ENCB_REQUEST    = 0x42,
      CMD_ENCL_RESPONSE   = 0x43,
      CMD_ENCR_RESPONSE   = 0x44,
      CMD_DIST_REQUEST    = 0x45,
      CMD_DIST_RESPONSE   = 0x46,
      CMD_SPD_REQUEST     = 0x47,
      CMD_SPD_RESPONSE    = 0x48,
      CMD_MCPS_REQUEST    = 0x49,
      CMD_MCPS_RESPONSE   = 0x4A,
      
      CMD_BV_REQUEST      = 0x50,
      CMD_BV_RESPONSE     = 0x51,
      CMD_DV_REQUEST      = 0x52,
      CMD_DV_RESPONSE     = 0x53,      
      CMD_BVF_REQUEST     = 0x54,
      CMD_BVF_RESPONSE    = 0x55,
      CMD_DVF_REQUEST     = 0x56,
      CMD_DVF_RESPONSE    = 0x57,
      
      CMD_RESET           = 0xF0,
      CMD_STATUS_REQUEST  = 0xF1,
      CMD_STATUS_RESPONSE = 0xF2
  };
  
  class Message {
    public:
      static const int RET_OK = 0;
      static const int RET_BAD_PARAMETER = 1;
      static const int RET_WRONG_PARAMS_COUNT = 2;
      static const int RET_BAD_COMMAND = 3;
      static const int RET_NOISE_RECEIVED = 4;
      static const int RET_CS_ERROR = 5;
    
      static void initialize();
      static void processInput(void (*handler)(Command, int, int, int));
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
      static int checksum1(uint8_t* data, int size);
      static int checksum2(int cs1);
  };
}

#endif
