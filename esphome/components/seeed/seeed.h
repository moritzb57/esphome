#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "ac_voltage_sensor.h"
#include <vector>

namespace esphome
{
  namespace seeed
  {

    class MultiChannelRelay;

    class SeeedDevice : public PollingComponent
    {
    public:
      SeeedDevice() : PollingComponent(60000) {} // Polling alle 60 Sekunden

      void setup() override;
      void update() override;
      void add_multi_channel_relay(MultiChannelRelay *relay);

      void add_ac_voltage_sensor(ACVoltageSensor *sensor)
      {
        // Hier können Sie weitere Initialisierungen vornehmen, falls nötig
      }

    protected:
      std::vector<MultiChannelRelay *> relays_;
    };

  } // namespace seeed
} // namespace esphome
