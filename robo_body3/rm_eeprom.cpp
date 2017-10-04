#include "rm_eeprom.h"
#include <EEPROM.h>

using namespace robot_mitya;

#define EEPROM_ADDRESS_MICRONS_PER_STEP 0
#define EEPROM_ADDRESS_BATTERY_VOLTAGE_FACTOR 2
#define EEPROM_ADDRESS_DC_DC_VOLTAGE_FACTOR 6

#define DEFAULT_MICRONS_PER_ENCODER_STEP 7857
#define DEFAULT_BATTERY_VOLTAGE_FACTOR 15329
#define DEFAULT_DC_DC_VOLTAGE_FACTOR 15329


int Eeprom::readInt(int address, int defaultValue) {
  int result = EEPROM.read(address);
  result <<= 8;
  result += EEPROM.read(address + 1);
  if (result == 0xFFFF) {
    Eeprom::writeInt(address, defaultValue);
    result = readInt(address, defaultValue);
  }
  return result;
}

void Eeprom::writeInt(int address, int value) {
  EEPROM.write(address, (unsigned char) (value >> 8));
  EEPROM.write(address + 1, (unsigned char) value);
}


long Eeprom::readLong(int address, long defaultValue) {
  long b0 = EEPROM.read(address);
  long b1 = EEPROM.read(address + 1);
  long b2 = EEPROM.read(address + 2);
  long b3 = EEPROM.read(address + 3);
  
  long result = ((b0 << 0) & 0xFF) + ((b1 << 8) & 0xFFFF) + ((b2 << 16) & 0xFFFFFF) + ((b3 << 24) & 0xFFFFFFFF);
  if (result == 0xFFFFFFFF) {
    Eeprom::writeLong(address, defaultValue);
    result = readLong(address, defaultValue);
  }
  return result;
}

void Eeprom::writeLong(int address, long value) {
  unsigned char b0 = (value & 0xFF);
  unsigned char b1 = ((value >> 8) & 0xFF);
  unsigned char b2 = ((value >> 16) & 0xFF);
  unsigned char b3 = ((value >> 24) & 0xFF);
  
  EEPROM.write(address, b0);
  EEPROM.write(address + 1, b1);
  EEPROM.write(address + 2, b2);
  EEPROM.write(address + 3, b3);
}


int Eeprom::readMicronsPerEncoderStep() {
  return Eeprom::readInt(EEPROM_ADDRESS_MICRONS_PER_STEP, DEFAULT_MICRONS_PER_ENCODER_STEP);
}

void Eeprom::writeMicronsPerEncoderStep(int microncPerStep) {
  Eeprom::writeInt(EEPROM_ADDRESS_MICRONS_PER_STEP, microncPerStep);
}

long Eeprom::readBatteryVoltageFactor() {
  return Eeprom::readLong(EEPROM_ADDRESS_BATTERY_VOLTAGE_FACTOR, DEFAULT_BATTERY_VOLTAGE_FACTOR);
}

void Eeprom::writeBatteryVoltageFactor(long batteryVoltageFactor) {
  Eeprom::writeLong(EEPROM_ADDRESS_BATTERY_VOLTAGE_FACTOR, batteryVoltageFactor);
}

long Eeprom::readDcDcVoltageFactor() {
  return Eeprom::readLong(EEPROM_ADDRESS_DC_DC_VOLTAGE_FACTOR, DEFAULT_DC_DC_VOLTAGE_FACTOR);
}

void Eeprom::writeDcDcVoltageFactor(long dcDcVoltageFactor) {
  Eeprom::writeLong(EEPROM_ADDRESS_DC_DC_VOLTAGE_FACTOR, dcDcVoltageFactor);
}

