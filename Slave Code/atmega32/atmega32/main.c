/*
 * Slave.c
 *
 * Created: 25/09/2026 11:06:24 PM
 * Author : Rahf Ayman
 */ 

#include <avr/io.h>

#include "SPI.h"
#include "USART.h"

#define RTC_START_BYTE    0xAA

UART_Config_t uartConfig =
{
    .baudRate = 9600
};


int main(void)
{
    uint8 data;

    uint8 hours;
    uint8 minutes;
    uint8 seconds;

    SPI_Init();

    USART_init(&uartConfig);

    while (1)
    {
		/* wait start packet */
        data = SPI_SendRecieveData(0x00);

        if (data == RTC_START_BYTE)
        {
			// Receive RTC data
            hours = SPI_SendRecieveData(0x00);

            minutes = SPI_SendRecieveData(0x00);

            seconds = SPI_SendRecieveData(0x00);
			
			// Send to Virtual Terminal
			char str[20]; 
			
			sprintf(str, "RTC: %02u:%02u:%02u\r\n", hours, minutes, seconds); 
			
			USART_sendString((uint8 *)str);
        }
    }
}
