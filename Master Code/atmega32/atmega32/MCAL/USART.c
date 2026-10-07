/*
 * USART.c
 *
 *  Created on: 13 Feb 2023
 *      Author: Alaa Wahba
 */

#include "inc/USART.h"
#define Default_Stop 		'\r'
static uint32 flag = 1;
static uint8 *TX_str;

void USART_init(UART_Config_t *pinConfig) {
	/*   Baud rate   */
	uint32 UBRR = (F_CPU / (Speed_EN * pinConfig->baudRate)) - 1;
	UBRRL = (uint8)UBRR;
	UBRRH = (uint8)(UBRR >> 8);

	/*  FRAME 	  */
	//UCSRC = 1000 001
	SET(UCSRC,URSEL);
	//This bit selects  Asynchronous
	CLEAR(UCSRC,UMSEL);
	//These bits enable and set type of parity generation (No parity )
	CLEAR(UCSRC,UPM1);
	CLEAR(UCSRC,UPM0);
	//This bit selects the number of Stop Bits to be inserted by the Transmitter (1 bit)
	CLEAR(UCSRC,USBS);
	// sets the number of data bits (8 bits by default)
	CLEAR(UCSRB,UCSZ2);
	SET(UCSRC,UCSZ1);
	SET(UCSRC,UCSZ0);

	/*   Enable  */
	// set RXEN.TXEN
	SET(UCSRB,RXEN);
	SET(UCSRB,TXEN);
}

void USART_send(uint8 data) {
	//The transmit buffer can only be written when the UDRE Flag in the UCSRA Register is set.
	while (!GET(UCSRA, UDRE))
		;
	UDR = data;

}
uint8 USART_recieve() {
	/*
	 * This flag bit is set when there are unread data in the receive buffer
	 * and cleared when the receive buffer is empty
	 */
	while (!GET(UCSRA, RXC))
		;

	return UDR;	

}

void USART_sendNumber(uint32 data) {

	char str[7];

	sprintf(str, "%d", data);  // Adjust the formatting to your liking.

	USART_sendString(str);

}
uint32 USART_recieveNumber() {
	uint32 num;
	return num;
}
void USART_sendString(uint8 *str) {
	for (uint8 i = 0; i < str[i]; i++)
		USART_send(str[i]);

	USART_send(Default_Stop);
}

void USART_recieveString(uint8 *Buff) {

	uint8 i = 0;
	Buff[i] = USART_recieve();
	while (Buff[i] != Default_Stop) {
		i++;
		Buff[i] = USART_recieve();
	}
	Buff[i] = '\0';
}

