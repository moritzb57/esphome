import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, switch, sensor
from esphome.const import (
    CONF_ID, CONF_ADDRESS, CONF_CHANNELS, CONF_NAME, CONF_I2C_ID,
    CONF_PIN, CONF_DEVICE_CLASS, CONF_UNIT_OF_MEASUREMENT, CONF_ACCURACY_DECIMALS
)

DEPENDENCIES = ['i2c']
CODEOWNERS = ['@moritzb57']

seeed_ns = cg.esphome_ns.namespace('seeed')
SeeedDevice = seeed_ns.class_('SeeedDevice', cg.PollingComponent)
MultiChannelRelay = seeed_ns.class_('MultiChannelRelay', cg.Component, i2c.I2CDevice)
ChannelSwitch = seeed_ns.class_('ChannelSwitch', switch.Switch)
ACVoltageSensor = seeed_ns.class_('ACVoltageSensor', cg.PollingComponent, sensor.Sensor)

AUTO_LOAD = ["switch", "sensor"]

AC_VOLTAGE_SENSOR_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(ACVoltageSensor),
    cv.Required(CONF_PIN): cv.int_,
    cv.Optional("voltage_calibration", default=1.0): cv.float_,
    cv.Optional("phase_calibration", default=1.7): cv.float_,
}).extend(cv.polling_component_schema('1s'))

MULTI_CHANNEL_RELAY_SCHEMA = cv.Schema({
    # ... Ihr bestehendes Schema für das Relay ...
})

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SeeedDevice),
    cv.Optional("multi_channel_relay"): cv.ensure_list(MULTI_CHANNEL_RELAY_SCHEMA),
    cv.Optional("ac_voltage_sensor"): cv.ensure_list(AC_VOLTAGE_SENSOR_SCHEMA),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # ... Ihr bestehender Code für MultiChannelRelay ...

    if "ac_voltage_sensor" in config:
        for sensor_config in config["ac_voltage_sensor"]:
            sens = cg.new_Pvariable(sensor_config[CONF_ID])
            await cg.register_component(sens, sensor_config)
            await sensor.register_sensor(sens, sensor_config)

            cg.add(sens.set_pin(sensor_config[CONF_PIN]))
            cg.add(sens.set_voltage_calibration(sensor_config.get("voltage_calibration", 1.0)))
            cg.add(sens.set_phase_calibration(sensor_config.get("phase_calibration", 1.7)))

            cg.add(var.add_ac_voltage_sensor(sens))

DEPENDENCIES = ['i2c']

seeed_ns = cg.esphome_ns.namespace('seeed')
SeeedDevice = seeed_ns.class_('SeeedDevice', cg.PollingComponent)
MultiChannelRelay = seeed_ns.class_('MultiChannelRelay', cg.Component, i2c.I2CDevice)
ChannelSwitch = seeed_ns.class_('ChannelSwitch', switch.Switch)
ACVoltageSensor = seeed_ns.class_('ACVoltageSensor', cg.PollingComponent, sensor.Sensor)

AUTO_LOAD = ["switch", "sensor"]

AC_VOLTAGE_SENSOR_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(ACVoltageSensor),
    cv.Required(CONF_PIN): cv.int_,
    cv.Optional("voltage_calibration", default=1.0): cv.float_,
    cv.Optional("phase_calibration", default=1.7): cv.float_,
}).extend(cv.polling_component_schema('1s'))

MULTI_CHANNEL_RELAY_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MultiChannelRelay),
    cv.Optional(CONF_I2C_ID): cv.use_id(i2c.I2CBus),
    cv.Optional(CONF_ADDRESS, default=0x11): cv.i2c_address,
    cv.Required(CONF_CHANNELS): cv.one_of(4, 8, int),
    cv.Optional(CONF_NAME, default=[]): cv.ensure_list(cv.string),
}).extend(i2c.i2c_device_schema(0x11))

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SeeedDevice),
    cv.Optional("multi_channel_relay"): cv.ensure_list(MULTI_CHANNEL_RELAY_SCHEMA),
    cv.Optional("ac_voltage_sensor"): cv.ensure_list(AC_VOLTAGE_SENSOR_SCHEMA),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    if "multi_channel_relay" in config:
        for relay_config in config["multi_channel_relay"]:
            relay = cg.new_Pvariable(relay_config[CONF_ID])
            await cg.register_component(relay, relay_config)
            await i2c.register_i2c_device(relay, relay_config)
            cg.add(relay.set_channels(relay_config[CONF_CHANNELS]))

            names = relay_config.get(CONF_NAME, [])
            num_channels = relay_config[CONF_CHANNELS]
            for i in range(num_channels):
                channel_name = names[i] if i < len(names) else f"Relay Channel {i+1}"

                # Generieren einer eindeutigen ID für den Switch
                switch_id_name = f"{relay_config[CONF_ID].id}_channel_{i+1}"
                switch_id = cv.declare_id(ChannelSwitch)(switch_id_name)

                # Erstellen der Switch-Konfiguration
                switch_config = {
                    cv.GenerateID(): switch_id,
                    cv.Required(CONF_NAME): channel_name,
                }

                # Anwenden des Switch-Schemas
                switch_config = switch.switch_schema(ChannelSwitch)(switch_config)

                # Erstellen und Registrieren des Switches
                sw = await switch.new_switch(switch_config)
                cg.add(sw.set_parent(relay))
                cg.add(sw.set_channel(i + 1))
                cg.add(relay.add_channel_switch(sw))

            cg.add(var.add_multi_channel_relay(relay))
    if "ac_voltage_sensor" in config:
        for sensor_config in config["ac_voltage_sensor"]:
            sens = cg.new_Pvariable(sensor_config[CONF_ID])
            await cg.register_component(sens, sensor_config)
            await sensor.register_sensor(sens, sensor_config)

            cg.add(sens.set_pin(sensor_config[CONF_PIN]))
            cg.add(sens.set_voltage_calibration(sensor_config.get("voltage_calibration", 1.0)))
            cg.add(sens.set_phase_calibration(sensor_config.get("phase_calibration", 1.7)))

            cg.add(var.add_ac_voltage_sensor(sens))
