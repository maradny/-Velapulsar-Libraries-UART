#include "spi_interface.h"

///* SPI Master Configuration Parameter */
//const eUSCI_SPI_MasterConfig spiMasterConfig =
//{
//        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
//        3000000,                                   // SMCLK = DCO = 3MHZ
//        1000000,                                    // SPICLK = 5Mhz
//        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
//        EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,    // Phase
//        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW, // High polarity
//        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
//};
//
//
void initSPI(void){
    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
//    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
//            GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
//
//    /* Configuring SPI in 3-wire master mode & enabling it & interrupts */
//    SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);
//    SPI_enableModule(EUSCI_B0_BASE);
}
