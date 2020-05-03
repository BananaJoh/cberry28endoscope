/*##############################################################*/
/* 																*/
/* File		: tft.c												*/
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
/*	This file controlls the communications between the 			*/
/*	Raspberry Pi and the TFT. The file initialized also the		*/
/*	GPIO Pins of the Raspberry Pi.								*/
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
#include <stdarg.h>
#include <stdio.h>
#include <bcm2835.h>
#include "ST7789.h"
#include "tft.h"


// initialization of GPIO and SPI
// ----------------------------------------------------------
void TFT_init_board ( void )
{
	// *************** set the pins to be an output and turn them on
	
	bcm2835_gpio_fsel( ST_RS, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_write( ST_RS, HIGH );
	
	bcm2835_gpio_fsel( ST_RST, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_write( ST_RST, HIGH );
	
	// *************** set pins for PWM
	
	bcm2835_gpio_fsel( BL_PWM, BCM2835_GPIO_FSEL_ALT5 );

	// Clock divider is set to 16.
    // 1.2MHz/1024 = 1171.875Hz
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL, PWM_RANGE);
	
	// *************** set pins for SPI
	
    bcm2835_gpio_fsel(MOSI, BCM2835_GPIO_FSEL_ALT0); 
    bcm2835_gpio_fsel(SCLK, BCM2835_GPIO_FSEL_ALT0);
    bcm2835_gpio_fsel(SPI_CE0, BCM2835_GPIO_FSEL_ALT0);
        
    // set the SPI CS register to the some sensible defaults
    volatile uint32_t* paddr = bcm2835_spi0 + BCM2835_SPI0_CS/8;
    bcm2835_peri_write( paddr, 0 ); // All 0s
    
    // clear TX and RX fifos
    bcm2835_peri_write_nb( paddr, BCM2835_SPI0_CS_CLEAR );
    
	bcm2835_spi_setBitOrder( BCM2835_SPI_BIT_ORDER_MSBFIRST );      
    bcm2835_spi_setDataMode( BCM2835_SPI_MODE0 );                 
    bcm2835_spi_setClockDivider( BCM2835_SPI_CLOCK_DIVIDER_2 ); 
    bcm2835_spi_chipSelect( BCM2835_SPI_CS0 );                      
    bcm2835_spi_setChipSelectPolarity( BCM2835_SPI_CS0, LOW );
}


// hard reset of the tft controller
// ----------------------------------------------------------
void TFT_hard_reset( void )
{
 	bcm2835_delay( 1 );
	bcm2835_gpio_write( ST_RST, LOW );
    bcm2835_delay( 10 );
 	bcm2835_gpio_write( ST_RST, HIGH );
 	bcm2835_delay( 120 );
}


// write byte to register
// ----------------------------------------------------------
void TFT_RegWrite( uint8_t reg )
{
	bcm2835_gpio_write( ST_RS, LOW );                    
    TFT_SPI_data_out ( reg );	
}


// write byte to tft
// ----------------------------------------------------------
void TFT_DataWrite( uint8_t value )
{
	bcm2835_gpio_write( ST_RS, HIGH );                    
    TFT_SPI_data_out ( value );	
}


// write 3 * 'count'-bytes to tft
// ----------------------------------------------------------
#ifdef CM_262K
void TFT_DataMultiWrite( uint32_t *data, uint32_t count )
#else
void TFT_DataMultiWrite( uint16_t *data, uint32_t count )
#endif
{
	volatile uint32_t* paddr = bcm2835_spi0 + BCM2835_SPI0_CS/4;
    volatile uint32_t* fifo  = bcm2835_spi0 + BCM2835_SPI0_FIFO/4;
	
	volatile uint32_t* gpio_set   = bcm2835_gpio + BCM2835_GPSET0/4;
	volatile uint32_t* gpio_clear = bcm2835_gpio + BCM2835_GPCLR0/4;
	
	uint32_t i;
	
	bcm2835_gpio_write( ST_RS, HIGH );  
                  
	for( i=0; i<count; i++ )
	{		
		// activate SPI transfer
		*paddr |= BCM2835_SPI0_CS_TA;

		// fill the FIFO
		#ifdef CM_262K
		*fifo = (uint8_t)(data[i] >> 16);	
		#endif
		*fifo = (uint8_t)(data[i] >> 8);
		*fifo = (uint8_t)(data[i] & 0xFF);
		
		
		// write fifo data to SPI TX buffer
		while (!(*paddr & BCM2835_SPI0_CS_DONE))
		{
			// clear SPI RX buffer
			*paddr |=BCM2835_SPI0_CS_CLEAR_RX;	
		};
		
		// deactivate SPI transfer
		*paddr &= ~BCM2835_SPI0_CS_TA;	
	//	if(i>3) { while(1);	}
	}
}


// write data via SPI to tft
// ----------------------------------------------------------
void TFT_SPI_data_out ( uint8_t data )
{	
	bcm2835_spi_writenb( &data, 1 );
}


// set PWM value for backlight -> 0 (0% PWM) - 1024 (100% PWM)
// ----------------------------------------------------------
void TFT_SetBacklightPWMValue( uint8_t BL_value )
{
	bcm2835_pwm_set_data( PWM_CHANNEL, BL_value );
}
