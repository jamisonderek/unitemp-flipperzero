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
#ifndef UNITEMP_ADC
#define UNITEMP_ADC

#include "../unitemp.h"
#include <furi_hal_adc.h>

typedef struct ADCSensor {
    FuriHalAdcHandle* handle;
    const GPIO* data_pin;
    FuriHalAdcChannel data_channel;
    float voltage;
} ADCSensor;

bool unitemp_adc_sensor_alloc(Sensor* sensor, char* args);

bool unitemp_adc_sensor_free(Sensor* sensor);

bool unitemp_adc_sensor_init(Sensor* sensor);

bool unitemp_adc_sensor_deinit(Sensor* sensor);

UnitempStatus unitemp_adc_sensor_update(Sensor* sensor);

#endif