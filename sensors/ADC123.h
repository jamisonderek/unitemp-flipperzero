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
#ifndef UNITEMP_ADC123
#define UNITEMP_ADC123

#include "../unitemp.h"
#include "../Sensors.h"

// ADC123 is a made up example sensor that reads ADC values from pin A7.
extern const SensorType ADC123;

bool unitemp_ADC123_alloc(Sensor* sensor, char* args);

bool unitemp_ADC123_init(Sensor* sensor);

bool unitemp_ADC123_deinit(Sensor* sensor);

UnitempStatus unitemp_ADC123_update(Sensor* sensor);

bool unitemp_ADC123_free(Sensor* sensor);

#endif