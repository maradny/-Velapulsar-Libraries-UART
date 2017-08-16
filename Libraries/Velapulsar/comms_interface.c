/*
 * Description: Communications interface implementation for Velapulsar
 * 				(application layer)
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
#include "comms_interface.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
static nwkCallbacks nwkEvents;
static volatile bool networkConnectionStatus;

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
/*!
 * \brief Resets MAC specific parameters to default
 */
static void OnNwkCmdSent (VelaMacStatus status);
static void OnNwkMsgRx (messageType msgType, uint16_t size, uint8_t shortAddr, uint8_t *payload, int16_t rssi, int8_t snr);
static void OnNwkNetworkJoined (VelaMacStatus status);
static void OnNwkNodeJoined (NodeDesc node);
static void OnNwkFailedToReport (VelaMacStatus status);
static void OnNwkReportSent (VelaMacStatus status);
static void OnNwkCycleChange (timeCycles newCycle);
static void OnNwkFrameStart (void);
static void OnNwkFrameEnd (void);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
VelaMacStatus CommsInitNode(){
	nwkEvents.NwkCommandSent = OnNwkCmdSent;
	nwkEvents.NwkMessageReceived = OnNwkMsgRx;
	nwkEvents.NwkNetworkJoined = OnNwkNetworkJoined;
	nwkEvents.NwkNewNodeJoined = OnNwkNodeJoined;
	nwkEvents.NwkNodeFailedToReport = OnNwkFailedToReport;
	nwkEvents.NwkReportSent = OnNwkReportSent;
	nwkEvents.NwkReportingCycle = OnNwkCycleChange;
	nwkEvents.NwkFrameStart = OnNwkFrameStart;
	nwkEvents.NwkFrameEnd = OnNwkFrameEnd;

	return VelaNwkInitialization(0, 60, &nwkEvents);
}

VelaMacStatus CommsInitCoordinator(){
	nwkEvents.NwkCommandSent = OnNwkCmdSent;
	nwkEvents.NwkMessageReceived = OnNwkMsgRx;
	nwkEvents.NwkNetworkJoined = OnNwkNetworkJoined;
	nwkEvents.NwkNewNodeJoined = OnNwkNodeJoined;
	nwkEvents.NwkNodeFailedToReport = OnNwkFailedToReport;
	nwkEvents.NwkReportSent = OnNwkReportSent;
	nwkEvents.NwkReportingCycle = OnNwkCycleChange;
	nwkEvents.NwkFrameStart = OnNwkFrameStart;
	nwkEvents.NwkFrameEnd = OnNwkFrameEnd;

	return VelaNwkInitialization(1, 60, &nwkEvents);
}
/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void ResetParameters (void){
	//IsVelaMacNetworkJoined = false;
}

static void OnNwkCmdSent (VelaMacStatus status){
    printf("NWK command sent\n");
}

static void OnNwkMsgRx (messageType msgType, uint16_t size, uint8_t shortAddr, uint8_t *payload, int16_t rssi, int8_t snr){
    printf("NWK command sent\n");
}

static void OnNwkNetworkJoined (VelaMacStatus status){
	switch (status){
	case VELAMAC_SUCCESSFUL:
	    printf("Connected to Network (APP)\n");
		networkConnectionStatus = true;
		break;

	case VELAMAC_FAILURE:
		printf("Connection to Network failed (APP)\n");
		networkConnectionStatus = false;
		break;
	}
}

static void OnNwkNodeJoined (NodeDesc node){
    printf("NWK command sent\n");
}

static void OnNwkFailedToReport (VelaMacStatus status){
    printf("NWK command sent\n");
}

static void OnNwkReportSent (VelaMacStatus status){
    printf("NWK command sent\n");
}

static void OnNwkCycleChange (timeCycles newCycle){
    printf("Received cycle change\n");
    switch (newCycle){
    case CONNECTION_PHASE:
    	break;

    case REPORTING_PHASE:
    	break;
    }
}

static void OnNwkFrameStart (void){
	// wake up
	printf("My frame is here!!! Yaaaay!!!\n");
    appDataPkt pkt;
	VelaNwkReport (pkt.pkt, sizeof(pkt.pkt));
}

static void OnNwkFrameEnd (void){
	printf("My frame is over... NOOO!!!\n");
	// sleep
}
