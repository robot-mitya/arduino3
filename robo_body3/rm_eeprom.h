#ifndef rm_eeprom_h
#define rm_eeprom_h

namespace robot_mitya {
  class Eeprom {
    private:
      static int readInt(int address, int defaultValue);
      static void writeInt(int address, int value);

      static long readLong(int address, long defaultValue);
      static void writeLong(int address, long value);
    public:
      static int readMicronsPerEncoderStep();
      static void writeMicronsPerEncoderStep(int microncPerStep);

      static long readBatteryVoltageFactor();
      static void writeBatteryVoltageFactor(long batteryVoltageFactor);

      static long readDcDcVoltageFactor();
      static void writeDcDcVoltageFactor(long dcDcVoltageFactor);
  };
}

#endif
