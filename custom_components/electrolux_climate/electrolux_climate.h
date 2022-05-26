#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"

namespace esphome {
namespace electrolux_climate {

class Communication {
    public:
        void setup();
        void send(climate::ClimateMode mode, float temperature, optional<climate::ClimateFanMode> fan_mode);
};

class ElectroluxClimate : public climate::Climate, public Component {
  public:
    void setup() override;
  protected: 
    Communication *communication_;
    void control(const climate::ClimateCall &call) override;
    climate::ClimateTraits traits() override;
};

} //namespace electrolux_climate
} //namespace esphome