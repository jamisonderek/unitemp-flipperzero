/*
    Unitemp - Universal temperature reader
    Copyright (C) 2022-2023  Victor Nikitchuk (https://github.com/quen0n)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <furi.h>
#include <furi_hal.h>
#include "ADCSensor.h"

bool unitemp_adc_sensor_alloc(Sensor* sensor, char* args) {
    if(args == NULL) {
        FURI_LOG_E(APP_NAME, "No args passed");
        return false;
    }

    ADCSensor* instance = malloc(sizeof(ADCSensor));
    if(instance == NULL) {
        FURI_LOG_E(APP_NAME, "Sensor %s instance allocation error", sensor->name);
        return false;
    }
    sensor->instance = instance;

    int gpio = 255;
    sscanf(args, "%d", &gpio);
    instance->data_pin = unitemp_gpio_getFromInt(gpio);
    if(instance->data_pin == NULL) {
        FURI_LOG_E(APP_NAME, "Sensor %s GPIO setting error. ARGS: %s", sensor->name, args);
        free(instance);
        return false;
    }

    instance->data_channel = FuriHalAdcChannelNone;
    if(instance->data_pin->num == 2) {
        instance->data_channel = FuriHalAdcChannel12;
    } else if(instance->data_pin->num == 3) {
        instance->data_channel = FuriHalAdcChannel11;
    } else if(instance->data_pin->num == 4) {
        instance->data_channel = FuriHalAdcChannel9;
    } else if(instance->data_pin->num == 7) {
        instance->data_channel = FuriHalAdcChannel4;
    } else if(instance->data_pin->num == 15) {
        instance->data_channel = FuriHalAdcChannel2;
    } else if(instance->data_pin->num == 16) {
        instance->data_channel = FuriHalAdcChannel1;
    }

    if(instance->data_channel == FuriHalAdcChannelNone) {
        FURI_LOG_E(APP_NAME, "Sensor %s GPIO.ch setting error", sensor->name);
        free(instance);
        return false;
    }

    instance->handle = furi_hal_adc_acquire();
    furi_hal_adc_configure_ex(
        instance->handle,
        FuriHalAdcScale2048,
        FuriHalAdcClockSync64,
        FuriHalAdcOversample64,
        FuriHalAdcSamplingtime247_5);

    bool status = sensor->type->allocator(sensor, args);
    FURI_LOG_I(APP_NAME, "Status %d from %s", status, sensor->name);
    if(instance->data_pin) {
        FURI_LOG_D(APP_NAME, "Data pin %s", instance->data_pin->name);
    }

    unitemp_gpio_lock(instance->data_pin, &ADC);

    FURI_LOG_D(APP_NAME, "Sensor %s allocated", sensor->name);
    return status;
}

bool unitemp_adc_sensor_free(Sensor* sensor) {
    bool status = sensor->type->mem_releaser(sensor);
    ADCSensor* adc_sensor = (ADCSensor*)sensor->instance;
    unitemp_gpio_unlock(adc_sensor->data_pin);
    if(adc_sensor->handle) {
        furi_hal_adc_release(adc_sensor->handle);
        adc_sensor->handle = NULL;
    }
    free(adc_sensor);

    FURI_LOG_D(APP_NAME, "Sensor %s freed", sensor->name);
    return status;
}

bool unitemp_adc_sensor_init(Sensor* sensor) {
    FURI_LOG_D(APP_NAME, "Sensor %s init", sensor->name);
    ADCSensor* adc_sensor = (ADCSensor*)sensor->instance;
    if(adc_sensor && adc_sensor->data_pin) {
        furi_hal_gpio_init(
            adc_sensor->data_pin->pin, GpioModeAnalog, GpioPullNo, GpioSpeedVeryHigh);
    }
    return sensor->type->initializer(sensor);
}

bool unitemp_adc_sensor_deinit(Sensor* sensor) {
    FURI_LOG_D(APP_NAME, "Sensor %s deinit", sensor->name);
    UNUSED(sensor);

    return true;
}

UnitempStatus unitemp_adc_sensor_update(Sensor* sensor) {
    FURI_LOG_D(APP_NAME, "Sensor %s update", sensor->name);
    ADCSensor* adc_sensor = (ADCSensor*)sensor->instance;
    if(adc_sensor && adc_sensor->handle) {
        uint16_t adc_value = furi_hal_adc_read(adc_sensor->handle, adc_sensor->data_channel);
        adc_sensor->voltage = furi_hal_adc_convert_to_voltage(adc_sensor->handle, adc_value);
    }

    return sensor->type->updater(sensor);
}