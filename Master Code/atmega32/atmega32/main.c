/*
 * Master.c
 *
 * Created: 25/09/2026 11:06:24 PM
 * Author : Rahf Ayman
 */ 

#include <avr/io.h>

#include "SPI.h"

#define DS1307_ADDRESS    0xD0  //0x68 in 7bit 110 1000 -> 1101 0000

#define DS1307_SECONDS    0x00
#define DS1307_MINUTES    0x01
#define DS1307_HOURS      0x02

#define I2C_WRITE         0
#define I2C_READ          1

// SPI packet
#define RTC_START_BYTE    0xAA

uint8 BCD_To_Decimal(uint8 bcd)
{
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint8 DS1307_ReadRegister(uint8 registerAddress)
{
    uint8 data;
	
    I2C_Start();

    I2C_SendSlaveAddress(DS1307_ADDRESS, I2C_WRITE);

    I2C_Write(registerAddress);

    I2C_Start();

    I2C_SendSlaveAddress(DS1307_ADDRESS, I2C_READ);

    // send NACK after read
    data = I2C_Read(0);

    I2C_Stop();

    return data;
}

void DS1307_ReadTime(uint8 *hours,
uint8 *minutes,
uint8 *seconds)
{
	uint8 rawHours;
	uint8 rawMinutes;
	uint8 rawSeconds;

	rawSeconds = DS1307_ReadRegister(DS1307_SECONDS);
	rawMinutes = DS1307_ReadRegister(DS1307_MINUTES);
	rawHours   = DS1307_ReadRegister(DS1307_HOURS);

	// RTC stores values in BCD
	*seconds = BCD_To_Decimal(rawSeconds);
	*minutes = BCD_To_Decimal(rawMinutes);
	*hours   = BCD_To_Decimal(rawHours);
}

int main(void)
{
	uint8 hours;
    uint8 minutes;
    uint8 seconds;

    I2C_init();

    SPI_Init();

    while (1)
    {
		/* Read RTC */
        DS1307_ReadTime(&hours,
                        &minutes,
                        &seconds);
		
		/* Send RTC data to Slave */

        // Start of packet
        SPI_SendRecieveData(RTC_START_BYTE);

        SPI_SendRecieveData(hours);

        SPI_SendRecieveData(minutes);

        SPI_SendRecieveData(seconds);

        _delay_ms(1000);
    }
}
