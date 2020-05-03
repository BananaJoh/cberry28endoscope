/*##############################################################*/
/* 																*/
/* File		: ST7789.c											*/
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
/*  This file contain several functions to initialize and       */
/*  control	the tft controller ST7789.							*/
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

#include <bcm2835.h>
#include <stdio.h>
#include <stdarg.h>
#include "ST7789.h"
#include "tft.h"
#include "bmp.h"

// initialization of ST7789
// ----------------------------------------------------------
void STcontroller_init( void )
{	
	// *************** wakeup display
    STcontroller_SetRegister(SLPOUT, 0);
    bcm2835_delay(120);
    
    // *************** display and color format setting
    
    // write data from top-left to bottom-right
    STcontroller_SetRegister(MADCTL, 1, 0xA0);	// 0x00
    
    #ifdef CM_262K
		// 18bit/pixel
		// 262K-colors (RGB 6-6-6)
		STcontroller_SetRegister(COLMOD, 1, 0x06);
	#elif defined(CM_65K)
		// 16bit/pixel
		// 65K-colors (RGB 5-6-5)
		STcontroller_SetRegister(COLMOD, 1, 0x05);
	#else
		#error "color_mode not defined"
	#endif
    
    // *************** ST7789 porch setting
    
    // seperate porch control = disabled
    // front porch in normal mode  = 13 CLK pulse
    // back porch in normal mode   = 13 CLK pulse
    // front porch in idle mode    =  3 CLK pulse
    // back porch in idle mode     =  3 CLK pulse
    // front porch in partial mode =  3 CLK pulse
    // back porch in partial mode  =  3 CLK pulse
    STcontroller_SetRegister(PORCTRL, 5, 0x0C, 0x0C, 0x00, 0x33, 0x33);
   
	// *************** ST7789 Power setting

    // VGH =  13.26V=0x03
    // VGL = -10.43V=0x05
    STcontroller_SetRegister( GCTRL, 1, 0x35 );
    
    // VDV and VRH register value comes from command line
    STcontroller_SetRegister( VDVVRHEN, 2, 0x01, 0xFF );
    
    // VAP = 4.7 + Vcom + Vcom_offset + 0.5*VDV//0x17h
    STcontroller_SetRegister( VRHS, 1, 0x17 );
    
    // VDV = 0V //VDVS[5:0]=20h
    STcontroller_SetRegister( VDVSET, 1, 0x20 );
    
    // Vcom = 0.675V 
    STcontroller_SetRegister( VCOMS, 1, 0x17 );
    
    // Vcom_offset = 0V 
    STcontroller_SetRegister( VCMOFSET, 1, 0x20 );
        
    // AVDD =  6.8V
    // AVCL = -4.8V//0x02
    // VDS  =  2.3V//0x01
    STcontroller_SetRegister(PWCTRL1, 2, 0xA4, 0xA1);
    
    // *************** ST7789 gamma setting
    
    STcontroller_SetRegister(PVGAMCTRL, 14, 0xD0, 0x00, 0x14, 0x15, 0x13, 0x2C, 0x42, 0x43, 0x4E, 0x09, 0x16, 0x14, 0x18, 0x21);
    STcontroller_SetRegister(NVGAMCTRL, 14, 0xD0, 0x00, 0x14, 0x15, 0x13, 0x0B, 0x43, 0x55, 0x53, 0x0C, 0x17, 0x14, 0x23, 0x20);
     
    // *************** miscellaneous settings
    
    // define area (start row, end row, start column, end column) of frame memory where MCU can access
    set_row( 0, DISPLAY_HEIGHT - 1 );
	set_column( 0, DISPLAY_WIDTH - 1 );
    
    // *************** display on
     
    STcontroller_SetRegister(DISPON, 0);	
}


// write command to a register
// ----------------------------------------------------------
void STcontroller_SetRegister( int reg, int count, ... )
{
	int i;
	va_list args;
	va_start(args, count);
		
	TFT_RegWrite ( reg );

	for( i=count; i > 0; i-- )
	{
		TFT_DataWrite ( (uint8_t)va_arg(args, int) );
	}
		
    va_end(args);
}


// show the BMP picture on the TFT screen
// ----------------------------------------------------------
#ifdef CM_262K
void STcontroller_Write_Picture( uint32_t *data, uint32_t count )
#else
void STcontroller_Write_Picture( uint16_t *data, uint32_t count )
#endif
{	
	TFT_RegWrite( RAMWR );
	TFT_DataMultiWrite( data, count );
	
}


// define area of frame memory where MCU can access
// ----------------------------------------------------------
void set_row( uint16_t row_start, uint16_t row_end ) 
{
	union my_union start;
	union my_union end;
	
	start.value = row_start;
	end.value = row_end;
	STcontroller_SetRegister( RASET, 4, start.split.high, start.split.low, end.split.high, end.split.low );
} 

void set_column( uint16_t col_start, uint16_t col_end ) 
{   
	union my_union start;
	union my_union end;
	
	start.value = col_start;
	end.value = col_end;
	STcontroller_SetRegister( CASET, 4, start.split.high, start.split.low, end.split.high, end.split.low );
}



