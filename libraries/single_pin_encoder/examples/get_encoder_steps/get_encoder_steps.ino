#include "single_pin_encoder.h"

#define ENCODER_PIN 2

SinglePinEncoder encoder(ENCODER_PIN);

unsigned long previousMicros;

void setup() {
  Serial.begin(9600);
  
  // Set POSITIVE or NEGATIVE direction:
  encoder.setDirection(POSITIVE);
  //encoder.setDirection(NEGATIVE);
}

void loop() {
  long currentMicros = micros();
  encoder.update(currentMicros);
  
  if (currentMicros - previousMicros > 2000000) {
    previousMicros = currentMicros;

    Serial.print("Positive steps: ");
    Serial.print(encoder.getPositiveSteps());
    Serial.print('\t');
    Serial.print("Negative steps: ");
    Serial.print(encoder.getNegativeSteps());
    Serial.print('\t');
    Serial.print("Result steps: ");
    Serial.print(encoder.getSteps());
    Serial.println();
  }
}

