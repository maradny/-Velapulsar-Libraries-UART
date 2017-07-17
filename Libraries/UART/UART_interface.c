/*
 * Description: UART interface implementation for Velapulsar
 * Maintainer: Mohamed Maradny
 *
 *	____   ____     .__                      .__
 *	\   \ /   /____ |  | _____  ______  __ __|  |   ___________ _______
 *	 \   Y   // __ \|  | \__  \ \____ \|  |  \  |  /  ___/\__  \\_  __ \
 *	  \     /\  ___/|  |__/ __ \|  |_> >  |  /  |__\___ \  / __ \|  | \/
 *	   \___/  \___  >____(____  /   __/|____/|____/____  >(____  /__|
 *				  \/          \/|__|                   \/      \/
 *				  (C)2017 TRL Queen's University
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * Copyright 2015 Queen's University TRL. All rights reserved.
 * Should you have any questions regarding your right to use this Software,
 * contact TRL at www.queenstrl.ca.
 */

/*****************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include "UART_interface.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
volatile uint8_t UARTBuffer [256];
volatile int rxAvailable = 0;
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
#ifdef SNIFFER
		78,//9600                                  // BRDIV = 78
		2,//9600                                       // UCxBRF = 2
		0,//9600                                       // UCxBRS = 0
#else
        6,//19,//78,//9600                                  // BRDIV = 78
        8,//8,//2,//9600                                       // UCxBRF = 2
        0x20,//0xAA,//9600                                       // UCxBRS = 0
#endif
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

/************************************************************* ****************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
void UARTA0_Handler(void);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
void UARTinit (void){
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
			GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

	/* Setting DCO to 12MHz */
	CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

	/* Configuring UART Module */
	MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

	/* Enable UART module */
	MAP_UART_enableModule(EUSCI_A0_BASE);

	/* Enabling interrupts */
	MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
	//MAP_Interrupt_enableSleepOnIsrExit();
	MAP_Interrupt_enableMaster();
}

void UARTSend (uint8_t *data, int size){
	// Send header
	MAP_UART_transmitData (EUSCI_A0_BASE, USTART_CHAR); // UART start char
	//MAP_UART_transmitData (EUSCI_A0_BASE, USTART_CHAR); // UART start char

	// Send data
	int i =0;
	uint8_t byte[2];
	for (i ; i < size ; i++){
		sprintf(byte, "%02x", data[i]);
		printf("byte: %s", byte);
		MAP_UART_transmitData(EUSCI_A0_BASE, byte[0]);
		//MAP_UART_transmitData(EUSCI_A0_BASE, data[i]);
		if (byte[1] != 0x00){
			MAP_UART_transmitData(EUSCI_A0_BASE, byte[1]);
		}
		MAP_UART_transmitData(EUSCI_A0_BASE, ' ');
		//printf("sending: %x\n", data[i]);
	}

	// Send footer
	MAP_UART_transmitData (EUSCI_A0_BASE, UEND_CHAR); // UART end char
	//MAP_UART_transmitData (EUSCI_A0_BASE, UEND_CHAR); // UART end char
	MAP_UART_transmitData (EUSCI_A0_BASE, '\n'); // UART end char
}

void UARTSendRssi (uint8_t *data, int size, int16_t rssi){
	// Send header
	MAP_UART_transmitData (EUSCI_A0_BASE, USTART_CHAR); // UART start char
	//MAP_UART_transmitData (EUSCI_A0_BASE, USTART_CHAR); // UART start char

	// Send data
	int i =0;
	uint8_t byte[2];
	for (i ; i < size ; i++){
		//sprintf(byte, "%02x", data[i]);
		//printf("byte: %s", byte);
		//MAP_UART_transmitData(EUSCI_A0_BASE, byte[0]);
		MAP_UART_transmitData(EUSCI_A0_BASE, data[i]);
//		if (byte[1] != 0x00){
//			MAP_UART_transmitData(EUSCI_A0_BASE, byte[1]);
//		}

		//printf("sending: %x\n", data[i]);
	}

	MAP_UART_transmitData (EUSCI_A0_BASE, rssi & 0xFF); //LSB of rssi
	MAP_UART_transmitData (EUSCI_A0_BASE, (rssi & 0xFF00) >> 8); //MSB of rssi

	// Send footer
	MAP_UART_transmitData (EUSCI_A0_BASE, UEND_CHAR); // UART end char
	//MAP_UART_transmitData (EUSCI_A0_BASE, UEND_CHAR); // UART end char
	//MAP_UART_transmitData (EUSCI_A0_BASE, '\n'); // UART end char
}

/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
void UARTA0_Handler(void){
	uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
	uint8_t tempChar;

	MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

	if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
		tempChar = MAP_UART_receiveData(EUSCI_A0_BASE);
		int i =0;
		switch (tempChar){
		case USTART_CHAR:
			rxAvailable = 0;
			break;

		case UEND_CHAR:
			UARTrx(UARTBuffer, rxAvailable);
			for (i;i<rxAvailable;i++)
				printf("buffer rx is: %x\n", UARTBuffer[i]);
			i=0;
			break;

		default:
			UARTBuffer[rxAvailable++] = tempChar;
			break;
		}
	}
}
