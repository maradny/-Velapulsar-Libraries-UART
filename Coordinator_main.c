/*
 * -------------------------------------------
 *    MSP432 DriverLib - v3_21_00_05
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author:
*******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"

/* Application Includes */
#include <rf95.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines */
#define CLK_FREQ 48000000

void initClocks (void){

    /* Configuring pins for peripheral/crystal usage and LED for output */
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Just in case the user wants to use the getACLK, getMCLK, etc. functions,
     * let's set the clock frequency in the code.
     */
    CS_setExternalClockSourceFrequency(32000,CLK_FREQ);

        /* Starting HFXT in non-bypass mode without a timeout. Before we start
     * we have to change VCORE to 1 to support the 48MHz frequency */
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
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
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

uint8_t GetPort (uint8_t number){
    return number/10;
}

uint16_t GetPin (uint8_t number){
    /* Isolate the second digit */
    uint8_t temp = number/10;
    temp *= 10;
    number = number-temp;

    /* Calculate the hex number */
    return 0x0001 << number;
}
bool temp;
int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    /* Set clocks to pre defined frequencies */
	initClocks();

    MAP_Interrupt_disableSleepOnIsrExit();

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    if (initRF()){
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        printf("Initialized correctly\n");
    }
    uint8_t radioPacket[2] = {0x00,0xff};
    uint8_t irq_flags;
    while(1){
        if (available()){
            uint8_t buf[MAX_MESSAGE_LEN];
            uint8_t len = sizeof(buf);

            printf("received:");
            printf(buf);
            printf("\n");
            receiveBuffer(buf, &len);

            if (radioPacket[0] < 0x02){
                radioPacket[0]++;
            }
            else{
                radioPacket[0] = 0;
            }
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, 0x01 << radioPacket[0]);
            //delay_ms(100);
            sendBuffer(radioPacket, sizeof(radioPacket));
            waitPacketSent();

            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
    }

    PCM_gotoLPM0();
    __no_operation();
}
