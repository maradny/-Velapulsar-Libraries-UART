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
 * Maximum PHY layer payload size
 */
#define VELA_MAC_PHY_MAXPAYLOAD                      255

/*
 * Maximum MAC commands buffer size
 */
#define VELA_MAC_COMMAND_MAX_LENGTH                 15

/*
 * FRMPayload overhead to be used when setting the Radio.SetMaxPayloadLength
 * in RxWindowSetup function.
 * Maximum PHYPayload = MaxPayloadOfDatarate/MaxPayloadOfDatarateRepeater + VELA_MAC_FRMPAYLOAD_OVERHEAD
 */
#define VELA_MAC_FRMPAYLOAD_OVERHEAD                13 // MHDR(1) + FHDR(7) + Port(1) + MIC(4)

/*
 * VelaMac internal states
 */
enum eVelaMacState
{
    MAC_IDLE          = 0x00000000,
    MAC_TX_RUNNING    = 0x00000001,
    MAC_RX            = 0x00000002,
    MAC_ACK_REQ       = 0x00000004,
    MAC_ACK_RETRY     = 0x00000008,
    MAC_TX_DELAYED    = 0x00000010,
    MAC_TX_CONFIG     = 0x00000020,
    MAC_RX_ABORT      = 0x00000040,
};

static RadioEvents_t radioEvents;
static macCallbacks* macEvents;

/*
 * Device IEEE EUI
 */
static uint8_t *VelaMacDevEui;

/*
 * Application IEEE EUI
 */
static uint8_t *VelaMacAppEui;

/*
 * AES encryption/decryption cipher application key
 */
static uint8_t *VelaMacAppKey;

/*
 * AES encryption/decryption cipher network session key
 */
static uint8_t VelaMacNwkSKey[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
 * AES encryption/decryption cipher application session key
 */
static uint8_t VelaMacAppSKey[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
 * Device nonce is a random value extracted by issuing a sequence of RSSI
 * measurements
 */
static uint16_t VelaMacDevNonce;

/*
 * Network ID ( 3 bytes )
 */
static uint32_t VelaMacNetID;

/*
 * Device (MAC) Address
 */
static uint32_t VelaMacDevAddr;

/*
 * VelaMac internal state
 */
uint32_t VelaMacState = MAC_IDLE;

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

static void OnRadioTxDone (void);
static void OnRadioRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
static void OnRadioRxError (void);
static void OnRadioTxTimeout (void);
static void OnRadioRxTimeout (void);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
VelaMacStatus VelaMacInitialization (uint8_t linkID, macCallbacks* callbacks){
	macEvents = callbacks; // callbacks back to nwk layer
	ResetMacParameters();

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
	dataPkt pkt;
	pkt.data.myAddr = MY_ADDR;
	pkt.data.toAddr = 0x00;
	pkt.data.msgType = COMMAND;
	pkt.data.pktID = 0;
	pkt.data.myType = 0;
	memcpy(&pkt.data.nwkPayload, nwkPayload, size);
	printf("size rx in mac: %d", size);
	//RFSetTxConfig(23, 0, 12,1, 10, true, false, 1000);
	RFSetTxConfig(23, 9, 10,1, 20, true, false, 1000);
	printf("MAC sending: ");
	debug_print_pkt(pkt.pkt , sizeof(pkt.pkt) - (MAX_NWK_PAYLOAD -size));
	RFSend(pkt.pkt,sizeof(pkt.pkt) - (MAX_NWK_PAYLOAD -size));
}
/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void ResetMacParameters (void){
	IsVelaMacNetworkJoined = false;
}

static void OnRadioTxDone (void){
	printf("Sent OK MAC\n");
	macEvents->MacTxDone();
}

static void OnRadioRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
	printf("Message received (MAC): ");
	debug_print_pkt(payload , size);
	macEvents->MacRxDone(payload, size, rssi, snr);
}

static void OnRadioRxError (void){
	printf("Received error Pkt\n");
	macEvents->MacRxError();
}

static void OnRadioTxTimeout (void){
	printf("TX timeout\n");
	macEvents->MacTxTimeout();
}

static void OnRadioRxTimeout (void){
	printf("RX timeout\n");
	macEvents->MacRxTimeout();
}
