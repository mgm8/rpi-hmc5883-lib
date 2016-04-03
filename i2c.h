/*
 * i2c.h:
 * Functions to Raspberry Pi I2C access.
 *
 * Copyright (C) 2016 Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * Based on wiringPi library I2C module (wiringPiI2C)
 * <https://projects.drogon.net/raspberry-pi/wiringpi/>
 * Copyright (c) 2013 Gordon Henderson
 * 
 * This file is part of HMC5883 Raspberry Pi library.
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

#ifndef I2C_H_
#define I2C_H_

int I2C_Setup(const int, int);

int I2C_Read(int);
int I2C_ReadReg8(int, int);
int I2C_ReadReg16(int, int);

int I2C_Writed(int, int);
int I2C_WriteReg8(int, int, int);
int I2C_WriteReg16(int, int, int);

#endif
