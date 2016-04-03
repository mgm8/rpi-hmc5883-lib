/*
 * i2c.cpp:
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

#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "i2c.h"

// I2C definitions
#define I2C_SLAVE 0x0703
#define I2C_SMBUS 0x0720                                                // SMBus-level access

#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0

// SMBus transaction types
#define I2C_SMBUS_QUICK		        0
#define I2C_SMBUS_BYTE		        1
#define I2C_SMBUS_BYTE_DATA	        2 
#define I2C_SMBUS_WORD_DATA	        3
#define I2C_SMBUS_PROC_CALL	        4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7                                   // SMBus 2.0
#define I2C_SMBUS_I2C_BLOCK_DATA    8

// SMBus messages
#define I2C_SMBUS_BLOCK_MAX	32	                                        // As specified in SMBus standard	
#define I2C_SMBUS_I2C_BLOCK_MAX	32                                      // Not specified but we use same structure

// Structures used in the ioctl() calls
union i2c_smbus_data
{
    uint8_t  byte;
    uint16_t word;
    uint8_t  block[I2C_SMBUS_BLOCK_MAX + 2];                            // block [0] is used for length + one more for PEC
};

struct i2c_smbus_ioctl_data
{
    char read_write;
    uint8_t command;
    int size;
    union i2c_smbus_data *data;
};

static inline int i2c_smbus_access(int fd, char rw, uint8_t command, int size, union i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = rw;
    args.command    = command;
    args.size       = size;
    args.data       = data;
    return ioctl(fd, I2C_SMBUS, &args);
}

//**********************************************************************
// -- INITIALIZATION ---------------------------------------------------
//**********************************************************************

int I2C_Setup(int device_id, int rpi_version)
{
    int fd;
    const char *device_adr;
    
    // RPi 1 and 2 have different I2C device address
    if (rpi_version == 1)
        device_adr = "/dev/i2c-0";
    else if (rpi_version == 2)
        device_adr = "/dev/i2c-1";
    
    if ((fd = open(device_adr, O_RDWR)) < 0)
        return -1;

    if (ioctl(fd, I2C_SLAVE, device_id) < 0)
        return -1;

    return fd;
}

//**********************************************************************
// -- READ -------------------------------------------------------------
//**********************************************************************

int I2C_Read(int fd)
{
    union i2c_smbus_data data;

    if (i2c_smbus_access(fd, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
        return -1;
    else
        return data.byte & 0xFF;
}

int I2C_ReadReg8(int fd, int reg)
{
    union i2c_smbus_data data;

    if (i2c_smbus_access(fd, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))
        return -1;
    else
        return data.byte & 0xFF;
}

int I2C_ReadReg16(int fd, int reg)
{
    union i2c_smbus_data data;

    if (i2c_smbus_access(fd, I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA, &data))
        return -1;
    else
        return data.word & 0xFFFF;
}

//**********************************************************************
// -- WRITE ------------------------------------------------------------
//**********************************************************************

int I2C_Write(int fd, int data)
{
    return i2c_smbus_access(fd, I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL);
}

int I2C_WriteReg8(int fd, int reg, int value)
{
    union i2c_smbus_data data;

    data.byte = value;
    return i2c_smbus_access(fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data);
}

int I2C_WriteReg16(int fd, int reg, int value)
{
    union i2c_smbus_data data;

    data.word = value;
    return i2c_smbus_access(fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_WORD_DATA, &data);
}
