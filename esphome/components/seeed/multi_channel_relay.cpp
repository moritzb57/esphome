#include "multi_channel_relay.h"
#include "esphome/core/log.h"

namespace esphome {
  namespace seeed {

    static const char *TAG = "multi_channel_relay";
    static const uint8_t CMD_CHANNEL_CTRL = 0x10;
    static const uint8_t CMD_SAVE_I2C_ADDR = 0x11;
    static const uint8_t CMD_READ_FIRMWARE_VER = 0x13;

    MultiChannelRelay::MultiChannelRelay()
        : i2c::I2CDevice(), channel_state_(0), num_channels_(0) {}

    void MultiChannelRelay::setup() {
      ESP_LOGCONFIG(TAG, "Setting up Multi-Channel Relay with address: 0x%02X", this->address_);
      for (auto *channel_switch : this->channel_switches_) {
        uint8_t channel = channel_switch->get_channel();
        bool state = (this->channel_state_ >> (channel - 1)) & 0x01;
        channel_switch->publish_state(state);
      }
    }

    void MultiChannelRelay::loop() {
      // Falls nötig, Status der Relais lesen und Switches aktualisieren
    }

    void MultiChannelRelay::change_i2c_address(uint8_t new_addr) {
      // Implementierung (falls benötigt)
    }

    uint8_t MultiChannelRelay::get_firmware_version() {
      // Implementierung (falls benötigt)
      return 0;
    }

    void MultiChannelRelay::control_channel(uint8_t state) {
      this->channel_state_ = state;
      // Senden des I2C-Kommandos zur Steuerung der Kanäle
      bool success = this->write_byte(CMD_CHANNEL_CTRL, state);
      if (!success) {
        ESP_LOGE(TAG, "Failed to write to relay at I2C address 0x%02X", this->address_);
      }
    }

    void MultiChannelRelay::turn_on_channel(uint8_t channel) {
      ESP_LOGD(TAG, "Turning on channel %u", channel);
      this->channel_state_ |= (1 << (channel - 1));
      this->control_channel(this->channel_state_);
    }

    void MultiChannelRelay::turn_off_channel(uint8_t channel) {
      ESP_LOGD(TAG, "Turning off channel %u", channel);
      this->channel_state_ &= ~(1 << (channel - 1));
      this->control_channel(this->channel_state_);
    }

    uint8_t MultiChannelRelay::get_channel_state() const {
      return this->channel_state_;
    }

    void MultiChannelRelay::set_channels(uint8_t num_channels) {
      if (num_channels > 0 && num_channels <= 8) {
        this->num_channels_ = num_channels;
        ESP_LOGCONFIG(TAG, "Number of channels set to %u", this->num_channels_);
      } else {
        ESP_LOGW(TAG, "Invalid number of channels: %u", num_channels);
      }
    }

    void MultiChannelRelay::set_channel_name(uint8_t channel, const std::string &name) {
      if (channel > 0 && channel <= this->num_channels_) {
        this->channel_names_[channel - 1] = name;
        ESP_LOGCONFIG(TAG, "Set channel %u name to: %s", channel, name.c_str());
      } else {
        ESP_LOGW(TAG, "Invalid channel number: %u", channel);
      }
    }

    void MultiChannelRelay::add_channel_switch(ChannelSwitch *channel_switch) {
      this->channel_switches_.push_back(channel_switch);
    }

    // ChannelSwitch Implementierung

    void ChannelSwitch::write_state(bool state) {
      if (state) {
        parent_->turn_on_channel(channel_);
      } else {
        parent_->turn_off_channel(channel_);
      }
      this->publish_state(state);
    }

  }  // namespace seeed
}  // namespace esphome
