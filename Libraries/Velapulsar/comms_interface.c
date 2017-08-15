/*
 * Description: Communications interface implementation for Velapulsar
 * 				(application layer)
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
#include "comms_interface.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
static nwkCallbacks nwkEvents;
static volatile bool txFlag = false;
static volatile bool isCoordinator = false;

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
/*!
 * \brief Resets MAC specific parameters to default
 */
static void ResetParameters( void );

static void OnTxDone (bool ack);
static void OnRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
static void OnRxError (void);
static void OnTxTimeout (void);
static void OnRxTimeout (uint16_t timeout);
void ReceivedCmd(uint8_t *data, int size);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
VelaMacStatus CommsSend(uint8_t linkID, appDataPkt pkt){// change later after implementing mac layer
	printf("APP sending: ");
	debug_print_pkt(pkt.pkt , sizeof(pkt.pkt));
	VelaNwkSend (linkID, pkt.pkt, sizeof(pkt.pkt));
	while (!txFlag);
	txFlag = false;
	return  VELAMAC_SUCCESSFUL;
}

VelaMacStatus CommsInit(uint8_t linkID){
		nwkEvents.NwkTxDone = OnTxDone;
		nwkEvents.NwkRxDone = OnRxDone;
		nwkEvents.NwkRxError = OnRxError;
		nwkEvents.NwkTxTimeout = OnTxTimeout;
		nwkEvents.NwkRxTimeout = OnRxTimeout;
#ifdef COORDINATOR
		UARTrx = ReceivedCmd;
#endif
		return VelaNwkInitialization(&nwkEvents, linkID);
}

void CommsStartContinuousRx(void){
	isCoordinator = true;
	//RFSetRxConfig(0, 12,1, 20, 1000000, true, 20, false, true);
	RFSetRxConfig(9, 12,1, 30, 1000, false, 20, true, true);
	RFSetRx(1000);
}
/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void ResetParameters (void){
	//IsVelaMacNetworkJoined = false;
}

static void OnTxDone (bool ack){
	//printf("Sent OK APP\n");
	if (ack){
		printf ("acknowledged\n");
	}
	else{
		printf (" Not acknowledged\n");
	}
	txFlag = true;
}

static void OnRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){

	printf("Message received (APP): %d, ", rssi);
	debug_print_pkt(payload , size);
	appDataPkt pkt;
	memcpy (&pkt.pkt, payload, size);
	PktReceived(0, pkt, rssi);
	if (isCoordinator)
		CommsStartContinuousRx();
}

static void OnRxError (void){
	printf("Received error Pkt APP\n");
	txFlag = true;
	if (isCoordinator)
		CommsStartContinuousRx();
}

static void OnTxTimeout (void){
	printf("TX timeout app\n");
}

static void OnRxTimeout (uint16_t timeout){
	printf("RX timeout app\n");
}

void ReceivedCmd(uint8_t *data, int size){
	//implement commands here
	printf("received command\n");
}
