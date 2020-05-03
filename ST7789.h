/*##############################################################*/
/* 																*/
/* File		: ST7789.h											*/
/*																*/
/* Project	: TFT for Raspberry Pi Revision 2					*/
/* 																*/
/* Date		: 2014-08-13  	    last update: 2014-08-13			*/
/* 																*/
/* Author	: Hagen Ploog   									*/
/*		  	  Kai Gillmann										*/
/*		  	  Timo Pfander										*/
/* 																*/
/* IDE	 	: Geany 1.22										*/
/* Compiler : gcc (Debian 4.6.3-14+rpi1) 4.6.3					*/
/*																*/
/* Copyright (C) 2013 admatec GmbH								*/
/*																*/
/*																*/	
/* Description  :												*/
/* 																*/
/*	This file contains the defines of the ST register.			*/
/*	Furthermore declared the file several functions to control	*/
/*	the tft controller ST7789.									*/
/*																*/
/*																*/
/* License:														*/
/*																*/
/*	This program is free software; you can redistribute it 		*/ 
/*	and/or modify it under the terms of the GNU General			*/ 	
/*	Public License as published by the Free Software 			*/
/*	Foundation; either version 3 of the License, or 			*/
/*	(at your option) any later version. 						*/
/*    															*/
/*	This program is distributed in the hope that it will 		*/
/*	be useful, but WITHOUT ANY WARRANTY; without even the 		*/
/*	implied warranty of MERCHANTABILITY or 						*/
/*	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 		*/
/*	Public License for more details. 							*/
/*																*/
/*	You should have received a copy of the GNU General 			*/
/*	Public License along with this program; if not, 			*/
/*	see <http://www.gnu.org/licenses/>.							*/
/*																*/
/*																*/
/* Revision History:											*/
/*																*/
/*	Version 1.0 - Initial release								*/
/*																*/
/*																*/
/*																*/
/*##############################################################*/

#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>
#include <stdarg.h>

//color modes (color depths) { CM_262K, CM_65K };
#define CM_65K

// TFT dimensions
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define PICTURE_PIXELS ( DISPLAY_WIDTH*DISPLAY_HEIGHT )

// ST register -> see datasheet ST7789
#define NOP			0x00
#define SWRESET 	0x01
#define RDDID 		0x04
#define RDDST 		0x09
#define RDDPM 		0x0A
#define RDDMADCTL 	0x0B
#define RDDCOLMOD 	0x0C
#define RDDIM 		0x0D
#define RDDSM 		0x0E
#define RDDSDR 		0x0F
#define SLPIN 		0x10
#define SLPOUT		0x11
#define PTLON		0x12
#define NORON		0x13
#define INVOFF		0x20
#define INVON		0x21
#define GAMSET		0x26
#define DISPOFF		0x28
#define DISPON		0x29
#define CASET		0x2A
#define RASET		0x2B
#define RAMWR		0x2C
#define RAMRD		0x2E
#define PTLAR		0x30
#define VSCRDEF		0x33
#define TEOFF		0x34
#define TEON		0x35
#define MADCTL		0x36
#define VSCRSADD	0x37
#define IDMOFF		0x38
#define IDMON		0x39
#define COLMOD		0x3A
#define RAMWRC		0x3C
#define RAMRDC		0x3E
#define TESCAN		0x44
#define RDTESCAN	0x45
#define WRDISBV		0x51
#define RDDISBV		0x52
#define WRCTRLD		0x53
#define RDCTRLD		0x54
#define WRCACE		0x55
#define RDCABC		0x56
#define WRCABCMB	0x5E
#define RDCABCMB	0x5F
#define RDID1		0xDA
#define RDID2		0xDB
#define RDID3		0xDC

#define RAMCTRL		0xB0
#define RGBCTRL		0xB1
#define PORCTRL		0xB2
#define FRCTRL1 	0xB3
#define GCTRL		0xB7
#define DGMEN		0xBA
#define VCOMS		0xBB
#define LCMCTRL		0xC0
#define IDSET		0xC1
#define VDVVRHEN	0xC2
#define VRHS		0xC3
#define VDVSET		0xC4
#define VCMOFSET	0xC5
#define FRCTRL2		0xC6
#define CABCCTRL	0xC7
#define REGSEL1		0xC8
#define REGSEL2 	0xCA
#define PWCTRL1		0xD0
#define VAPVANEN	0xD2
#define PVGAMCTRL	0xE0
#define NVGAMCTRL	0xE1
#define DGMLUTR		0xE2
#define DGMLUTB		0xE3
#define GATECTRL	0xE4
#define PWCTRL2		0xE8
#define EQCTRL		0xE9
#define PROMCTRL	0xEC
#define PROMEN		0xFA
#define NVMSET		0xFC
#define PROMACT		0xFE


// declaration of a union (used in ST7789.c and tft.c)
// ----------------------------------------------------------
union my_union
{
	uint16_t value;
	struct
	{
		unsigned char low;
		unsigned char high;
	} split;
};


// initialization of ST7789
// ----------------------------------------------------------
void STcontroller_init( void );


// write command to a register
// ----------------------------------------------------------
void STcontroller_SetRegister( int reg, int count, ... );


// show the BMP picture on the TFT screen
// ----------------------------------------------------------
#ifdef CM_262K
void STcontroller_Write_Picture( uint32_t *data, uint32_t count );
#else
void STcontroller_Write_Picture( uint16_t *data, uint32_t count );
#endif


// define area of frame memory where MCU can access
// ----------------------------------------------------------
void set_row( uint16_t row_start, uint16_t row_end );  
void set_column( uint16_t col_start, uint16_t col_end );  


#endif
