#include "ac_voltage_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace seeed {

static const char *TAG = "seeed.ac_voltage_sensor";

void ACVoltageSensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up AC Voltage Sensor on pin %d", this->analog_pin_);
  // Initialer Offset-Wert (ADC_Mitte)
  this->offset_v_ = 2048.0;  // Für einen 12-Bit-ADC (ESP32)
}

void ACVoltageSensor::update() {
  // Anzahl der Samples für eine Messung
  const uint16_t num_samples = 1000;

  this->sum_v_ = 0;
  this->sample_count_ = 0;

  for (uint16_t i = 0; i < num_samples; i++) {
    // A) Lesen des Rohwerts
    int16_t sample_v = analogRead(this->analog_pin_);

    // B) DC-Offset entfernen
    this->offset_v_ = this->offset_v_ + ((sample_v - this->offset_v_) / 1024.0);
    this->filtered_v_ = sample_v - this->offset_v_;

    // C) Quadratischer Wert
    double sq_v = this->filtered_v_ * this->filtered_v_;
    this->sum_v_ += sq_v;

    this->sample_count_++;
    delayMicroseconds(40);  // Wartezeit zwischen den Samples
  }

  // D) Effektivwert berechnen
  double vrms = this->volt_cal_ * sqrt(this->sum_v_ / this->sample_count_);

  // Wert veröffentlichen
  this->publish_state(vrms);
}

}  // namespace seeed
}  // namespace esphome
