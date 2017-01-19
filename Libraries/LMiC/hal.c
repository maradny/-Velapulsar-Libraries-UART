/*
 * Description: Hardware abstraction layer implementation
 * Maintainer: Galal Hassan
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
#include "hal.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
static const u1_t outputpins[] = {NSS_PORT, NSS_PIN, RST_PORT, RST_PIN};
static const u1_t inputpins[] = {DIO0_PORT, DIO0_PIN, DIO1_PORT, DIO1_PIN, DIO2_PORT, DIO2_PIN};

// HAL state
static struct{
	int irqlevel;
	u4_t ticks;
};

/* SPI Master Configuration Parameter */
const eUSCI_SPI_MasterConfig spiMasterConfig =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // SMCLK Clock Source
        3000000,                                   // SMCLK = DCO = 3MHZ
        1000000,                                    // SPICLK = 5Mhz
        EUSCI_B_SPI_MSB_FIRST,                     // MSB First
        EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW, // High polarity
        EUSCI_B_SPI_3PIN                           // 3Wire SPI Mode
};

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
static void hal_io_init(){

	u1_t i;
	// configure output lines and set as low
	for (i=0; i<sizeof(outputpins); i+=2){
		GPIO_setAsOutputPin(outputpins[i], outputpins[i+1]);
		GPIO_setOutputLowOnPin(outputpins[i], outputpins[i+1]);
	}

	// configure output lines and set as low
	for (i=0; i<sizeof(inputpins); i+=2){
		GPIO_setAsInputPinWithPullDownResistor(inputpins[i], inputpins[i+1]);
		GPIO_clearInterruptFlag(inputpins[i], inputpins[i+1]);
		GPIO_interruptEdgeSelect(inputpins[i], inputpins[i+1], GPIO_LOW_TO_HIGH_TRANSITION);
		GPIO_enableInterrupt(inputpins[i], inputpins[i+1]);

		MAP_Interrupt_setPriority(inputpins[i] + 50, 0x40);
		MAP_Interrupt_enableInterrupt(inputpins[i] +50);
	}
	MAP_Interrupt_enableMaster();
}

void hal_pin_rxtx (u1_t val){

}

// set radio RST pin to value
void hal_pin_rst (u1_t val){
	if (val == 0){
		GPIO_setOutputLowOnPin(RST_PORT, RST_PIN);
	}
	else{
		GPIO_setOutputHighOnPin(RST_PORT, RST_PIN);
	}
}

extern void radio_irq_handler (u1_t dio);

// generic EXTI IRQ handler for all channels
void EXTI_IRQHandler (){
	uint32_t status;
	status = MAP_GPIO_getEnabledInterruptStatus(DIO0_PORT);

	// DIO 0
	if (status == DIO0_PIN){
		MAP_GPIO_clearInterruptFlag(DIO0_PORT, DIO0_PIN);
		radio_irq_handler(0);
	}

	// DIO 1
	if (status == DIO1_PIN){
		MAP_GPIO_clearInterruptFlag(DIO1_PORT, DIO1_PIN);
		radio_irq_handler(1);
	}

	// DIO 2
	if (status == DIO2_PIN){
		MAP_GPIO_clearInterruptFlag(DIO2_PORT, DIO2_PIN);
		radio_irq_handler(2);
	}
}

u1_t hal_spi (u1_t out){
	u1_t val = 0;
	SPI_transmitData (EUSCI_B0_BASE, out);
	SPI_transmitData(EUSCI_B0_BASE, 0);
	delay_ms(5);
	val = SPI_receiveData(EUSCI_B0_BASE);
	return val;
}

/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void hal_spi_init(){
    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(SCK_PORT,
            SCK_PIN | MISO_PIN | MOSI_PIN, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring SPI in 3-wire master mode & enabling it & interrupts */
    SPI_initMaster(EUSCI_B0_BASE, &spiMasterConfig);
    SPI_enableModule(EUSCI_B0_BASE);
}

static void hal_time_init(){
	/* Configuring pins for peripheral/crystal usage */
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
			GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

	/* Just in case the user wants to use the getACLK, getMCLK, etc. functions,
	 * let's set the clock frequency in the code.
	 */
	CS_setExternalClockSourceFrequency(32000,CLK_FREQ);

	/* Starting HFXT in non-bypass mode without a timeout. Before we start
	 * we have to change VCORE to 1 to support the 48MHz frequency
	 */
	MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
	MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
	MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
	CS_startHFXT(false);

	/* Initializing the clock source as follows:
	 *      MCLK = HFXTCLK/1 = 48MHz
	 *      ACLK = REFO/2 = 16kHz
	 *      HSMCLK = DCO/2 = 1.5Mhz
	 *      SMCLK = DCO/4 = 750kHz
	 *      BCLK  = REFO = 32kHz
	 */
	MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_8);
	// MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);
	// MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	// MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

	/*
	 *  Getting all of the frequency values of the CLK sources using the
	 * convenience functions */
	// aclk = CS_getACLK();
	// mclk = CS_getMCLK();
	// smclk = CS_getSMCLK();
	// hsmclk = CS_getHSMCLK();
	// bclk = CS_getBCLK();

	Init_SysTick(CLK_FREQ);
}

