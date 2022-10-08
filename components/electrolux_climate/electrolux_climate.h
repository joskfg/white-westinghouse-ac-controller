#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

namespace esphome {
namespace electrolux_climate {

// Temperature
const uint8_t ELECTROLUX_TEMP_MIN = 16.0f; // Celsius
const uint8_t ELECTROLUX_TEMP_MAX = 30.0f; // Celsius
const uint8_t ELECTROLUX_TEMP_STEP = 1.0f; // Celsius

// Modes
const uint8_t ELECTROLUX_MODE_COOL = 0b001;
const uint8_t ELECTROLUX_MODE_HEAT = 0b010;
const uint8_t ELECTROLUX_MODE_AUTO = 0b011;
const uint8_t ELECTROLUX_MODE_DRY = 0b100;
const uint8_t ELECTROLUX_MODE_FAN = 0b101;

// Fan Speed
const uint8_t ELECTROLUX_FAN_LOW    = 0b000;
const uint8_t ELECTROLUX_FAN_MEDIUM = 0b010;
const uint8_t ELECTROLUX_FAN_HIGH   = 0b100;
const uint8_t ELECTROLUX_FAN_AUTO   = 0b110;

// Serial Transmission
const uint32_t ELECTROLUX_BIT_SIZE = 976;

class ElectroluxClimate : public climate_ir::ClimateIR {
  public:
    ElectroluxClimate()
        : climate_ir::ClimateIR(ELECTROLUX_TEMP_MIN, ELECTROLUX_TEMP_MAX, ELECTROLUX_TEMP_STEP, true, true,
                                {climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH, climate::CLIMATE_FAN_AUTO}) {}
    void control(const climate::ClimateCall &call);

  protected:
    bool was_on_ = false;
    void transmit_state() override;
    uint8_t operation_mode_();
    uint8_t fan_speed_();
    uint8_t temperature_();
};

} //namespace electrolux_climate
} //namespace esphome