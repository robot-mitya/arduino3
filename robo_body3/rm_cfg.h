// -------------------------------------------------------------------------------------
// file = "rm_cfg.h" (Robot Mitya's configuration constants)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2017
// -------------------------------------------------------------------------------------

#ifndef rm_cfg_h
#define rm_cfg_h

#include <Arduino.h>
#include "rm_eeprom.h"

namespace robot_mitya {
  class Cfg {
    public:
      // Baud rate of the controller-PC serial link.
      static const int SERIAL_BAUD_RATE = 9600;
      // Headlights pins.
      static const int PIN_LED = 13;
      static const int PIN_LED_PWM = 11;
      // Left encoder pin.
      static const int PIN_LEFT_ENCODER = 2;
      // Right encoder pin.
      static const int PIN_RIGHT_ENCODER = 3;
      // Battery voltage pin.
      static const int PIN_BATTERY_VOLTAGE = A0;
      // DC-DC 5V voltage pin.
      static const int PIN_DC_DC_VOLTAGE = A1;
  };
}

#endif
