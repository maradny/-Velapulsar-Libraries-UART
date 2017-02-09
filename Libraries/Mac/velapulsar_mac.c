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

// VelaMac upper layer event functions
static VelaMacPrimitives_t *VelaMacPrimitives;

/*
 * Actual device type
 */
static DeviceType_t VelaMacDeviceType;

/*
 * VelaMac internal state
 */
uint32_t VelaMacState = MAC_IDLE;

// VelaMac tx/rx/ operation state
VelaMacFlags_t VelaMacFlags;

/*
 * Number of trials for the Join Request
 */
static uint16_t JoinRequestTrials;

/*!
 * VelaMac parameters
 */
VelaMacParams_t VelaMacParams;

/*!
 * VelaMac default parameters
 */
VelaMacParams_t VelaMacParamsDefaults;

/*!
 * Indicates if the MAC layer has already joined a network.
 */
static bool IsVelaMacNetworkJoined = false;

/*!
 * Data rates table definition
 */
const uint8_t Datarates[]  = { 10, 9, 8,  7,  8,  0,  0, 0, 12, 11, 10, 9, 8, 7, 0, 0 };

/*!
 * Up/Down link data rates offset definition
 */
const int8_t datarateOffsets[16][4] =
{
    { DR_10, DR_9 , DR_8 , DR_8  }, // DR_0
    { DR_11, DR_10, DR_9 , DR_8  }, // DR_1
    { DR_12, DR_11, DR_10, DR_9  }, // DR_2
    { DR_13, DR_12, DR_11, DR_10 }, // DR_3
    { DR_13, DR_13, DR_12, DR_11 }, // DR_4
};

/*!
 * VelaMAC channels
 */
static ChannelParams_t Channels[VELA_MAX_NB_CHANNELS];

/*!
 * Contains the channels which remain to be applied.
 */
static uint16_t ChannelsMaskRemaining[6];

/*!
 * Multicast channels linked list
 */
static MulticastParams_t *MulticastChannels = 0;

/*!
 * VelaMAC frame counter. Each time a packet is sent the counter is incremented.
 * Only the 16 LSB bits are sent
 */
static uint32_t UpLinkCounter = 1;

/*!
 * VelaMAC frame counter. Each time a packet is received the counter is incremented.
 * Only the 16 LSB bits are received
 */
static uint32_t DownLinkCounter = 0;

/*!
 * Counts the number of missed ADR acknowledgements
 */
static uint32_t AdrAckCounter = 0;

/*!
 * Uplink messages repetitions counter
 */
static uint8_t ChannelsNbRepCounter = 0;

/*!
 * Number of trials to get a frame acknowledged
 */
static uint8_t AckTimeoutRetries = 1;

/*!
 * Number of trials to get a frame acknowledged
 */
static uint8_t AckTimeoutRetriesCounter = 1;

/*!
 * Indicates if the AckTimeout timer has expired or not
 */
static bool AckTimeoutRetry = false;

/*!
 * Maximum duty cycle
 * \remark Possibility to shutdown the device.
 */
static uint8_t MaxDCycle = 0;

/*!
 * Contains the current MacCommandsBuffer index
 */
static uint8_t MacCommandsBufferIndex = 0;

/*!
 * Contains the current MacCommandsBuffer index for MAC commands to repeat
 */
static uint8_t MacCommandsBufferToRepeatIndex = 0;

/*!
 * Used for test purposes. Disables the opening of the reception windows.
 */
static bool IsRxWindowsEnabled = true;

/*!
 * Indicates if the MAC layer wants to send MAC commands
 */
static bool MacCommandsInNextTx = false;

/*!
 * Current channel index
 */
static uint8_t Channel;

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
VelaMacStatus_t VelaMacInitialization (VelaMacPrimitives_t *primitives){
	if (primitives == 0){
		return VELAMAC_STATUS_PARAMETER_INVALID;
	}

	if ( (primitives->MacMcpsConfirm == 0) ||
		 (primitives->MacMcpsIndication ==0) ||
		 (primitives->MacMlmeConfirm == 0) ){
		return VELAMAC_STATUS_PARAMETER_INVALID;
	}
	char x = LINK_CHAR;
	printf ("x is: %x\n", x);
	VelaMacPrimitives = primitives;

	VelaMacFlags.Value = 0;

	VelaMacDeviceType = TYPE_VELA_ADAPTER;
	VelaMacState = MAC_IDLE;

	JoinRequestTrials = 0;

	VelaMacParamsDefaults.ChannelsTxPower = VELAMAC_DEFAULT_TX_POWER;
	VelaMacParamsDefaults.ChannelsDatarate = VELAMAC_DEFAULT_DATARATE;

	VelaMacParamsDefaults.MaxRxWindow = MAX_RX_WINDOW;
	VelaMacParamsDefaults.ReceiveDelay1 = RECEIVE_DELAY1;
	VelaMacParamsDefaults.ReceiveDelay2 = RECEIVE_DELAY2;
	VelaMacParamsDefaults.JoinAcceptDelay1 = JOIN_ACCEPT_DELAY1;
	VelaMacParamsDefaults.JoinAcceptDelay2 = JOIN_ACCEPT_DELAY2;

	VelaMacParamsDefaults.ChannelsNbRep = 1;
	VelaMacParamsDefaults.Rx1DrOffset = 0;

	VelaMacParamsDefaults.Rx2Channel = (Rx2ChannelParams_t)RX_WND_2_CHANNEL;

	VelaMacParamsDefaults.ChannelsMask[0] = 0xFFFF;
	VelaMacParamsDefaults.ChannelsMask[1] = 0xFFFF;
	VelaMacParamsDefaults.ChannelsMask[2] = 0xFFFF;
	VelaMacParamsDefaults.ChannelsMask[3] = 0xFFFF;
	VelaMacParamsDefaults.ChannelsMask[4] = 0x00FF;
	VelaMacParamsDefaults.ChannelsMask[5] = 0x0000;

	uint8_t i;
	// 125 kHz channels
	for( i = 0; i < VELA_MAX_NB_CHANNELS - 8; i++ ){
		Channels[i].Frequency = 902.3e6 + i * 200e3;
		Channels[i].DrRange.Value = ( DR_3 << 4 ) | DR_0;
		Channels[i].Band = 0;
	}
	// 500 kHz channels
	for( i = VELA_MAX_NB_CHANNELS - 8; i < VELA_MAX_NB_CHANNELS; i++ ){
		Channels[i].Frequency = 903.0e6 + ( i - ( VELA_MAX_NB_CHANNELS - 8 ) ) * 1.6e6;
		Channels[i].DrRange.Value = ( DR_4 << 4 ) | DR_4;
		Channels[i].Band = 0;
	}

	ResetMacParameters();

	// Initialize timers here.....

	// Initialize radio
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

/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void OnRadioTxDone (void){
	printf("Sent OK\n");

}

static void OnRadioRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
	printf("Message received: %s\n", payload);
}

static void OnRadioRxError (void){
	printf("Received error Pkt\n");
}

static void OnRadioTxTimeout (void){
	printf("TX timeout\n");
}

static void OnRadioRxTimeout (void){
	printf("RX timeout\n");
}

static void ResetMacParameters (void){
	IsVelaMacNetworkJoined = false;

	//Counters
	UpLinkCounter = 1;
	DownLinkCounter = 0;
	AdrAckCounter = 0;

	ChannelsNbRepCounter = 0;

	AckTimeoutRetries = 1;
	AckTimeoutRetriesCounter = 1;
	AckTimeoutRetry = false;

	MaxDCycle = 0;

	MacCommandsBufferIndex = 0;
	MacCommandsBufferToRepeatIndex = 0;

	IsRxWindowsEnabled = true;

	VelaMacParams.ChannelsTxPower = VelaMacParamsDefaults.ChannelsTxPower;
	VelaMacParams.ChannelsDatarate = VelaMacParamsDefaults.ChannelsDatarate;

	VelaMacParams.MaxRxWindow = VelaMacParamsDefaults.MaxRxWindow;
	VelaMacParams.ReceiveDelay1 = VelaMacParamsDefaults.ReceiveDelay1;
	VelaMacParams.ReceiveDelay2 = VelaMacParamsDefaults.ReceiveDelay2;
	VelaMacParams.JoinAcceptDelay1 = VelaMacParamsDefaults.JoinAcceptDelay1;
	VelaMacParams.JoinAcceptDelay2 = VelaMacParamsDefaults.JoinAcceptDelay2;

	VelaMacParams.Rx1DrOffset = VelaMacParamsDefaults.Rx1DrOffset;
	VelaMacParams.ChannelsNbRep = VelaMacParamsDefaults.ChannelsNbRep;

	VelaMacParams.Rx2Channel = VelaMacParamsDefaults.Rx2Channel;

	memcpy( ( uint8_t* ) VelaMacParams.ChannelsMask, ( uint8_t* ) VelaMacParamsDefaults.ChannelsMask, sizeof( VelaMacParams.ChannelsMask ) );
	memcpy( ( uint8_t* ) ChannelsMaskRemaining, ( uint8_t* ) VelaMacParamsDefaults.ChannelsMask, sizeof( VelaMacParams.ChannelsMask ) );

	MacCommandsInNextTx = false;

	// Reset Multicast downlink counters
	MulticastParams_t *cur = MulticastChannels;
	while( cur != 0 )
	{
		cur->DownLinkCounter = 0;
		cur = cur->Next;
	}

	// Initialize channel index.
	Channel = VELA_MAX_NB_CHANNELS;
}
