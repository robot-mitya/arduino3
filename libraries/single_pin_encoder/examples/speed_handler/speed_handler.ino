#include <single_pin_encoder.h>

#define ENCODER_PIN 2

void speedHandler(Speed speed) {
  Serial.print("Duration in micro seconds: ");
  Serial.print(speed.Duration);
  Serial.print('\t');
  Serial.print("Steps: ");
  Serial.print(speed.Steps);
  Serial.println();
}

SinglePinEncoder encoder(ENCODER_PIN);

void setup() {
  Serial.begin(9600);
  
  encoder.setSpeedHandler(speedHandler);
  
  // Call getSpeed function every 2 seconds:
  encoder.setSpeedHandlerPeriod(2000000);
  
  encoder.setDirection(POSITIVE);
}

void loop() {
  unsigned long currentMicros = micros();
  encoder.update(currentMicros);
}
