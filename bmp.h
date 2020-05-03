/*##############################################################*/
/* 																*/
/* File		: bmp.h												*/
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
/*	The current software can display BMP files on the C-Berry.	*/
/*	The BMP file must have a dimension of 320 x 240 pixel and	*/
/*	a color depth of 24Bit.										*/
/*	The picture(s) will be stored into a memory. The function	*/
/*	returns back a pointer with the address of the memory.		*/
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

#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include "ST7789.h"

// store BMP files in memory
// ----------------------------------------------------------
#ifdef CM_262K
int32_t Read_bmp2memory ( char const *file_name, uint32_t *picture_pointer  );
#else
int32_t Read_bmp2memory ( char const *file_name, uint16_t *picture_pointer  );
#endif


#endif
