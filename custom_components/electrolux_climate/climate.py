import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import climate, sensor
from esphome.const import CONF_AWAY_CONFIG, CONF_COOL_ACTION, \
    CONF_DEFAULT_TARGET_TEMPERATURE_HIGH, CONF_DEFAULT_TARGET_TEMPERATURE_LOW, CONF_HEAT_ACTION, \
    CONF_ID, CONF_IDLE_ACTION, CONF_SENSOR

electrolux_climate_ns = cg.esphome_ns.namespace('electrolux_climate')

ElectroluxClimate = electrolux_climate_ns.class_('ElectroluxClimate', climate.Climate, cg.Component)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ElectroluxClimate),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield climate.register_climate(var, config)
