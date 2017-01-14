/*
 * Description: MAC layer header file
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

#ifndef LIBRARIES_MAC_VELAPULSAR_MAC_H_
#define LIBRARIES_MAC_VELAPULSAR_MAC_H_

/*****************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include "rf95.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
/*!
 * LoRaMac maximum number of channels
 */
#define VELA_MAX_NB_CHANNELS                        72

/*!
 * Minimal datarate that can be used by the node
 */
#define VELAMAC_TX_MIN_DATARATE                     DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define VELAMAC_TX_MAX_DATARATE                     DR_4

/*!
 * Minimal datarate that can be used by the node
 */
#define VELAMAC_RX_MIN_DATARATE                     DR_8

/*!
 * Maximal datarate that can be used by the node
 */
#define VELAMAC_RX_MAX_DATARATE                     DR_13

/*!
 * Default datarate used by the node
 */
#define VELAMAC_DEFAULT_DATARATE                    DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define VELAMAC_MIN_RX1_DR_OFFSET                   0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define VELAMAC_MAX_RX1_DR_OFFSET                   3

/*!
 * Minimal Tx output power that can be used by the node
 */
#define VELAMAC_MIN_TX_POWER                        TX_POWER_10_DBM

/*!
 * Maximal Tx output power that can be used by the node
 */
#define VELAMAC_MAX_TX_POWER                        TX_POWER_30_DBM

/*!
 * Default Tx output power used by the node
 */
#define VELAMAC_DEFAULT_TX_POWER                    TX_POWER_20_DBM

/*!
 * LoRaMac TxPower definition
 */
#define TX_POWER_30_DBM                             0
#define TX_POWER_28_DBM                             1
#define TX_POWER_26_DBM                             2
#define TX_POWER_24_DBM                             3
#define TX_POWER_22_DBM                             4
#define TX_POWER_20_DBM                             5
#define TX_POWER_18_DBM                             6
#define TX_POWER_16_DBM                             7
#define TX_POWER_14_DBM                             8
#define TX_POWER_12_DBM                             9
#define TX_POWER_10_DBM                             10

/*!
 * LoRaMac datarates definition
 */
#define DR_0                                        0  // SF10 - BW125 |
#define DR_1                                        1  // SF9  - BW125 |
#define DR_2                                        2  // SF8  - BW125 +-> Up link
#define DR_3                                        3  // SF7  - BW125 |
#define DR_4                                        4  // SF8  - BW500 |
#define DR_5                                        5  // RFU
#define DR_6                                        6  // RFU
#define DR_7                                        7  // RFU
#define DR_8                                        8  // SF12 - BW500 |
#define DR_9                                        9  // SF11 - BW500 |
#define DR_10                                       10 // SF10 - BW500 |
#define DR_11                                       11 // SF9  - BW500 |
#define DR_12                                       12 // SF8  - BW500 +-> Down link
#define DR_13                                       13 // SF7  - BW500 |
#define DR_14                                       14 // RFU          |
#define DR_15                                       15 // RFU          |

/*!
 * Second reception window channel definition.
 */
// Channel = { Frequency [Hz], Datarate }
#define RX_WND_2_CHANNEL                                  { 923300000, DR_8 }

/*!
 * VelaMac maximum number of bands
 */
#define VELA_MAX_NB_BANDS                           1

// Band = { DutyCycle, TxMaxPower, LastTxDoneTime, TimeOff }
#define BAND0              { 1, TX_POWER_20_DBM, 0,  0 } //  100.0 %

#define BACKOFF_RND_OFFSET      600000

/*!
 * LoRaMac default channels
 */
// Channel = { Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
/*
 * US band channels are initialized using a loop in LoRaMacInit function
 * \code
 * // 125 kHz channels
 * for( uint8_t i = 0; i < LORA_MAX_NB_CHANNELS - 8; i++ )
 * {
 *     Channels[i].Frequency = 902.3e6 + i * 200e3;
 *     Channels[i].DrRange.Value = ( DR_3 << 4 ) | DR_0;
 *     Channels[i].Band = 0;
 * }
 * // 500 kHz channels
 * for( uint8_t i = LORA_MAX_NB_CHANNELS - 8; i < LORA_MAX_NB_CHANNELS; i++ )
 * {
 *     Channels[i].Frequency = 903.0e6 + ( i - ( LORA_MAX_NB_CHANNELS - 8 ) ) * 1.6e6;
 *     Channels[i].DrRange.Value = ( DR_4 << 4 ) | DR_4;
 *     Channels[i].Band = 0;
 * }
 */

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
/*!
 * Beacon interval in ms
 */
#define BEACON_INTERVAL                             128000

/*!
 * Class A&B receive delay 1 in ms
 */
#define RECEIVE_DELAY1                              1000

/*!
 * Class A&B receive delay 2 in ms
 */
#define RECEIVE_DELAY2                              2000

/*!
 * Join accept receive delay 1 in ms
 */
#define JOIN_ACCEPT_DELAY1                          5000

/*!
 * Join accept receive delay 2 in ms
 */
#define JOIN_ACCEPT_DELAY2                          6000

/*!
 * Class A&B maximum receive window delay in ms
 */
#define MAX_RX_WINDOW                               3000

/*!
 * Maximum allowed gap for the FCNT field
 */
#define MAX_FCNT_GAP                                16384

/*!
 * ADR acknowledgement counter limit
 */
#define ADR_ACK_LIMIT                               64

/*!
 * Number of ADR acknowledgement requests before returning to default datarate
 */
#define ADR_ACK_DELAY                               32

/*!
 * Number of seconds after the start of the second reception window without
 * receiving an acknowledge.
 * AckTimeout = \ref ACK_TIMEOUT + Random( -\ref ACK_TIMEOUT_RND, \ref ACK_TIMEOUT_RND )
 */
#define ACK_TIMEOUT                                 2000

/*!
 * Random number of seconds after the start of the second reception window without
 * receiving an acknowledge
 * AckTimeout = \ref ACK_TIMEOUT + Random( -\ref ACK_TIMEOUT_RND, \ref ACK_TIMEOUT_RND )
 */
#define ACK_TIMEOUT_RND                             1000

/*!
 * Check the Mac layer state every MAC_STATE_CHECK_TIMEOUT in ms
 */
#define MAC_STATE_CHECK_TIMEOUT                     1000

/*!
 * Maximum number of times the MAC layer tries to get an acknowledge.
 */
#define MAX_ACK_RETRIES                             8

/*
 * RSSI free threshold [dBm]
 */
#define RSSI_FREE_TH                                ( int8_t )( -90 )

/*
 * Frame direction definition for up-link communications
 */
#define UP_LINK                                     0

/*
 * Frame direction definition for down-link communications
 */
#define DOWN_LINK                                   1

/*
 * Sets the length of the LoRaMAC footer field.
 * Mainly indicates the MIC field length
 */
#define VELAMAC_MFR_LEN                             4

// Velapulsar device types definition
typedef enum eDeviceType
{
    TYPE_VELA_SINK,

    TYPE_VELA_ADAPTER,

    TYPE_VELA_TOOTH,
}DeviceType_t;

/*!
 * VelaMAC Status
 */
typedef enum eVelaMacStatus
{
    /*!
     * Service started successfully
     */
    VELAMAC_STATUS_OK,
    /*!
     * Service not started - LoRaMAC is busy
     */
    VELAMAC_STATUS_BUSY,
    /*!
     * Service unknown
     */
    VELAMAC_STATUS_SERVICE_UNKNOWN,
    /*!
     * Service not started - invalid parameter
     */
    VELAMAC_STATUS_PARAMETER_INVALID,
    /*!
     * Service not started - invalid frequency
     */
    VELAMAC_STATUS_FREQUENCY_INVALID,
    /*!
     * Service not started - invalid datarate
     */
    VELAMAC_STATUS_DATARATE_INVALID,
    /*!
     * Service not started - invalid frequency and datarate
     */
    VELAMAC_STATUS_FREQ_AND_DR_INVALID,
    /*!
     * Service not started - the device is not in a LoRaWAN
     */
    VELAMAC_STATUS_NO_NETWORK_JOINED,
    /*!
     * Service not started - playload lenght error
     */
    VELAMAC_STATUS_LENGTH_ERROR,
    /*!
     * Service not started - playload lenght error
     */
    VELAMAC_STATUS_MAC_CMD_LENGTH_ERROR,
    /*!
     * Service not started - the device is switched off
     */
    VELAMAC_STATUS_DEVICE_OFF,
}VelaMacStatus_t;


/*!
 * VelaMac tx/rx operation state
 */
typedef union eVelaMacFlags_t
{
    /*!
     * Byte-access to the bits
     */
    uint8_t Value;
    /*!
     * Structure containing single access to bits
     */
    struct sMacFlagBits
    {
        /*!
         * MCPS-Req pending
         */
        uint8_t McpsReq         : 1;
        /*!
         * MCPS-Ind pending
         */
        uint8_t McpsInd         : 1;
        /*!
         * MLME-Req pending
         */
        uint8_t MlmeReq         : 1;
        /*!
         * MAC cycle done
         */
        uint8_t MacDone         : 1;
    }Bits;
}VelaMacFlags_t;

/*!
 * Enumeration containing the status of the operation of a MAC service
 */
typedef enum eVelaMacEventInfoStatus
{
    /*!
     * Service performed successfully
     */
	VELAMAC_EVENT_INFO_STATUS_OK = 0,
    /*!
     * An error occured during the execution of the service
     */
	VELAMAC_EVENT_INFO_STATUS_ERROR,
    /*!
     * A Tx timeouit occured
     */
	VELAMAC_EVENT_INFO_STATUS_TX_TIMEOUT,
    /*!
     * An Rx timeout occured on receive window 2
     */
	VELAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT,
    /*!
     * An Rx error occured on receive window 2
     */
	VELAMAC_EVENT_INFO_STATUS_RX2_ERROR,
    /*!
     * An error occured in the join procedure
     */
	VELAMAC_EVENT_INFO_STATUS_JOIN_FAIL,
    /*!
     * A frame with an invalid downlink counter was received. The
     * downlink counter of the frame was equal to the local copy
     * of the downlink counter of the node.
     */
	VELAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED,
    /*!
     * The node has lost MAX_FCNT_GAP or more frames.
     */
	VELAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS,
    /*!
     * An address error occured
     */
	VELAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL,
    /*!
     * message integrity check failure
     */
    VELAMAC_EVENT_INFO_STATUS_MIC_FAIL,
}VelaMacEventInfoStatus_t;

/*!
 *
 * \brief   VelaMAC data services
 *
 * \details The following table list the primitives which are supported by the
 *          specific MAC data service:
 *
 * Name                  | Request | Indication | Response | Confirm
 * --------------------- | :-----: | :--------: | :------: | :-----:
 * \ref MCPS_UNCONFIRMED | YES     | YES        | NO       | YES
 * \ref MCPS_CONFIRMED   | YES     | YES        | NO       | YES
 * \ref MCPS_MULTICAST   | NO      | YES        | NO       | NO
 * \ref MCPS_PROPRIETARY | YES     | YES        | NO       | YES
 *
 * The following table provides links to the function implementations of the
 * related MCPS primitives:
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MCPS-Request     | \ref VelaMacMlmeRequest
 * MCPS-Confirm     | MacMcpsConfirm in \ref VelaMacPrimitives_t
 * MCPS-Indication  | MacMcpsIndication in \ref VelaMacPrimitives_t
 */
typedef enum eMcps
{
    /*!
     * Unconfirmed VelaMAC frame
     */
    MCPS_UNCONFIRMED,
    /*!
     * Confirmed VelaMAC frame
     */
    MCPS_CONFIRMED,
    /*!
     * Multicast VelaMAC frame
     */
    MCPS_MULTICAST,
    /*!
     * Proprietary frame
     */
    MCPS_PROPRIETARY,
}Mcps_t;

/*!
 * \brief VelaMAC management services
 *
 * \details The following table list the primitives which are supported by the
 *          specific MAC management service:
 *
 * Name                  | Request | Indication | Response | Confirm
 * --------------------- | :-----: | :--------: | :------: | :-----:
 * \ref MLME_JOIN        | YES     | NO         | NO       | YES
 * \ref MLME_LINK_CHECK  | YES     | NO         | NO       | YES
 *
 * The following table provides links to the function implementations of the
 * related MLME primitives.
 *
 * Primitive        | Function
 * ---------------- | :---------------------:
 * MLME-Request     | \ref VelaMacMlmeRequest
 * MLME-Confirm     | MacMlmeConfirm in \ref VelaMacPrimitives_t
 */
typedef enum eMlme
{
    /*!
     * Initiates the Over-the-Air activation
     */
    MLME_JOIN,
    /*!
     * LinkCheckReq - Connectivity validation
     */
    MLME_LINK_CHECK,
}Mlme_t;

/*!
 * VelaMAC MLME-Confirm primitive
 */
typedef struct sMlmeConfirm
{
    /*!
     * Holds the previously performed MLME-Request
     */
    Mlme_t MlmeRequest;
    /*!
     * Status of the operation
     */
    VelaMacEventInfoStatus_t Status;
    /*!
     * The transmission time on air of the frame
     */
    uint32_t TxTimeOnAir;
    /*!
     * Demodulation margin. Contains the link margin [dB] of the last
     * successfully received LinkCheckReq
     */
    uint8_t DemodMargin;
    /*!
     * Number of gateways which received the last LinkCheckReq
     */
    uint8_t NbGateways;
}MlmeConfirm_t;

/*!
 * VelaMAC MCPS-Indication primitive
 */
typedef struct sMcpsIndication
{
    /*!
     * MCPS-Indication type
     */
    Mcps_t McpsIndication;
    /*!
     * Status of the operation
     */
    VelaMacEventInfoStatus_t Status;
    /*!
     * Multicast
     */
    uint8_t Multicast;
    /*!
     * Application port
     */
    uint8_t Port;
    /*!
     * Downlink datarate
     */
    uint8_t RxDatarate;
    /*!
     * Frame pending status
     */
    uint8_t FramePending;
    /*!
     * Pointer to the received data stream
     */
    uint8_t *Buffer;
    /*!
     * Size of the received data stream
     */
    uint8_t BufferSize;
    /*!
     * Indicates, if data is available
     */
    bool RxData;
    /*!
     * Rssi of the received packet
     */
    int16_t Rssi;
    /*!
     * Snr of the received packet
     */
    uint8_t Snr;
    /*!
     * Receive window
     *
     * [0: Rx window 1, 1: Rx window 2]
     */
    uint8_t RxSlot;
    /*!
     * Set if an acknowledgement was received
     */
    bool AckReceived;
    /*!
     * The downlink counter value for the received frame
     */
    uint32_t DownLinkCounter;
}McpsIndication_t;

/*!
 * VelaMAC MCPS-Confirm
 */
typedef struct sMcpsConfirm
{
    /*!
     * Holds the previously performed MCPS-Request
     */
    Mcps_t McpsRequest;
    /*!
     * Status of the operation
     */
    VelaMacEventInfoStatus_t Status;
    /*!
     * Uplink datarate
     */
    uint8_t Datarate;
    /*!
     * Transmission power
     */
    int8_t TxPower;
    /*!
     * Set if an acknowledgement was received
     */
    bool AckReceived;
    /*!
     * Provides the number of retransmissions
     */
    uint8_t NbRetries;
    /*!
     * The transmission time on air of the frame
     */
    uint32_t TxTimeOnAir;
    /*!
     * The uplink counter value related to the frame
     */
    uint32_t UpLinkCounter;
}McpsConfirm_t;

/*!
 * VelaMAC events structure
 * Used to notify upper layers of MAC events
 */
typedef struct sVelaMacPrimitives
{
    /*!
     * \brief   MCPS-Confirm primitive
     *
     * \param   [OUT] MCPS-Confirm parameters
     */
    void ( *MacMcpsConfirm )( McpsConfirm_t *McpsConfirm );
    /*!
     * \brief   MCPS-Indication primitive
     *
     * \param   [OUT] MCPS-Indication parameters
     */
    void ( *MacMcpsIndication )( McpsIndication_t *McpsIndication );
    /*!
     * \brief   MLME-Confirm primitive
     *
     * \param   [OUT] MLME-Confirm parameters
     */
    void ( *MacMlmeConfirm )( MlmeConfirm_t *MlmeConfirm );
}VelaMacPrimitives_t;

/*!
 * VelaMAC receive window 2 channel parameters
 */
typedef struct sRx2ChannelParams
{
    /*!
     * Frequency in Hz
     */
    uint32_t Frequency;
    /*!
     * Data rate
     *
     * EU868 - [DR_0, DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7]
     *
     * US915 - [DR_8, DR_9, DR_10, DR_11, DR_12, DR_13]
     */
    uint8_t  Datarate;
}Rx2ChannelParams_t;

/*!
 * Global MAC layer parameters
 */
typedef struct sVelaMacParams
{
    /*!
     * Channels TX power
     */
    int8_t ChannelsTxPower;
    /*!
     * Channels data rate
     */
    int8_t ChannelsDatarate;
    /*!
     * VelaMac maximum time a reception window stays open
     */
    uint32_t MaxRxWindow;
    /*!
     * Receive delay 1
     */
    uint32_t ReceiveDelay1;
    /*!
     * Receive delay 2
     */
    uint32_t ReceiveDelay2;
    /*!
     * Join accept delay 1
     */
    uint32_t JoinAcceptDelay1;
    /*!
     * Join accept delay 1
     */
    uint32_t JoinAcceptDelay2;
    /*!
     * Number of uplink messages repetitions [1:15] (unconfirmed messages only)
     */
    uint8_t ChannelsNbRep;
    /*!
     * Datarate offset between uplink and downlink on first window
     */
    uint8_t Rx1DrOffset;
    /*!
     * VelaMAC 2nd reception window settings
     */
    Rx2ChannelParams_t Rx2Channel;
    /*!
     * Mask indicating which channels are enabled
     */
    uint16_t ChannelsMask[6];
}VelaMacParams_t;

/*!
 * VelaMAC channels parameters definition
 */
typedef union uDrRange
{
    /*!
     * Byte-access to the bits
     */
    int8_t Value;
    /*!
     * Structure to store the minimum and the maximum datarate
     */
    struct sFields
    {
         /*!
         * Minimum data rate
         *
         * EU868 - [DR_0, DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7]
         *
         * US915 - [DR_0, DR_1, DR_2, DR_3, DR_4]
         */
        int8_t Min : 4;
        /*!
         * Maximum data rate
         *
         * EU868 - [DR_0, DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7]
         *
         * US915 - [DR_0, DR_1, DR_2, DR_3, DR_4]
         */
        int8_t Max : 4;
    }Fields;
}DrRange_t;

/*!
 * VelaMAC channel definition
 */
typedef struct sChannelParams
{
    /*!
     * Frequency in Hz
     */
    uint32_t Frequency;
    /*!
     * Data rate definition
     */
    DrRange_t DrRange;
    /*!
     * Band index
     */
    uint8_t Band;
}ChannelParams_t;

/*!
 * VelaMAC multicast channel parameter
 */
typedef struct sMulticastParams
{
    /*!
     * Address
     */
    uint32_t Address;
    /*!
     * Network session key
     */
    uint8_t NwkSKey[16];
    /*!
     * Application session key
     */
    uint8_t AppSKey[16];
    /*!
     * Downlink counter
     */
    uint32_t DownLinkCounter;
    /*!
     * Reference pointer to the next multicast channel parameters in the list
     */
    struct sMulticastParams *Next;
}MulticastParams_t;

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/
/*!
 * \brief   VelaMAC layer initialization
 *
 * \details In addition to the initialization of the VelaMAC layer, this
 *          function initializes the callback primitives of the MCPS and
 *          MLME services. Every data field of \ref VelaMacPrimitives_t must be
 *          set to a valid callback function.
 *
 * \param   [IN] events - Pointer to a structure defining the VelaMAC
 *                        event functions. Refer to \ref VelaMacPrimitives_t.
 *
 * \param   [IN] events - Pointer to a structure defining the VelaMAC
 *                        callback functions. Refer to \ref VelaMacCallback_t.
 *
 * \retval  VelaMacStatus_t Status of the operation. Possible returns are:
 *          returns are:
 *          \ref VELAMAC_STATUS_OK,
 *          \ref VELAMAC_STATUS_PARAMETER_INVALID.
 */
VelaMacStatus_t VelaMacInitialization( VelaMacPrimitives_t *primitives);

#endif /* LIBRARIES_MAC_VELAPULSAR_MAC_H_ */
