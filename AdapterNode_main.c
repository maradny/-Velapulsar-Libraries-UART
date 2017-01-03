/*
 * Description: Adapter Node main implementation
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
/* Application Includes */
#include "velapulsar.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 *                             LOCAL FUNCTIONS
 *****************************************************************************/

/******************************************************************************
* Function: Main
****************
* Main program entry point
*
* Parameters: Null
*
* Returns: Null
******************************************************************************/
int main(void)
{
	/* Set clocks to pre defined frequencies */
	initClocks();

    /* Initialize all peripherals */
    initPeripherals();
    
    uint8_t packetNum = 0;
    
    while(1){
        printf("Sending to sink\n");
        
        uint8_t radioPacket[20] = "Hello World #      ";
        
        sprintf(radioPacket+13, "%d", packetNum++);
        printf("Sending "); printf(radioPacket); printf("\n");
        radioPacket[19] = 0;
        
        printf("Sending....\n"); delay_ms(10);
        sendBuffer(radioPacket, 20);
        
        printf("Waiting for packet to complete...\n"); //delay_ms(10);
        waitPacketSent();
        
        uint8_t buf[MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        
        setModeRx();
        printf("Waiting for reply...\n"); //delay_ms(10);
        delay_ms(1000);
        if (available()){
            receiveBuffer(buf, &len);
            printf("Got reply: "); printf(buf); printf("\n");
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, 0x01 << buf[0]);
            //sendBuffer(radioPacket, 20);
            //waitPacketSent();
            //printf("RSSI: "); printf()
        }
        else{
            printf("No reply, is there a listener around?\n");
        }
    	setModeIdle();
		GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
		printf("going to bed\n");
		sleepFor(1);
		GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    }
    
    PCM_gotoLPM0();
    __no_operation();
}


