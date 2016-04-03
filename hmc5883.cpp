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

#include <unistd.h>														// close()
#include <stdexcept>
#include <cmath>														// atan() and M_PI

#include "hmc5883.h"
#include "i2c.h"

HMC5883::HMC5883(double d)
{	
	declination_angle = d;
	
	if ((fd = I2C_Setup(HMC5883_ADDRESS_MAG, 2)) < 0)                   // 2 = RPi 2, if you are using RPi 1, set it here
        throw std::runtime_error("Failed to open I2C bus!");
    else
    {   
		if (I2C_WriteReg8(fd, HMC5883_MODE_REG, 0x00) < 0)		        // Enable the magnetometer
			throw std::runtime_error("Error writing into the device!");
		else
		{
			hmc5883_Gauss_LSB_XY = 1100.0F;								// Varies with gain
			hmc5883_Gauss_LSB_Z  = 980.0F;								// Varies with gain
			
			if (!setMagGain(HMC5883_MAGGAIN_1_3))						// Set the gain to a known level
				throw std::runtime_error("Error writing into the device!");
		}
	}
}

HMC5883::~HMC5883()
{
	if (fd >= 0)
		close(fd);
}

bool HMC5883::setMagGain(uint8_t gain)
{
	if (I2C_WriteReg8(fd, HMC5883_CONFIG_REG_B, gain) < 0)
		return false;
	else
	{
		magGain = gain;

		switch(gain)
		{
			case HMC5883_MAGGAIN_1_3:
				hmc5883_Gauss_LSB_XY = 1100;
				hmc5883_Gauss_LSB_Z  = 980;
				break;
			case HMC5883_MAGGAIN_1_9:
				hmc5883_Gauss_LSB_XY = 855;
				hmc5883_Gauss_LSB_Z  = 760;
				break;
			case HMC5883_MAGGAIN_2_5:
				hmc5883_Gauss_LSB_XY = 670;
				hmc5883_Gauss_LSB_Z  = 600;
				break;
			case HMC5883_MAGGAIN_4_0:
				hmc5883_Gauss_LSB_XY = 450;
				hmc5883_Gauss_LSB_Z  = 400;
				break;
			case HMC5883_MAGGAIN_4_7:
				hmc5883_Gauss_LSB_XY = 400;
				hmc5883_Gauss_LSB_Z  = 255;
				break;
			case HMC5883_MAGGAIN_5_6:
				hmc5883_Gauss_LSB_XY = 330;
				hmc5883_Gauss_LSB_Z  = 295;
				break;
			case HMC5883_MAGGAIN_8_1:
				hmc5883_Gauss_LSB_XY = 230;
				hmc5883_Gauss_LSB_Z  = 205;
				break;
		}
		
		return true;
	}
}

void HMC5883::setDeclinationAngle(double angle)
{
	declination_angle = angle;
}

void HMC5883::Read()
{	
	int8_t x_msb = I2C_ReadReg8(fd, HMC5883_X_MSB);
	int8_t x_lsb = I2C_ReadReg8(fd, HMC5883_X_LSB);
	int8_t z_msb = I2C_ReadReg8(fd, HMC5883_Z_MSB);
	int8_t z_lsb = I2C_ReadReg8(fd, HMC5883_Z_LSB);
	int8_t y_msb = I2C_ReadReg8(fd, HMC5883_Y_MSB);
	int8_t y_lsb = I2C_ReadReg8(fd, HMC5883_Y_LSB);

	// Shift values to create properly formed integer (low byte first)
	magData.x = int16_t(x_lsb | (int16_t(x_msb) << 8))/hmc5883_Gauss_LSB_XY*100;		// 100: gauss to uT conversion
	magData.y = int16_t(y_lsb | (int16_t(y_msb) << 8))/hmc5883_Gauss_LSB_XY*100;
	magData.z = int16_t(z_lsb | (int16_t(z_msb) << 8))/hmc5883_Gauss_LSB_Z*100;
	magData.orientation = 0.0;											// >>>>>>>>>>>>> ToDo: Calculate orientation
}

double HMC5883::getXMagData()
{
	return magData.x;
}

double HMC5883::getYMagData()
{
	return magData.y;
}

double HMC5883::getZMagData()
{
	return magData.z;
}

double HMC5883::getOrientation()
{
	return magData.orientation;
}

double HMC5883::getHeading()
{	
	double heading = atan2(magData.y, magData.x);
	
	heading += declination_angle;
	
	if(heading < 0)														// Correct for when signs are reversed
		heading += 2*M_PI;
		
	if(heading > 2*M_PI)												// Check for wrap due to addition of declination
		heading -= 2*M_PI;

	double headingDegrees = heading * 180/M_PI;							// Convert radians to degrees for readability
	
	return headingDegrees;
}
