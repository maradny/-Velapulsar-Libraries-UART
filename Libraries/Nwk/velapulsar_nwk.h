/*
 * Description: Network layer header file
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

#ifndef LIBRARIES_NWK_VELAPULSAR_NWK_H_
#define LIBRARIES_NWK_VELAPULSAR_NWK_H_
/*****************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include "velapulsar_mac.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
typedef struct{
	uint8_t		myShovel;
	uint8_t		myUnit;
	uint16_t	myType;
	uint8_t		appPayload[MAX_APP_PAYLOAD];
}nwkPayLoad;

typedef union{
	nwkPayLoad	data;
	uint8_t		pkt[sizeof(nwkPayLoad)];
}nwkDataPkt;

typedef struct{
	void    ( *NwkTxDone )( void );
	void    ( *NwkRxDone )( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
	void    ( *NwkRxError )( void );
	void    ( *NwkTxTimeout )( void );
	void    ( *NwkRxTimeout )( void );
}nwkCallbacks;

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/
VelaMacStatus VelaNwkInitialization(nwkCallbacks* callbacks, uint8_t linkID); //maybe change return type later
VelaMacStatus VelaNwkSend(uint8_t linkID, uint8_t appPayload[], int size);

#endif /* LIBRARIES_NWK_VELAPULSAR_NWK_H_ */