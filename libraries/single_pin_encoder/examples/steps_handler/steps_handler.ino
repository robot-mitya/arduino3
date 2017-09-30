#include <single_pin_encoder.h>

#define ENCODER_PIN 2

void stepsHandler(long steps) {
  Serial.print("Steps: ");
  Serial.print(steps);
  Serial.println();
}

SinglePinEncoder encoder(ENCODER_PIN);

void setup() {
  Serial.begin(9600);
  
  encoder.setStepsHandler(stepsHandler);
  
  // Call getSpeed function every 2 seconds:
  encoder.setStepsHandlerPeriod(2000000);
  
  encoder.setDirection(POSITIVE);
}

void loop() {
  unsigned long currentMicros = micros();
  encoder.update(currentMicros);
}
