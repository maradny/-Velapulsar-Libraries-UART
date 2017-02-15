#ifndef spi_interface_h
#define spi_interface_h

#include "driverlib.h"
#include "delay.h"

#ifdef KIT
#define SPI_PORT EUSCI_B0_BASE
#else
#define SPI_PORT EUSCI_A0_BASE
#endif

void initSPI(void);

#endif
