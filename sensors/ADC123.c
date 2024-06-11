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
#include "ADC123.h"
#include "../interfaces/ADCSensor.h"

// ADC123 is a made up example sensor that reads ADC values from pin A7.
const SensorType ADC123 = {
    .typename = "ADC123",
    .altname = "ADC123 (ADC)",
    .interface = &ADC,
    .datatype = UT_TEMPERATURE,
    .pollingInterval = 2000,
    .allocator = unitemp_ADC123_alloc,
    .mem_releaser = unitemp_ADC123_free,
    .initializer = unitemp_ADC123_init,
    .deinitializer = unitemp_ADC123_deinit,
    .updater = unitemp_ADC123_update};

bool unitemp_ADC123_alloc(Sensor* sensor, char* args) {
    UNUSED(sensor);
    UNUSED(args);
    FURI_LOG_D(APP_NAME, "Allocating ADC123 sensor. args: %s", args);
    return true;
}

bool unitemp_ADC123_free(Sensor* sensor) {
    UNUSED(sensor);
    FURI_LOG_D(APP_NAME, "Freed ADC123 sensor");
    return true;
}

bool unitemp_ADC123_init(Sensor* sensor) {
    UNUSED(sensor);
    return true;
}

bool unitemp_ADC123_deinit(Sensor* sensor) {
    UNUSED(sensor);
    return true;
}

UnitempStatus unitemp_ADC123_update(Sensor* sensor) {
    ADCSensor* adc_sensor = (ADCSensor*)sensor->instance;
    // TODO: Actual temp calculation
    sensor->temp = adc_sensor->voltage / 10;
    return UT_SENSORSTATUS_OK;
}
