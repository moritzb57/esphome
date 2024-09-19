#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace seeed {

class ACVoltageSensor : public PollingComponent, public sensor::Sensor {
 public:
  void set_pin(uint8_t pin) { analog_pin_ = pin; }
  void set_voltage_calibration(double volt_cal) { volt_cal_ = volt_cal; }
  void set_phase_calibration(double phase_cal) { phase_cal_ = phase_cal; }

  void setup() override;
  void update() override;

 protected:
  uint8_t analog_pin_;
  double volt_cal_ = 1.0;
  double phase_cal_ = 1.7;

  // Interne Variablen für die Berechnung
  double offset_v_ = 0;
  double filtered_v_ = 0;
  double last_filtered_v_ = 0;
  double sum_v_ = 0;
  uint32_t sample_count_ = 0;
};

}  // namespace seeed
}  // namespace esphome
