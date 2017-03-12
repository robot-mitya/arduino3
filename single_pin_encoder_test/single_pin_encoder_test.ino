#include "single_pin_encoder.h"

void speedHandler(Speed speed) {
  Serial.print("Duration: ");
  Serial.print(speed.Duration);
  Serial.print(" uS");
  Serial.print('\t');
  Serial.print("Steps: ");
  Serial.print(speed.Steps);
  Serial.println();
}

SinglePinEncoder encoderLeft(2);
//SinglePinEncoder encoderRight(3);

unsigned long previousMicros;
/*
void setup() {
  Serial.begin(9600);
  //encoderLeft.setDirection(NEGATIVE);
}

void loop() {
  long currentMicros = micros();

  encoderLeft.update(currentMicros);
  
  if (currentMicros - previousMicros > 2000000) {
    previousMicros = currentMicros;

    Speed speed = encoderLeft.getSpeed(currentMicros);

//    Serial.print("Steps: ");
//    Serial.print(encoderLeft.getPositiveSteps());
//    Serial.print('\t');
//    Serial.print(encoderLeft.getNegativeSteps());
//    Serial.print('\t');
//    Serial.print(encoderLeft.getSteps());
//    Serial.println();

    Serial.print("Duration: ");
    Serial.print(speed.Duration);
    Serial.print(" uS");
    Serial.print('\t');
    Serial.print("Steps: ");
    Serial.print(speed.Steps);
    Serial.println();
  }
}
*/
void setup() {
  Serial.begin(9600);
  encoderLeft.setSpeedHandler(speedHandler);
  encoderLeft.setSpeedHandlerPeriod(3000000);
  //encoderLeft.setDirection(NEGATIVE);
}

void loop() {
  long currentMicros = micros();

  encoderLeft.update(currentMicros);
}

