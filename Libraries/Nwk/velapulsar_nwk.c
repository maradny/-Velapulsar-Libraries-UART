/*
 * Description: Network layer interface
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
#include "velapulsar_nwk.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
static nwkCallbacks* nwkEvents;
static macCallbacks macEvents;

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
/*!
 * \brief Resets MAC specific parameters to default
 */
static void ResetNwkParameters( void );

static void OnMacTxDone (bool ack);
static void OnMacRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
static void OnMacRxError (void);
static void OnMacTxTimeout (void);
static void OnMacRxTimeout (uint16_t timeout);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
VelaMacStatus VelaNwkInitialization(nwkCallbacks* callbacks, uint8_t linkID){
	nwkEvents = callbacks;

	macEvents.MacTxDone = OnMacTxDone;
	macEvents.MacRxDone = OnMacRxDone;
	macEvents.MacRxError = OnMacRxError;
	macEvents.MacTxTimeout = OnMacTxTimeout;
	macEvents.MacRxTimeout = OnMacRxTimeout;

	return VelaMacInitialization(linkID, &macEvents);
}

VelaMacStatus VelaNwkSend (uint8_t linkID, uint8_t appPayload[], int size){
	nwkDataPkt pkt;
	pkt.data.myShovel = MY_SHOVEL;
	pkt.data.myType = MY_TYPE;
	pkt.data.myUnit = MY_UNIT;

	memcpy(&pkt.data.appPayload, appPayload, size);

	printf("NWK sending: ");
	debug_print_pkt(pkt.pkt , sizeof(pkt.pkt) - (MAX_APP_HEADER -size));
	return VelaMacSend(linkID, pkt.pkt, sizeof(pkt.pkt) - (MAX_APP_HEADER -size));
}
/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void OnMacTxDone (bool ack){
	//printf("Sent OK NWK\n");
	nwkEvents->NwkTxDone(ack);
}

static void OnMacRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
	nwkDataPkt pkt;
	memcpy (&pkt.pkt, payload, size);
//	printf("Pkt received (NWK): ");
//	debug_print_pkt(pkt.pkt , size);
//	printf("Message received (NWK) %d: ", size);
//	debug_print_pkt(payload , size);
	nwkEvents->NwkRxDone(pkt.data.appPayload,
			size - sizeof pkt.data.myShovel - sizeof pkt.data.myType -
			sizeof pkt.data.myUnit, rssi, snr);
}

static void OnMacRxError (void){
	printf("Received error Pkt NWK\n");
	nwkEvents->NwkRxError();
}

static void OnMacTxTimeout (void){
	printf("TX timeout nwk\n");
	nwkEvents->NwkTxTimeout();
}

static void OnMacRxTimeout (uint16_t timeout){
	printf("RX timeout nwk\n");
	nwkEvents->NwkRxTimeout(timeout);
}
