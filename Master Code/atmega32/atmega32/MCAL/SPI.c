/*
 * SPI.c
 *
 *  Created on: 22 Feb 2023
 *      Author: Alaa Wahba
 */

#include "inc/SPI.h"

void SPI_Init() {
#ifdef MASTER_MODE
	/* GPIO pins Configuration */
	DIO_initPin(SS,PORT_B,OUTPUT);
	DIO_initPin(MOSI,PORT_B,OUTPUT);
	DIO_initPin(MISO,PORT_B,INPUT);
	DIO_initPin(SCLK,PORT_B,OUTPUT);
	/* Master Configuration */
	
	// Configure it as Master
	SET(SPCR,MSTR);
	// shift clock =clk/16
	CLEAR(SPCR,SPR1);
	SET(SPCR,SPR0);
	CLEAR(SPSR,SPI2X);
	// Enable SPI
	SET(SPCR,SPE);
#endif

#ifdef SLAVE_MODE
	/* GPIO pins Configuration */
	DIO_initPin(SS,PORT_B,INPUT);
	DIO_initPin(MOSI,PORT_B,INPUT);
	DIO_initPin(MISO,PORT_B,OUTPUT);
	DIO_initPin(SCLK,PORT_B,INPUT);	
	/* Slave Configuration */
	
	// Configure it as Slave
	CLEAR(SPCR,MSTR);
	// Enable SPI
	SET(SPCR,SPE);
#endif

}
uint8 SPI_SendRecieveData(uint8 Data) {
	SPDR = Data;
	while(!GET(SPSR,SPIF)) // Wait until transfer finishes
		;
		
	return SPDR; 
}
