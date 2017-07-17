/*
 * Description: MAC layer interface
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
#include "velapulsar_mac.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/

/*
 * VelaMac internal states
 */
typedef enum{
    MAC_TX,
    MAC_RX
}VelaMacStates;

static RadioEvents_t radioEvents;
static macCallbacks* macEvents;

static volatile VelaMacStates currentState;

volatile uint16_t pktID;

volatile dataPkt rxPkt;
volatile int16_t rxRssi;
volatile int8_t rxSnr;
volatile uint16_t rxSize;

volatile dataPkt txPkt;
volatile uint16_t txSize;
volatile uint8_t numOfFailed = 0;
volatile bool waitForAck = false;

/*!
 * Indicates if the MAC layer has already joined a network.
 */
static bool IsVelaMacNetworkJoined = false;

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
/*!
 * \brief Resets MAC specific parameters to default
 */
static void ResetMacParameters( void );

static void OnRadioTxDone (bool ack);
static void OnRadioRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
static void OnRadioRxError (void);
static void OnRadioTxTimeout (void);
static void OnRadioRxTimeout (uint16_t timeout);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
VelaMacStatus VelaMacInitialization (uint8_t linkID, macCallbacks* callbacks){
	macEvents = callbacks; // callbacks back to nwk layer
	ResetMacParameters();

	pktID = 0;
	// Initialize timers here.....

	// Initialize radio callbacks
	radioEvents.TxDone = OnRadioTxDone;
	radioEvents.RxDone = OnRadioRxDone;
	radioEvents.RxError = OnRadioRxError;
	radioEvents.TxTimeout = OnRadioTxTimeout;
	radioEvents.RxTimeout = OnRadioRxTimeout;
	if (!RFInit(&radioEvents)){
		return VELAMAC_STATUS_DEVICE_OFF;
	}

	return VELAMAC_STATUS_OK;
}

VelaMacStatus VelaMacSend (uint8_t linkID, uint8_t nwkPayload[], int size){
	currentState = MAC_TX;
	dataPkt pkt;
	pkt.data.myAddr = MY_ADDR;
	pkt.data.toAddr = 0x00;
	pkt.data.msgType = COMMAND;
	pkt.data.pktID = pktID;
	memcpy(&pkt.data.nwkPayload, nwkPayload, size);

	memcpy (&txPkt.pkt, pkt.pkt, size + MAX_MAC_HEADER);
	txSize = size + MAX_MAC_HEADER;

	printf("size rx in mac: %d", size);
	//RFSetTxConfig(23, 0, 12,1, 10, true, false, 1000);
	RFSetTxConfig(23, 9, 12,1, 20, false, true, 1000);
	printf("MAC sending: ");
	debug_print_pkt(pkt.pkt , size + MAX_MAC_HEADER);
	printf("size of pkt.pkt: %d\n", sizeof(pkt.pkt));
	printf("size of pkt im sending: %d\n", size + MAX_MAC_HEADER);
	RFSend(pkt.pkt, size + MAX_MAC_HEADER);
	if (pktID < 65535)
		pktID++;
	else
		pktID = 0;
	return VELAMAC_STATUS_OK;
}
/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void ResetMacParameters (void){
	IsVelaMacNetworkJoined = false;
}

static void OnRadioTxDone (bool ack){
	printf("Sent OK MAC\n");
	//macEvents->MacTxDone(ack);

#ifdef PKT_ACK
	if (ack == true){
		printf ("sent ack\n");
		//RFSetRxConfig(9, 12,1, 50, REPEAT_TIMEOUT, false, 20, false, false);

//	#ifdef COORDINATOR
//		//UARTSend(rxPkt.pkt, rxSize);
//		UARTSendRssi(rxPkt.pkt, rxSize, rxRssi);
//		//SPISend(SERVER, rxPkt.pkt, rxSize);
//		RFSetRxConfig(9, 12,1, 50, 1000, false, 20, true, true);
//		RFSetRx(1000);
//	#endif
		macEvents->MacRxDone(rxPkt.data.nwkPayload,
					rxSize - sizeof rxPkt.data.msgType - sizeof rxPkt.data.myAddr -
							sizeof rxPkt.data.pktID -
							sizeof rxPkt.data.toAddr, rxRssi, rxSnr);
	}
	else{
		printf ("sent pkt\n");
		waitForAck = true;
		RFSetRxConfig(9, 12,1, 30, ACK_TIMEOUT, false, 20, true, false);
		RFSetRx(1000);
	}

#endif
}

static void OnRadioRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
	//printf("Message received (MAC): ");
	//debug_print_pkt(payload , size);
	dataPkt pkt;
	memcpy (&pkt.pkt, payload, size);
	memcpy (&rxPkt.pkt, payload, size);
	rxSize = size;
	rxRssi = rssi;
	rxSnr = snr;
	//printf("Pkt received (MAC): ");
	//debug_print_pkt(pkt.pkt , size);

#if defined(COORDINATOR)
	UARTSendRssi(rxPkt.pkt, rxSize, rxRssi);
#elif defined(SNIFFER)
	UARTSend(rxPkt.pkt, rxSize);
#endif

#ifdef PKT_ACK
	#ifndef SNIFFER
		if (rxPkt.data.msgType != ACKNOWLEDGE && !waitForAck){
			printf("sending ack\n");
			// received a new packet
			SendAck();
		}
		else if(rxPkt.data.msgType == ACKNOWLEDGE){
			// received an ack for the msg i sent
			printf("received ack\n");
			waitForAck = false;
			numOfFailed = 0;
			macEvents->MacTxDone(true);
		}
	//	else {
	//		macEvents->MacTxDone(false);
	//	}
	#else
		macEvents->MacRxDone(rxPkt.data.nwkPayload,
					rxSize - sizeof rxPkt.data.msgType - sizeof rxPkt.data.myAddr -
							sizeof rxPkt.data.pktID -
							sizeof rxPkt.data.toAddr, rxRssi, rxSnr);
	#endif
#endif
}

void SendAck(void){
	dataPkt ackPkt = rxPkt;
	ackPkt.data.toAddr = rxPkt.data.myAddr;
	ackPkt.data.myAddr = MY_ADDR;
	ackPkt.data.msgType = ACKNOWLEDGE;

	//RFSetTxConfig(23, 0, 12,1, 10, true, false, 1000);
	RFSetTxConfig(23, 9, 12,1, 20, false, true, 5555); // timeout is 5555 to signify transmitting ack
	printf("MAC sending ack: ");
	debug_print_pkt(ackPkt.pkt , MAX_MAC_HEADER);// rxSize);
	printf("Time on air: %d\n", RFGetTimeOnAir(MAX_MAC_HEADER));
	printf("size of ackPkt.pkt: %d\n", sizeof(ackPkt.pkt));
	printf("size of ackPkt im sending: %d\n", MAX_MAC_HEADER);//rxSize);
	RFSend(ackPkt.pkt,MAX_MAC_HEADER);//rxSize);
}

static void OnRadioRxError (void){
	printf("Received error Pkt MAC\n");
	macEvents->MacRxError();
}

static void OnRadioTxTimeout (void){
	printf("TX timeout mac\n");
	macEvents->MacTxTimeout();
}

static void OnRadioRxTimeout (uint16_t timeout){
	printf("RX timeout mac\n");

	switch (timeout){
	case REPEAT_TIMEOUT:
		// forward received msg
//	#ifdef COORDINATOR
//		UARTSend(rxPkt.pkt, rxSize);
//		RFSetRxConfig(9, 12,1, 50, 1000, false, 20, false, true);
//		RFSetRx(1000);
//	#endif
//		macEvents->MacRxDone(rxPkt.data.nwkPayload,
//					rxSize - sizeof rxPkt.data.msgType - sizeof rxPkt.data.myAddr -
//							sizeof rxPkt.data.pktID -
//							sizeof rxPkt.data.toAddr, rxRssi, rxSnr);
		break;

	case ACK_TIMEOUT:
		// notify sent not ack
		if (numOfFailed < MAX_NUM_FAILED){
			numOfFailed++;
			RFSetTxConfig(23, 9, 12,1, 20, false, true, 1000);
			RFSend(txPkt.pkt,txSize);
		}
		else{
			numOfFailed = 0;
			macEvents->MacTxDone(false);
		}
		break;

	case RX_WINDOW:
		// back to normal
		break;
	}
	//macEvents->MacRxTimeout(timeout);
}
