/*##############################################################*/
/* 																*/
/* File		: tft.h												*/
/*																*/
/* Project	: TFT for Raspberry Pi Revision 2					*/
/* 																*/
/* Date		: 2014-08-13   	    last update: 2014-08-13			*/
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
/*	This file declared functions for the SPI communications 	*/
/*	between the Raspberry Pi and the TFT and for the			*/
/*	initialization of the GPIO Pins of the Raspberry Pi.		*/
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

#ifndef TFT_H
#define TFT_H

#include <stdint.h>
#include "ST7789.h"

#define MOSI      RPI_V2_GPIO_P1_19
#define SCLK      RPI_V2_GPIO_P1_23
#define SPI_CE0   RPI_V2_GPIO_P1_24
#define ST_RST    RPI_V2_GPIO_P1_22
#define ST_RS     RPI_V2_GPIO_P1_15
#define BL_PWM	  RPI_V2_GPIO_P1_12	

// PWM settings
#define PWM_CHANNEL    0
#define PWM_RANGE    255


// initialization of GPIO and SPI
// ----------------------------------------------------------
void TFT_init_board( void );


// hard reset of the graphic controller and the tft
// ----------------------------------------------------------
void TFT_hard_reset( void );


// write byte to register
// ----------------------------------------------------------
void TFT_RegWrite( uint8_t reg );


// write byte to tft
// ----------------------------------------------------------
void TFT_DataWrite( uint8_t value );


// write 'count'-bytes to tft
// ----------------------------------------------------------
#ifdef CM_262K
void TFT_DataMultiWrite( uint32_t *data, uint32_t count );
#else
void TFT_DataMultiWrite( uint16_t *data, uint32_t count );
#endif

// write data via SPI to tft
// ----------------------------------------------------------
void TFT_SPI_data_out ( uint8_t data );


// set PWM value for backlight -> 0 (0% PWM) - PWM_RANGE (100% PWM)
// PWM channel 0
// MARKSPACE mode
// ----------------------------------------------------------
void TFT_SetBacklightPWMValue( uint8_t BL_value );


#endif
