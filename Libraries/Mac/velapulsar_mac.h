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
    APPROVAL_TIMEOUT,
    ACKNOWLEDGMENT_TIMEOUT,
    WAKEUP_TIMEOUT
}VelaMacTimeouts;

typedef enum{
    VELAMAC_SUCCESSFUL,
    VELAMAC_FAILURE,
	VELAMAC_UNDEFINED
}VelaMacStatus;

typedef enum{
    CONNECTION_PHASE,
    REPORTING_PHASE
}timeCycles;

typedef enum{
	MODE_CHANGE,
	JOIN_REQUEST,
	REQUEST_APPROVAL,
	REQUEST_DENIAL,
	REPORT,
	ACKNOWLEDGE,
	NOT_ACKNOWLEDGED
}messageType;

/*Node description*/
typedef struct{
    uint8_t     ID;
    uint32_t    longAddr;
    uint8_t     short_Add;
    uint16_t    timeSlot;
    uint16_t    password;
    uint16_t    lastPktID;
    bool        connected;
}NodeDesc;

/* Packets */
typedef struct{
    messageType msgType;
    uint32_t    myAddr;
    timeCycles  mode;
    uint16_t    time;
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
    uint16_t    password;
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
    uint32_t    myAddr;
    uint32_t    toAddr;
    uint16_t    pktID;
}request_Denial;

typedef union{
   request_Denial  data;
   uint8_t     pkt[sizeof(request_Denial)];
}request_DenialPkt;

/////////////////////////////////////////////////////

typedef struct{
    messageType msgType;
    uint8_t     short_Add;
    uint16_t    password;
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
}ack;

typedef union{
   ack         data;
   uint8_t     pkt[sizeof(ack)];
}ackPkt;

/////////////////////////////////////////////////////

typedef struct{
    messageType msgType;
    uint8_t     short_Add;
    uint16_t    time;
    uint16_t    pktID;
}nAck;

typedef union{
   nAck         data;
   uint8_t     pkt[sizeof(nAck)];
}nAckPkt;

typedef struct{
	void    ( *MacMessageReceived )( messageType msgType, uint16_t size, uint8_t shortAddr, uint8_t *payload, int16_t rssi, int8_t snr );
    void    ( *MacReportSent )( VelaMacStatus status );
    void    ( *MacCommandSent )( VelaMacStatus status );
    void    ( *MacNetworkJoined )( VelaMacStatus status );
    void    ( *MacReportingCycle )( timeCycles newCycle );
    void    ( *MacNewNodeJoined )( NodeDesc node );
    void    ( *MacNodeFailedToReport )( VelaMacStatus status );
    void	( *MacFrameStart )( void );
    void	( *MacFrameEnd )( void );
}macCallbacks;

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/
VelaMacStatus VelaMacInit(uint8_t nodeType, uint16_t dutyCycle, macCallbacks* callbacks);
VelaMacStatus VelaMacReport (uint8_t report[], int size);
int VelaMacRetrieveListOfNodes( NodeDesc nodesBuffer[]);
#endif /* LIBRARIES_MAC_VELAPULSAR_MAC_H_ */
