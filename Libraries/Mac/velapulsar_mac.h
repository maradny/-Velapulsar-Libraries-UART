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

#pragma pack(1)
/*****************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include "rf95.h"
#ifdef COORDINATOR
#include "UART_interface.h"
#endif
/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
#define MAX_NWK_PAYLOAD MAX_NWK_HEADER+MAX_APP_HEADER
#define MAX_PKT_SIZE MAX_NWK_PAYLOAD+MAX_MAC_HEADER

typedef enum{
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
}VelaMacStatus;

/*!
 * Enumeration containing the status of the operation of a MAC service
 */
typedef enum
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
}VelaMacEventInfoStatus;

/*!
 * Global MAC layer parameters
 */
typedef struct sVelaMacParams
{
    int8_t ChannelsTxPower;
    int8_t ChannelsDatarate;
    uint32_t MaxRxWindow;
    uint32_t ReceiveDelay1;
    uint32_t ReceiveDelay2;
    uint32_t JoinAcceptDelay1;
    uint32_t JoinAcceptDelay2;
}VelaMacParams;

typedef enum{
	COMMAND,
	ACKNOWLEDGE,
	FORWARD,
	REPORT,
	LONG_REPORT
}messageType;

typedef enum{
	RESET,
	GOTO_STATE,
	NO_CMD
}command;

typedef struct{
	uint16_t	pktID;
	uint32_t	myAddr;
	uint32_t	toAddr;
	messageType msgType;
	uint8_t		nwkPayload[MAX_NWK_PAYLOAD];
}macPayLoad;
//hello
typedef union{
	macPayLoad	data;
	uint8_t		pkt[sizeof(macPayLoad)];
}dataPkt;

typedef struct{
	void    ( *MacTxDone )( bool ack );
	void    ( *MacRxDone )( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
	void    ( *MacRxError )( void );
	void    ( *MacTxTimeout )( void );
	void    ( *MacRxTimeout )( uint16_t timeout );
}macCallbacks;

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/
VelaMacStatus VelaMacInitialization(uint8_t linkID, macCallbacks* callbacks);
VelaMacStatus VelaMacSend (uint8_t linkID, uint8_t nwkPayload[], int size);
#endif /* LIBRARIES_MAC_VELAPULSAR_MAC_H_ */
