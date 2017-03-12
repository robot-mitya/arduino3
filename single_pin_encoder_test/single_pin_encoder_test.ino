#include "single_pin_encoder.h"

SinglePinEncoder encoderLeft(2);
//SinglePinEncoder encoderRight(3);

unsigned long previousMicros;

void setup() {
  Serial.begin(9600);
}

void loop() {
  long currentMicros = micros();

  encoderLeft.update(currentMicros);
  
  if (currentMicros - previousMicros > 2000000) {
    previousMicros = currentMicros;

    unsigned long duration;
    long steps;
    encoderLeft.GetSpeed(duration, steps);

    Serial.print("Steps: ");
    Serial.print(encoderLeft.GetPositiveSteps());
    Serial.print('\t');
    Serial.print(encoderLeft.GetNegativeSteps());
    Serial.print('\t');
    Serial.print(encoderLeft.GetSteps());
    Serial.println();

//    Serial.print("Duration: ");
//    Serial.print(duration);
//    Serial.print(" uS");
//    Serial.print('\t');
//    Serial.print("Steps: ");
//    Serial.print(steps);
//    Serial.println();
  }
}
