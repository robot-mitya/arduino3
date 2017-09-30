#include <single_pin_encoder.h>

#define ENCODER_PIN 2

SinglePinEncoder encoder(ENCODER_PIN);

unsigned long previousMicros;

void setup() {
  Serial.begin(9600);

  encoder.setDirection(POSITIVE);
}

void loop() {
  long currentMicros = micros();
  encoder.update(currentMicros);

  // Call getSpeed function every 2 seconds:
  if (currentMicros - previousMicros > 2000000) {
    previousMicros = currentMicros;

    Speed speed = encoder.getSpeed(currentMicros);

    Serial.print("Duration in micro seconds: ");
    Serial.print(speed.Duration);
    Serial.print('\t');
    Serial.print("Steps: ");
    Serial.print(speed.Steps);
    Serial.println();
  }
}
