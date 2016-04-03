/*
 * HMC5883 Raspberry Pi library:
 * A C++ library to access HoneyWell HMC5883 sensor in Raspberry Pi.
 *
 * Copyright (C) 2016 Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>
 * 
 */

#ifndef HMC5883_H_
#define HMC5883_H_

#include <stdint.h>

// I2C address
#define HMC5883_ADDRESS_MAG 0x1E

// Registers
#define HMC5883_CONFIG_REG_A 0x00
#define HMC5883_CONFIG_REG_B 0x01
#define HMC5883_MODE_REG 0x02
#define HMC5883_X_MSB 0x03
#define HMC5883_X_LSB 0x04
#define HMC5883_Z_MSB 0x05
#define HMC5883_Z_LSB 0x06
#define HMC5883_Y_MSB 0x07
#define HMC5883_Y_LSB 0x08
#define HMC5883_STATUS_REG 0x09
#define HMC5883_ID_REG_A 0x0A
#define HMC5884_ID_REG_B 0x0B
#define HMC5883_ID_REG_C 0x0C

// Gain setings
#define HMC5883_MAGGAIN_1_3 0x20										// +/- 1.3
#define HMC5883_MAGGAIN_1_9 0x40										// +/- 1.9
#define HMC5883_MAGGAIN_2_5 0x60										// +/- 2.5
#define HMC5883_MAGGAIN_4_0 0x80										// +/- 4.0
#define HMC5883_MAGGAIN_4_7 0xA0										// +/- 4.7
#define HMC5883_MAGGAIN_5_6 0xC0										// +/- 5.6
#define HMC5883_MAGGAIN_8_1 0xE0										// +/- 8.1

// Internal magnetometer data type (In Gauss)
struct HMC5883_MagData {
	double x;
	double y;
	double z;
	double orientation;
};

// Balneário Rincão magnetic declination
#define MAG_DECLINATION 0.31726

class HMC5883 {
		int fd;
		uint8_t magGain;
		double hmc5883_Gauss_LSB_XY;
		double hmc5883_Gauss_LSB_Z;
		HMC5883_MagData magData;										// Last read magnetometer data will be available here
		double declination_angle;
	public:
		HMC5883(double d=0);
		~HMC5883();
		bool setMagGain(uint8_t);
		void setDeclinationAngle(double);
		void Read();
		double getXMagData();
		double getYMagData();
		double getZMagData();
		double getOrientation();
		double getHeading();
};

#endif
