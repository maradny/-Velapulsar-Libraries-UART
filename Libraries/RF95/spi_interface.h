#ifndef spi_interface_h
#define spi_interface_h

#include "driverlib.h"
#include "delay.h"

#ifdef KIT
#define SPI_PORT EUSCI_B0_BASE
#define SERVER_CS_PORT     GPIO_PORT_P4
#define SERVER_CS_PIN      GPIO_PIN1
#define SERVER_INT_PORT    GPIO_PORT_P2
#define SERVER_INT_PIN     GPIO_PIN7
#else
#define SPI_PORT EUSCI_A0_BASE
#endif

typedef enum{
	RADIO,
	SERVER
}spiSlave;

void initSPI(void);
void SPISend (spiSlave slave, uint8_t *data, int size);
void SPI_Read (spiSlave slave, uint8_t *dest, int size);

#endif
