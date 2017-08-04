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


typedef enum{
	MODE_CHANGE,
	JOIN_REQUEST,
	REQUEST_APPROVAL,
	REPORT,
	ACKNOWLEDGE
}messageType;

/* Packets */
typedef struct{
    messageType msgType;
    uint32_t    myAddr;
    uint8_t     mode;
}mode_change;

typedef union{
    mode_change  data;
    uint8_t     pkt[sizeof(mode_change)];
}mode_changePkt;
/////////////////////////////////////////////////////
typedef struct{
    messageType msgType;
    uint32_t    myAddr;
    uint32_t    toAddr;
    uint8_t     size;
    uint16_t    pktID;
}join_request;

typedef union{
    join_request  data;
    uint8_t     pkt[sizeof(join_request)];
}join_requestPkt;
/////////////////////////////////////////////////////
typedef struct{
    messageType msgType;
    uint32_t    myAddr;
    uint32_t    toAddr;
    uint8_t     short_Add;
    uint8_t     coord_secret;
    uint16_t    time;
    uint16_t    pktID;
}request_Approval;

typedef union{
   request_Approval  data;
   uint8_t     pkt[sizeof(request_Approval)];
}request_ApprovalPkt;
/////////////////////////////////////////////////////
typedef struct{
    messageType msgType;
    uint8_t     short_Add;
    uint8_t     coord_secret;
    uint8_t     payload[APP_PAYLOAD_SIZE];
    uint16_t    pktID;
}report;

typedef union{
   report       data;
   uint8_t      pkt[sizeof(report)];
}reportPkt;
/////////////////////////////////////////////////////
typedef struct{
    messageType msgType;
    uint8_t     short_Add;
    uint16_t    time;
    uint16_t    pktID;
}Ack;

typedef union{
   Ack  data;
   uint8_t     pkt[sizeof(Ack)];
}AckPkt;




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
