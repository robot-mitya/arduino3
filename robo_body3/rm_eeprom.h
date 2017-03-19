#ifndef rm_eeprom_h
#define rm_eeprom_h

namespace robot_mitya {
  class Eeprom {
    public:
      static int readMicronsPerEncoderStep();
      static void writeMicronsPerEncoderStep(int microncPerStep);
  };
}

#endif
