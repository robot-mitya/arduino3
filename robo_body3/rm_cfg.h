// -------------------------------------------------------------------------------------
// file = "rm_cfg.h" (Robot Mitya's configuration constants)
// company = "Robot Mitya community"
// Copyright © Dmitry Dzakhov 2017
// -------------------------------------------------------------------------------------

#ifndef rm_cfg_h
#define rm_cfg_h

namespace robot_mitya
{
  class Cfg
  {
    public:
      // Baud rate of the controller-PC serial link.
      static const int SERIAL_BAUD_RATE = 9600;
      // Headlights pin.
      static const int PIN_HEADLIGHTS = 13;
  };
}

#endif
