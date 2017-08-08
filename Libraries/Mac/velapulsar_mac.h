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
    VELAMAC_SUCCESSFUL,
    VELAMAC_FAILURE
}VelaMacStatus;


typedef enum{
	MODE_CHANGE,
	JOIN_REQUEST,
	REQUEST_APPROVAL,
	REPORT,
	ACKNOWLEDGE
}messageType;

/*Node description*/
typedef struct{
    uint8_t     ID;
    uint32_t    longAddr;
    uint8_t     short_Add;
    uint16_t    timeSlot;
    uint16_t    password;
}NodeDesc

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
}Ack;

typedef union{
   Ack  data;
   uint8_t     pkt[sizeof(Ack)];
}AckPkt;




typedef struct{
	void    ( *MacMessageReceived )( uint8_t msgType, uint8_t size, uint32_t fromAddr,uint8_t *payload);
    void    ( *MacReportSent )( bool sentStatus);
    void    ( *MacCommandSent )( bool commandStatus );
    void    ( *MacNetworkJoined )( bool NetworkStatus );
    void    ( *MacReportingCycle )( bool ReportingStatus );
    void    ( *MacNewNodeJoined )( NodeDescPkt);
    void    ( *MacNodeFailedToReport )( NodeDescPkt);
}macCallbacks;

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/
VelaMacStatus MacInit(uint8_t nodeType, uint16_t dutyCycle, macCallbacks* callbacks);
VelaMacStatus MacReport (uint8_t report[], int size);
#endif /* LIBRARIES_MAC_VELAPULSAR_MAC_H_ */
