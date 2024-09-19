#include "seeed.h"
#include "multi_channel_relay.h"
#include "esphome/core/log.h"

namespace esphome {
  namespace seeed {

    static const char *TAG = "seeed.device";

    void SeeedDevice::setup() {
      ESP_LOGCONFIG(TAG, "Setting up Seeed Device with multiple relays...");
      for (auto *relay : this->relays_) {
        relay->setup();
      }
    }

    void SeeedDevice::update() {
      for (auto *relay : this->relays_) {
        relay->control_channel(relay->get_channel_state());
      }
    }

    void SeeedDevice::add_multi_channel_relay(MultiChannelRelay *relay) {
      this->relays_.push_back(relay);
    }

  }  // namespace seeed
}  // namespace esphome
