#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/switch/switch.h"
//#include <vector>

namespace esphome {
  namespace seeed {

    class ChannelSwitch;

    class MultiChannelRelay : public Component, public i2c::I2CDevice {
     public:
      MultiChannelRelay();

      void setup() override;
      void loop() override;

      void change_i2c_address(uint8_t new_addr);
      uint8_t get_firmware_version();
      void control_channel(uint8_t state);
      void turn_on_channel(uint8_t channel);
      void turn_off_channel(uint8_t channel);
      uint8_t get_channel_state() const;

      void set_channels(uint8_t num_channels);
      void set_channel_name(uint8_t channel, const std::string &name);

      void add_channel_switch(ChannelSwitch *channel_switch);

     protected:
      uint8_t channel_state_;
      uint8_t num_channels_;
      std::string channel_names_[8];  // Maximale Anzahl der Kanäle
      std::vector<ChannelSwitch *> channel_switches_;
    };

    class ChannelSwitch : public switch_::Switch {
     public:
      ChannelSwitch() {}

      void write_state(bool state) override;

      void set_parent(MultiChannelRelay *parent) { this->parent_ = parent; }
      void set_channel(uint8_t channel) { this->channel_ = channel; }
      uint8_t get_channel() const { return this->channel_; }

     protected:
      MultiChannelRelay *parent_{nullptr};
      uint8_t channel_{0};
    };

  }  // namespace seeed
}  // namespace esphome
