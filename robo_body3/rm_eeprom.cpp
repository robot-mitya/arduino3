#include "rm_eeprom.h"
#include <EEPROM.h>

using namespace robot_mitya;

#define DEFAULT_MICRONS_PER_ENCODER_STEP 7857

#define EEPROM_ADDRESS_MICRONS_PER_STEP_HI 0
#define EEPROM_ADDRESS_MICRONS_PER_STEP_LO 1

int Eeprom::readMicronsPerEncoderStep() {
  int micronsPerStep = EEPROM.read(EEPROM_ADDRESS_MICRONS_PER_STEP_HI);
  micronsPerStep <<= 8;
  micronsPerStep += EEPROM.read(EEPROM_ADDRESS_MICRONS_PER_STEP_LO);
  if (micronsPerStep == 0xFFFF) {
    Eeprom::writeMicronsPerEncoderStep(DEFAULT_MICRONS_PER_ENCODER_STEP);
    micronsPerStep = readMicronsPerEncoderStep();
  }
  return micronsPerStep;
}

void Eeprom::writeMicronsPerEncoderStep(int microncPerStep) {
  EEPROM.write(EEPROM_ADDRESS_MICRONS_PER_STEP_HI, (unsigned char) (microncPerStep >> 8));
  EEPROM.write(EEPROM_ADDRESS_MICRONS_PER_STEP_LO, (unsigned char) microncPerStep);
}

