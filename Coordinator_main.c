/*
 * Description: Coordinator Node main implementation
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
 *                                DEFINES
 *****************************************************************************/
static volatile bool cmdAvailable = false;

/*****************************************************************************
 *                             LOCAL FUNCTIONS
 *****************************************************************************/
void ReceivedPkt(uint8_t linkID, appDataPkt pkt, int16_t rssi);
void initSerial(void);
void sendSerial(uint8_t* data, int size);
void Alarm_Went_Off(void);

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

    PktReceived = ReceivedPkt;

    printf("alarm starting\n");
    printf ("Time now: %d\n",getTotal_ms());
    //Set_Alarm (10000, &Alarm_Went_Off);

    while(1){
    	while (!cmdAvailable){

    	}
    }

    PCM_gotoLPM0();
    __no_operation();
}

int _system_pre_init( void )
{
  WDTCTL = WDTPW | WDTHOLD;

  return 1;
}

void Alarm_Went_Off(void){
	printf ("Time now: %d\n",getTotal_ms());
	printf("ALAAAAAAAAAAARM!!!!!\n");
}

void ReceivedPkt(uint8_t linkID, appDataPkt pkt, int16_t rssi){
	//sendSerial("Packet received with rssi:",26);
	char buff[4];
	sprintf(buff, "%d", rssi); // convert values to ascii string
	printf("rssi: %s\n",buff);
	printf("received this: %x, %x, %x, %x\n", pkt.data.sensorData.battery, pkt.data.sensorData.claw, pkt.data.sensorData.light, pkt.data.sensorData.magnetic);
}
