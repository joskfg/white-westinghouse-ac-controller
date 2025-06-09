import esphome.codegen as cg
from esphome.components import climate_ir

AUTO_LOAD = ["climate_ir"]

electrolux_climate_ns = cg.esphome_ns.namespace("electrolux_climate")

ElectroluxClimate = electrolux_climate_ns.class_(
    "ElectroluxClimate", climate_ir.ClimateIR
)

CONFIG_SCHEMA = climate_ir.climate_ir_schema(ElectroluxClimate)

async def to_code(config):
    await climate_ir.new_climate_ir(config)
