/*##############################################################*/
/* 																*/
/* File		: bmp.c												*/
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

#include <stdint.h>
#include <stdio.h>
#include "bmp.h"
#include "ST7789.h"
#include <bcm2835.h>


// store BMP files in memory
// ----------------------------------------------------------
#ifdef CM_262K
int32_t Read_bmp2memory ( char const *file_name, uint32_t *picture_pointer  )
#else
int32_t Read_bmp2memory ( char const *file_name, uint16_t *picture_pointer  )
#endif
{
	FILE *my_file;
	
	//	bmp_header_t bmp_header;
	uint8_t bmp_header_buffer[54];
	uint8_t bmp_line_buffer[ DISPLAY_WIDTH * 3 ];
	
	uint16_t bfType;
	uint32_t bfOffBits;
	
	uint32_t biSize;
	
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biBitCount;
	
	uint32_t y,x;
	
	uint8_t red, green, blue;
	uint32_t color;

	
	// check for file
	//printf( "Opening input file... " );
	if( ( my_file = fopen( file_name, "rb" ) ) == NULL )
	{
		printf( "ERROR: Could not open input file for reading.\n" );
		return( -1 );
	}
	//printf( "OK\n" );


	// read header
	fread( &bmp_header_buffer, 1, 54, my_file );


	// check for "BM"
	//printf( "Checking magic number... " );
	bfType = bmp_header_buffer[1];
	bfType = (bfType << 8) | bmp_header_buffer[0];
	if( bfType != 0x4D42)
	{
		printf( "ERROR: Not a bitmap file.\n" );
		fclose( my_file );
		return( -1 );
	}
	//printf( "OK\n" );

		
	//printf( "Checking header size... " );
	biSize = bmp_header_buffer[17];
	biSize = (biSize << 8) | bmp_header_buffer[16];
	biSize = (biSize << 8) | bmp_header_buffer[15];
	biSize = (biSize << 8) | bmp_header_buffer[14];  
    //printf( "%d ", biSize);
    if( biSize != 40 )
    {
		printf( "ERROR: Not Windows V3\n" );
        fclose( my_file );
		return( -1 );
    }
    //printf( "OK\n" );
	
	
	//printf( "Checking dimensions... " );
	biWidth = bmp_header_buffer[21];
	biWidth = (biWidth << 8) | bmp_header_buffer[20];
	biWidth = (biWidth << 8) | bmp_header_buffer[19];
	biWidth = (biWidth << 8) | bmp_header_buffer[18];
	
	biHeight = bmp_header_buffer[25];
	biHeight = (biHeight << 8) | bmp_header_buffer[24];
	biHeight = (biHeight << 8) | bmp_header_buffer[23];
	biHeight = (biHeight << 8) | bmp_header_buffer[22];

	biBitCount = bmp_header_buffer[29];
	biBitCount = (biBitCount << 8) | bmp_header_buffer[28];
	
    //printf( "%dx%dx%dbbp. ", biWidth, biHeight, biBitCount );
    
    if( (biWidth != DISPLAY_WIDTH) || (biHeight != DISPLAY_HEIGHT) || (biBitCount != 24) )
	{
		printf( "ERROR. %dx%dx%d required.\n", DISPLAY_WIDTH, DISPLAY_HEIGHT, 24 );
		fclose( my_file );
        return( -1 );
	}
	//printf( "OK\n\n" );
	
	
	bfOffBits = bmp_header_buffer[13];
	bfOffBits = (bfOffBits << 8) | bmp_header_buffer[12];
	bfOffBits = (bfOffBits << 8) | bmp_header_buffer[11];
	bfOffBits = (bfOffBits << 8) | bmp_header_buffer[10];
	
	//printf( "biOffBits = %d\n", bfOffBits );
	
	
	fseek( my_file, bfOffBits, SEEK_SET );
    //printf( "Filling picture buffer... \n" );


	for (y=DISPLAY_HEIGHT; y>0; y--)
	{
		fread( &bmp_line_buffer[0], sizeof(bmp_line_buffer), 1, my_file );
		for (x=DISPLAY_WIDTH; x>0; x--)
		{	
			blue =  bmp_line_buffer[(x-1)*3 +0];
			green = bmp_line_buffer[(x-1)*3 +1];
			red =   bmp_line_buffer[(x-1)*3 +2];
			
			#ifdef CM_262K
				color = (red >> 2);
				color = color << 8;
				color = color | (green >> 2);
				color = color << 8;
				color = color | (blue >> 2);
				color = color << 2;
			#elif defined(CM_65K)
				color = (red >> 3);
				color = color << 6;
				color = color | (green >> 2);
				color = color << 5;
				color = color | (blue >> 3);
			#else
				#error "color_mode not defined"
			#endif
				
			*picture_pointer = color;
			picture_pointer--;
		}
	}
	
	fclose( my_file );
	
	return ( 0 );
}
