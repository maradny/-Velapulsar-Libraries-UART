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

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
/*!
 * \brief Resets MAC specific parameters to default
 */
static void ResetParameters( void );

static void OnTxDone (void);
static void OnRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
static void OnRxError (void);
static void OnTxTimeout (void);
static void OnRxTimeout (void);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
VelaMacStatus CommsSend(uint8_t linkID, appDataPkt pkt){// change later after implementing mac layer
	printf("APP sending: ");
	debug_print_pkt(pkt.pkt , sizeof(pkt.pkt));
	VelaNwkSend (linkID, pkt.pkt, sizeof(pkt.pkt));
}

VelaMacStatus CommsInit(uint8_t linkID){
		nwkEvents.NwkTxDone = OnTxDone;
		nwkEvents.NwkRxDone = OnRxDone;
		nwkEvents.NwkRxError = OnRxError;
		nwkEvents.NwkTxTimeout = OnTxTimeout;
		nwkEvents.NwkRxTimeout = OnTxTimeout;

		return VelaNwkInitialization(&nwkEvents, linkID);
}

/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void ResetParameters (void){
	//IsVelaMacNetworkJoined = false;
}

static void OnTxDone (void){
	printf("Sent OK APP\n");
}

static void OnRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
	printf("Message received (APP): ");
	debug_print_pkt(payload , size);
}

static void OnRxError (void){
	printf("Received error Pkt\n");
}

static void OnTxTimeout (void){
	printf("TX timeout\n");
}

static void OnRxTimeout (void){
	printf("RX timeout\n");
}
