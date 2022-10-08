import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate_ir
from esphome.const import CONF_ID

AUTO_LOAD = ["climate_ir"]

electrolux_climate_ns = cg.esphome_ns.namespace('electrolux_climate')

ElectroluxClimate = electrolux_climate_ns.class_('ElectroluxClimate', climate_ir.ClimateIR, cg.Component)

CONFIG_SCHEMA = climate_ir.CLIMATE_IR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ElectroluxClimate),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield climate_ir.register_climate_ir(var, config)
