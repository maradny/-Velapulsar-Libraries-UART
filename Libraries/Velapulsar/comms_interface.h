/*
 * Description: Communications interface API for Velapulsar (application layer)
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
#ifndef LIBRARIES_VELAPULSAR_COMMS_INTERFACE_H_
#define LIBRARIES_VELAPULSAR_COMMS_INTERFACE_H_

#include "velapulsar.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
typedef struct{
	sensorData 	sensorData;
	states		currentState;
}appPayLoad;	// app layer payload

// Move next defines to lower network layers
typedef enum{
	COMMAND,
	FORWARD,
	REPORT,
	LONG_REPORT
}messageType;

typedef enum{
	RESET,
	GOTO_STATE,
	NO_CMD
}command;

typedef union{
	frameStructure	data;
	uint8_t			pkt[sizeof(frameStructure)];
}dataPkt; //should be final packet in mac layer

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/
bool CommsSend(uint8_t linkID, dataPkt pkt, int size);
bool CommsReceive(uint8_t linkID, dataPkt* pkt);
bool CommsInit(uint8_t linkID, bool);

#endif /* LIBRARIES_VELAPULSAR_COMMS_INTERFACE_H_ */

