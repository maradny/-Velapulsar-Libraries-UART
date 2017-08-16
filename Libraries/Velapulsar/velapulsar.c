/*
 * Description: Velapulsar application interface, provides an interface for
 * 				all the application functions
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
#include "velapulsar.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
static RTC_C_Calendar calendarTime;
volatile bool awake = true;

VelaMacStatus Status;

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
void initPeripherals (void){
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

	/* Set Calendar to pre defined date */
	initCalendar();

	/* Initialize ports */
	initPorts();

#if defined (COORDINATOR) || defined (SNIFFER)
	UARTinit();
#endif

	/* Initialize radio */

	printf("initializing\n");
#ifdef NODE
	if (CommsInitNode() == VELAMAC_SUCCESSFUL){
		printf("initialized\n");
		//GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
	}
	else{
		printf("Not initialized\n");
	}
#else
	#ifdef COORDINATOR
		if (CommsInitCoordinator() == VELAMAC_SUCCESSFUL){
			printf("initialized\n");
			//GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
		}
		else{
			printf("Not initialized\n");
		}
	#else
		if (CommsInitSniffer() == VELAMAC_SUCCESSFUL){
			printf("initialized\n");
			//GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
		}
		else{
			printf("Not initialized\n");
		}
	#endif
#endif

}

void initPorts (void){
	MAP_Interrupt_disableSleepOnIsrExit();

//	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
//	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
//
//	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
//	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
//
//	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
//	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
//
//	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
//	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
//
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN2);
	GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);
}

void initClocks (void){
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
#if defined (COORDINATOR) || defined (SNIFFER)
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    // MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    // MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    // MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
#else
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);//CS_CLOCK_DIVIDER_8);
    // MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    // MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    // MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
#endif


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

void sleepFor(int minutes){

	RFSetSleep();
	//setModeSleep();

	RTC_C_Calendar currentTime = RTC_C_getCalendarTime();

    /* Setting alarm for one minute later */
    RTC_C_setCalendarAlarm(currentTime.minutes+minutes,RTC_C_ALARMCONDITION_OFF,RTC_C_ALARMCONDITION_OFF,RTC_C_ALARMCONDITION_OFF);

    RTC_C_clearInterruptFlag(RTC_C_getInterruptStatus());
	RTC_C_enableInterrupt(RTC_C_CLOCK_ALARM_INTERRUPT);
	Interrupt_enableInterrupt(INT_RTC_C);
	Interrupt_enableMaster();

	awake = false;

    /* Starting RTC and going into shutdown */
    RTC_C_startClock();
    //MAP_Interrupt_enableSleepOnIsrExit(); //enabling this messes up entering to sleep mode :/

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, PIN_ALL8);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, PIN_ALL8);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_PB, PIN_ALL16);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_PC, PIN_ALL16);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_PD, PIN_ALL16);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_PE, PIN_ALL16);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PB, PIN_ALL16);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PC, PIN_ALL16);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PD, PIN_ALL16);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_PE, PIN_ALL16);

	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
			GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_CS_startLFXT(false);
	MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_BCLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);

	/* Disabling high side voltage monitor/supervisor */
	MAP_PSS_disableHighSide();

	/* Enabling "rude" mode which forces device to go to sleep regardless of
	 * outstanding clock requests
	 */
	MAP_PCM_enableRudeMode();
//	if (PCM_shutdownDevice(PCM_LPM35_VCORE0)){
    if ( MAP_PCM_gotoLPM3()){
		printf("slept\n");
	}
	else{
		printf("failed to sleep\n");
	}
    while(!awake){
//    	printf("%d\n",RTC_C_getCalendarTime().seconds);
//    	printf("%d\n",getTotal_ms());
//    	printf("sleeping\n");
    }
    printf("woke up\n");
    initPeripherals();
    RFSetStby();
    //setModeIdle();
}

void initCalendar (void){
    /* Configuring LFXTOUT and LFXTIN for XTAL operation */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Starting LFXT */
    CS_startLFXT(CS_LFXT_DRIVE3);

    calendarTime.seconds = 0;
    calendarTime.minutes = 53;
    calendarTime.hours = 16;
    calendarTime.dayOfWeek = 3;
    calendarTime.dayOfmonth = 3;
    calendarTime.month = 1;
    calendarTime.year = 2017;

    RTC_C_initCalendar(&calendarTime, RTC_C_FORMAT_BINARY);
}

void RTC_AlarmHandler(void){
    uint32_t status;

    status = RTC_C_getEnabledInterruptStatus();
    RTC_C_clearInterruptFlag(status);

    if(status & RTC_C_CLOCK_ALARM_INTERRUPT){
    	awake = true;
    }
}


void debug_print_pkt (uint8_t pkt[], int size){
	int i;
	for (i = 0; i < size;i ++){
		printf("0x%02x ", pkt[i]);
	}
	printf ("\n");
}
/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
