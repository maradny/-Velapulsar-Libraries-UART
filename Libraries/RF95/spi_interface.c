#include "spi_interface.h"

/* SPI Master Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
#ifdef KIT
		EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
		3000000,                                   // SMCLK = DCO = 3MHZ
		1000000,                                    // SPICLK = 5Mhz
		EUSCI_B_SPI_MSB_FIRST,                     // MSB First
		EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,    // Phase
		EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW, // High polarity
		EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
#else
		EUSCI_A_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
		3000000,                                   // SMCLK = DCO = 3MHZ
		1000000,                                    // SPICLK = 5Mhz
		EUSCI_A_SPI_MSB_FIRST,                     // MSB First
		EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,    // Phase
		EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW, // High polarity
		EUSCI_A_SPI_3PIN                           // 3Wire SPI Mode
#endif
};


void initSPI(void){
/*
 	1.0 ste
	1.1 clk
	1.2 miso
	1.3 mosi
	1.4 rst
	1.5 dio0
	1.6 dio1
	1.7 dio2
*/
    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
#ifdef KIT
            GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
#else
    		GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
#endif
    
    /* Configuring SPI in 3-wire master mode & enabling it & interrupts */
    SPI_initMaster(SPI_PORT, &spiMasterConfig);
    SPI_enableModule(SPI_PORT);
}
