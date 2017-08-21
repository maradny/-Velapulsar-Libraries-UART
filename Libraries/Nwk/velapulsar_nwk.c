/*
 * Description: Network layer interface
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
#include "velapulsar_nwk.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
static nwkCallbacks* nwkEvents;
static macCallbacks macEvents;

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
/*!
 * \brief Resets MAC specific parameters to default
 */
static void ResetNwkParameters( void );

static void OnMacCmdSent (VelaMacStatus status);
static void OnMacMsgRx (messageType msgType, uint16_t size, uint8_t shortAddr, uint8_t *payload, int16_t rssi, int8_t snr);
static void OnMacNetworkJoined (VelaMacStatus status);
static void OnMacNodeJoined (NodeDesc node);
static void OnMacFailedToReport (VelaMacStatus status);
static void OnMacReportSent (VelaMacStatus status);
static void OnMacCycleChange (timeCycles newCycle);
static void OnMacFrameStart (void);
static void OnMacFrameEnd (void);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/
VelaMacStatus VelaNwkInitialization(uint8_t nodeType, uint16_t dutyCycle, nwkCallbacks* callbacks){
	nwkEvents = callbacks;

	macEvents.MacCommandSent = OnMacCmdSent;
	macEvents.MacMessageReceived = OnMacMsgRx;
	macEvents.MacNetworkJoined = OnMacNetworkJoined;
	macEvents.MacNewNodeJoined = OnMacNodeJoined;
	macEvents.MacNodeFailedToReport = OnMacFailedToReport;
	macEvents.MacReportSent = OnMacReportSent;
	macEvents.MacReportingCycle = OnMacCycleChange;
	macEvents.MacFrameStart = OnMacFrameStart;
	macEvents.MacFrameEnd = OnMacFrameEnd;
	return VelaMacInit(nodeType, dutyCycle, &macEvents);
}

VelaMacStatus VelaNwkReport (uint8_t appPayload[], int size){
	nwkDataPkt pkt;
	pkt.data.myShovel = MY_SHOVEL;
	pkt.data.myType = MY_TYPE;
	pkt.data.myUnit = MY_UNIT;

	memcpy(&pkt.data.appPayload, appPayload, size);

	printf("NWK sending: ");
	debug_print_pkt(pkt.pkt , sizeof(pkt.pkt) - (12 -size));
	return VelaMacReport(pkt.pkt, sizeof(pkt.pkt) - (12 -size));
}
/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
static void OnMacCmdSent (VelaMacStatus status){
    printf("NWK command sent\n");
    nwkEvents->NwkCommandSent(VELAMAC_SUCCESSFUL);
}

static void OnMacMsgRx (messageType msgType, uint16_t size, uint8_t shortAddr, uint8_t *payload, int16_t rssi, int8_t snr){
    printf("NWK message received\n");
    nwkEvents->NwkMessageReceived(msgType, size, shortAddr, payload, rssi, snr);
}

static void OnMacNetworkJoined (VelaMacStatus status){
    printf("NWK network joined\n");
    nwkEvents->NwkNetworkJoined(VELAMAC_SUCCESSFUL);
}

static void OnMacNodeJoined (NodeDesc node){
    printf("NWK new node joined\n");
    nwkEvents->NwkNewNodeJoined(node);
}

static void OnMacFailedToReport (VelaMacStatus status){
    printf("NWK failed to report\n");
    nwkEvents->NwkNodeFailedToReport(VELAMAC_SUCCESSFUL);
}

static void OnMacReportSent (VelaMacStatus status){
    printf("NWK report sent\n");
    nwkEvents->NwkReportSent(VELAMAC_SUCCESSFUL);
}

static void OnMacCycleChange (timeCycles newCycle){
    printf("NWK phase change\n");
    nwkEvents->NwkReportingCycle(VELAMAC_SUCCESSFUL);
}

static void OnMacFrameStart (void){
	nwkEvents->NwkFrameStart();
}

static void OnMacFrameEnd (void){
	nwkEvents->NwkFrameEnd();
}
