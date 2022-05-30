#include "esphome/core/log.h"
#include "electrolux_climate.h"

namespace esphome {
namespace electrolux_climate {

uint8_t ElectroluxClimate::operation_mode_() {
  uint8_t operating_mode = 0;
  switch (this->mode) {
    case climate::CLIMATE_MODE_COOL:
      return ELECTROLUX_MODE_COOL;
      break;
    case climate::CLIMATE_MODE_HEAT:
      return ELECTROLUX_MODE_HEAT;
      break;
    case climate::CLIMATE_MODE_HEAT_COOL:
      return ELECTROLUX_MODE_AUTO;
      break;
    case climate::CLIMATE_MODE_DRY:
      return ELECTROLUX_MODE_DRY;
      break;
    case climate::CLIMATE_MODE_FAN_ONLY:
    case climate::CLIMATE_MODE_OFF:
    default:
      return ELECTROLUX_MODE_FAN;
      break;
  }
}

uint8_t ElectroluxClimate::fan_speed_() {
  switch (this->fan_mode.value()) {
    case climate::CLIMATE_FAN_LOW:
      return ELECTROLUX_FAN_LOW;
      break;
    case climate::CLIMATE_FAN_MEDIUM:
      return ELECTROLUX_FAN_MEDIUM;
      break;
    case climate::CLIMATE_FAN_HIGH:
      return ELECTROLUX_FAN_HIGH;
      break;
    case climate::CLIMATE_FAN_AUTO:
    default:
      return ELECTROLUX_FAN_AUTO;
  }
}

uint8_t ElectroluxClimate::temperature_() {
  // set temparature from 1 to 15 instead of 16 to 30.
  uint8_t temp_offset = 15;
  return (uint8_t) roundf(this->target_temperature) - temp_offset;
}

void ElectroluxClimate::control(const climate::ClimateCall &call) {
  this->was_on_ = this->mode != climate::CLIMATE_MODE_OFF;
  climate_ir::ClimateIR::control(call);
}

void ElectroluxClimate::transmit_state() {
  auto transmit = this->transmitter_->transmit();

  transmit.set_send_times(2);

  auto *data = transmit.get_data();

  // Header
  data->mark(ELECTROLUX_BIT_SIZE * 3);
  data->space(ELECTROLUX_BIT_SIZE * 3);

  // Turn on/off
  if((this->was_on_ && this->mode == climate::CLIMATE_MODE_OFF) || (!this->was_on_ && this->mode != climate::CLIMATE_MODE_OFF)) {
    data->space(ELECTROLUX_BIT_SIZE);
    data->mark(ELECTROLUX_BIT_SIZE);
  } else { // No turn/on/off
    data->mark(ELECTROLUX_BIT_SIZE);
    data->space(ELECTROLUX_BIT_SIZE);
  }

  // Operation Mode
  uint8_t operating_mode = this->operation_mode_();
  ESP_LOGD("ElectroluxClimate", "Operating mode: %d", operating_mode);
  // loop through the bits of the operating mode
  for (uint8_t i = 3; i > 0; i--) {
    uint8_t mask = 1 << (i-1);
    if (operating_mode & mask) {
      ESP_LOGD("ElectroluxClimate", "Operating mode bits: %d -> 1", i-1);
      data->space(ELECTROLUX_BIT_SIZE);
      data->mark(ELECTROLUX_BIT_SIZE);
    } else {
      ESP_LOGD("ElectroluxClimate", "Operating mode bits: %d -> 0", i-1);
      data->mark(ELECTROLUX_BIT_SIZE);
      data->space(ELECTROLUX_BIT_SIZE);
    }
  }

  // Fan Speed
  uint8_t fan_speed = this->fan_speed_();
  // loop through the bits of the fan speed
  ESP_LOGD("ElectroluxClimate", "Fan speed: %d", fan_speed);
  for (uint8_t i = 3; i > 0; i--) {
    uint8_t mask = 1 << (i-1);
    if (fan_speed & mask) {
      ESP_LOGD("ElectroluxClimate", "Fan speed bits: %d -> 1", i-1);
      data->space(ELECTROLUX_BIT_SIZE);
      data->mark(ELECTROLUX_BIT_SIZE);
    } else {
      ESP_LOGD("ElectroluxClimate", "Fan speed bits: %d -> 0", i-1);
      data->mark(ELECTROLUX_BIT_SIZE);
      data->space(ELECTROLUX_BIT_SIZE);
    }
  }

  // Empty data
  data->mark(ELECTROLUX_BIT_SIZE);
  data->space(ELECTROLUX_BIT_SIZE);
  data->mark(ELECTROLUX_BIT_SIZE);
  data->space(ELECTROLUX_BIT_SIZE);

  // I feel (I always don't feel)
  data->mark(ELECTROLUX_BIT_SIZE);
  data->space(ELECTROLUX_BIT_SIZE);

  // Temperature
  uint8_t temperature = this->temperature_();
  ESP_LOGD("ElectroluxClimate", "Temperature: %d", this->target_temperature);
  ESP_LOGD("ElectroluxClimate", "Adapted Temperature: %d", temperature);
  // loop through the bits of the temperature
  for (uint8_t i = 4; i > 0; i--) {
    uint8_t mask = 1 << (i-1);
    if (temperature & mask) {
      ESP_LOGD("ElectroluxClimate", "Temperature bits: %d -> 1", i-1);
      data->space(ELECTROLUX_BIT_SIZE);
      data->mark(ELECTROLUX_BIT_SIZE);
    } else {
      ESP_LOGD("ElectroluxClimate", "Temperature bits: %d -> 0", i-1);
      data->mark(ELECTROLUX_BIT_SIZE);
      data->space(ELECTROLUX_BIT_SIZE);
    }
  }

  for(uint8_t i = 0; i < 18; i++) {
    data->mark(ELECTROLUX_BIT_SIZE);
    data->space(ELECTROLUX_BIT_SIZE);
  }

  // Triplicate data vector contatenating initial information
  std::vector<int32_t> vector = data->get_data();
  std::vector<int32_t> new_vector;
  for(uint8_t i = 0; i < 3; i++) {
    for(uint8_t j = 0; j < vector.size(); j++) {
      new_vector.push_back(vector[j]);
    }
  }

  data->set_data(new_vector);
  data->mark(ELECTROLUX_BIT_SIZE * 4);
  data->space(ELECTROLUX_BIT_SIZE * 30);

  transmit.perform();
}

} //namespace electrolux_climate
} //namespace esphome