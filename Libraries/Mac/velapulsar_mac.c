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
 * VelaMac internal states
 */
typedef enum{
    MAC_TX,
    MAC_RX
}VelaMacStates;

static RadioEvents_t radioEvents;
static macCallbacks* macEvents;

static volatile VelaMacStates currentState;
static volatile bool connected;

#ifdef COORDINATOR
    static volatile uint8_t numOfConnectedNodes = 0;
    static volatile NodeDesc connectedNodes[MAX_NUM_OF_NODES];
    static volatile uint16_t dutyCycle=60;
    static volatile int reportingTime,connectingTime,frameSize;
    static volatile timeCycles currentPhase;
    static volatile uint32_t currentPhaseStartingTime;
#else
    static volatile NodeDesc myNode;
#endif


/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
static void OnRadioTxDone (bool ack);
static void OnRadioRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
static void OnRadioRxError (void);
static void OnRadioTxTimeout (void);
static void OnRadioRxTimeout (VelaMacTimeouts timeout);
static void WaitForRand (int start, int end);
/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/

VelaMacStatus VelaMacInit (uint8_t nodeType, uint16_t _dutyCycle, macCallbacks* callbacks){
	macEvents = callbacks; // callbacks back to nwk layer
	// Initialize timers here.....

	// Initialize radio callbacks
	radioEvents.TxDone = OnRadioTxDone;
	radioEvents.RxDone = OnRadioRxDone;
	radioEvents.RxError = OnRadioRxError;
	radioEvents.TxTimeout = OnRadioTxTimeout;
	radioEvents.RxTimeout = OnRadioRxTimeout;
	if (!RFInit(&radioEvents)){
		return VELAMAC_FAILURE;
	}

#ifdef COORDINATOR
	dutyCycle = _dutyCycle;
	MacCalcDutyCycles();
	currentPhase = REPORTING_PHASE;
	ChangePhase();
#endif

	RFRxWithDefaultContinuous();
	currentState = MAC_RX;

	return VELAMAC_SUCCESSFUL;
}

VelaMacStatus VelaMacReport (uint8_t report[], int size){
#ifdef NODE
	currentState = MAC_TX;
	reportPkt pkt;
	pkt.data.msgType = REPORT;
	pkt.data.short_Add = myNode.short_Add ;
	pkt.data.password = myNode.password;
	pkt.data.pktID = myNode.lastPktID;
	memcpy(&pkt.data.payload, report, size);

	RFSendWithDefault(pkt.pkt, sizeof(pkt.pkt), pkt.data.msgType);
	printf("MAC sending");
#endif
	return VELAMAC_SUCCESSFUL;
}

int VelaMacRetrieveListOfNodes( NodeDesc nodesBuffer[]){
    int i;
#ifdef COORDINATOR
    for (i = 0; i < numOfConnectedNodes; i++){
        nodesBuffer[i] = connectedNodes[i];
    }

    return  numOfConnectedNodes;
#endif
}

/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
#ifdef COORDINATOR
void MacCalcDutyCycles(){
    int  airTime = RFGetTimeOnAir(sizeof(reportPkt)); //returns in milliseconds
    int  wakeUpTime= 6000; //1 sec
    //int sleepTime;
    // calculate the timeOnAir of the Ack
    //sleepTime = dutyCycle * 1000 - wakeUpTime - airTime - (RFGetTimeOnAir(sizeof(ackPkt))*2); //for now !!

    //printf ("sleepTime: %d\n", sleepTime);

//    connectingTime = (dutyCycle-(numOfConnectedNodes*3));
//    reportingTime = dutyCycle - connectingTime;
    connectingTime = dutyCycle/2;
    reportingTime = dutyCycle/2;
    frameSize = reportingTime/numOfConnectedNodes;
    printf ("connectingTime: %d\n", connectingTime);
    printf ("reportingTime: %d\n", reportingTime);
    printf ("frameSize: %d\n", frameSize);
}

uint16_t CalcNodeReportAfter (int nodeIndex){
	int timeSlot = frameSize * nodeIndex;

	switch (currentPhase){
	case REPORTING_PHASE:
		return ((getTotal_ms() - currentPhaseStartingTime) + connectingTime + timeSlot); // report in next reporting phase
		break;

	case CONNECTION_PHASE:
		if (reportingTime > timeSlot){
			return (timeSlot + connectingTime - (getTotal_ms() - currentPhaseStartingTime));
		}
		break;

	}
}

void SendAck(reportPkt repPkt, int16_t rssi, int8_t snr){
    ackPkt ackPkt;
    //calculate new reporting time
    int i, index;
    for (i = 0; i<numOfConnectedNodes; i++){
    	if (repPkt.data.short_Add == connectedNodes[i].short_Add){
    		index = i;
    	}
    }

    ackPkt.data.time = CalcNodeReportAfter (index);
    ackPkt.data.short_Add = repPkt.data.short_Add;
    ackPkt.data.pktID = repPkt.data.pktID;
    ackPkt.data.msgType = ACKNOWLEDGE;

    RFSendWithDefault(ackPkt.pkt, sizeof(ackPkt.pkt), ACKNOWLEDGE);

    UARTSendRssi(ackPkt.pkt, sizeof(ackPkt.pkt), rssi);
}

void SendNAck(reportPkt repPkt, int16_t rssi, int8_t snr){
    nAckPkt nAckPkt;
    //calculate new reporting time
    //ackPkt.data.time = repPkt.data.timeSlot;
    nAckPkt.data.short_Add = repPkt.data.short_Add;
    nAckPkt.data.pktID = repPkt.data.pktID;
    nAckPkt.data.msgType = NOT_ACKNOWLEDGED;

    RFSendWithDefault(nAckPkt.pkt, sizeof(nAckPkt.pkt), NOT_ACKNOWLEDGED);

    UARTSendRssi(nAckPkt.pkt, sizeof(nAckPkt.pkt), rssi);
}

void ProcessJoinReq (join_requestPkt joinReqPkt){
    //store node info
   connectedNodes[numOfConnectedNodes].longAddr= joinReqPkt.data.myAddr;
   connectedNodes[numOfConnectedNodes].ID= numOfConnectedNodes;
   connectedNodes[numOfConnectedNodes].lastPktID= joinReqPkt.data.pktID;
   connectedNodes[numOfConnectedNodes].short_Add = 123;  //change later
   //create random password
   srand((unsigned int)time(0));
   int r =rand() % 65535;
   connectedNodes[numOfConnectedNodes].password= r;
   connectedNodes[numOfConnectedNodes].connected = true;
   //Creating a time slot
   connectedNodes[numOfConnectedNodes].reportAfter = CalcNodeReportAfter(numOfConnectedNodes); //change later
   numOfConnectedNodes++;
   MacCalcDutyCycles();
   printf("CONNECTED with id: %d\n", numOfConnectedNodes-1);
}

void SendJoinResult (VelaMacStatus successful, uint8_t index, request_ApprovalPkt pkt){
    if (successful == VELAMAC_SUCCESSFUL){
        //create the request approval pkt and send it
        request_ApprovalPkt appPkt;
        appPkt.data.msgType=REQUEST_APPROVAL;
        appPkt.data.myAddr=MY_ADDRESS;                 //coordinator address
        appPkt.data.password=connectedNodes[index].password;
        appPkt.data.pktID=connectedNodes[index].lastPktID;
        appPkt.data.toAddr=connectedNodes[index].longAddr;
        appPkt.data.short_Add=connectedNodes[index].short_Add;
        appPkt.data.time = connectedNodes[index].reportAfter;

        RFSendWithDefault(appPkt.pkt, sizeof(appPkt.pkt), appPkt.data.msgType);

        printf("Sent approval for id: %d", index);
        macEvents->MacNewNodeJoined(connectedNodes[index]);
    }
    else{
        //create the request approval pkt and send it
        request_DenialPkt denPkt;
        denPkt.data.msgType=REQUEST_DENIAL;
        denPkt.data.myAddr=MY_ADDRESS;                 //coordinator address
        denPkt.data.pktID=pkt.data.pktID;
        denPkt.data.toAddr=pkt.data.myAddr;
        RFSendWithDefault(denPkt.pkt, sizeof(denPkt.pkt), denPkt.data.msgType);

        printf("Sent denial for node with id: %d", pkt.data.myAddr);
    }
}

void ChangePhase (void){
	switch (currentPhase){
	case REPORTING_PHASE:
		currentPhase = CONNECTION_PHASE;
		printf ("Time now: %d\n",getTotal_ms());
		Set_Alarm (connectingTime * 1000, &ChangePhase);
		break;

	case CONNECTION_PHASE:
		currentPhase = REPORTING_PHASE;
		printf ("Time now: %d\n",getTotal_ms());
		Set_Alarm (reportingTime * 1000, &ChangePhase);
		break;
	}

	currentPhaseStartingTime = getTotal_ms();
	SendPhaseChange();
}

void SendPhaseChange (void){
	mode_changePkt modePkt;

	modePkt.data.mode = currentPhase;
	modePkt.data.msgType = MODE_CHANGE;
	modePkt.data.myAddr = MY_ADDRESS;

	if (currentPhase == CONNECTION_PHASE){
		modePkt.data.time = connectingTime;
		modePkt.data.mode = CONNECTION_PHASE;
	}
	else{
		modePkt.data.time = reportingTime;
		modePkt.data.mode = REPORTING_PHASE;
	}
	//debug_print_pkt(modePkt.pkt, sizeof(modePkt.pkt));
	RFSendWithDefault(modePkt.pkt, sizeof(modePkt.pkt), MODE_CHANGE);
}

#else
void SendJoinReq (uint16_t phaseTime){
    //make the join packet and send it to the coordinator
    join_requestPkt reqPkt;
    reqPkt.data.msgType = JOIN_REQUEST;
    reqPkt.data.myAddr = myNode.longAddr;
    reqPkt.data.toAddr = BROADCAST_ADDRESS;
    reqPkt.data.pktID = myNode.lastPktID;

    WaitForRand(0,phaseTime-2);
    RFSendWithDefault(reqPkt.pkt, sizeof(reqPkt.pkt), reqPkt.data.msgType);
}

void SleepFor(uint16_t time){
    //sleep shwia

    //wakeup
    RFRxWithDefault();
}
#endif

static void WaitForRand (int start, int end){
	srand((unsigned int)time(0));
	int r =rand() % (end);
	printf("end was: %d\n", end*1000);
	printf("random time to wait was: %d\n", r);
	printf ("Time now: %d\n",getTotal_ms());
	delay_ms(r);
	printf ("Time now: %d\n",getTotal_ms());
}



static void OnRadioTxDone (bool ack){
#ifdef NODE
    RFRxWithDefault();
#else
    RFRxWithDefaultContinuous();
#endif
    currentState = MAC_RX;
}

static void OnRadioRxDone (uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
#ifdef SNIFFER
	UARTSendRssi(payload, size, rssi);
#else
    messageType receivedMsgType = *payload;
    mode_changePkt modePkt;
    request_ApprovalPkt appPkt;
    ackPkt ackPkt;
    join_requestPkt joinReqPkt;
    reportPkt repPkt;
    bool found = false;
    int i = 0;
    debug_print_pkt(payload, size);
	switch (receivedMsgType){
	#ifdef NODE
    case MODE_CHANGE:
    	printf("received mode change \n");
        memcpy(&modePkt.pkt, payload, size);
        if (modePkt.data.mode == CONNECTION_PHASE){
            if (myNode.connected == false){
                SendJoinReq(modePkt.data.time);
            }
            else{
                //sleep until next reporting cycle
            }
        }
        else{
            //wait for my frame and start reporting
        }

        macEvents->MacReportingCycle(modePkt.data.mode);
	    break;

    case REQUEST_APPROVAL:
        // I am a NODE !
        // Get the connection Information and wait for my reporting time
        memcpy(&appPkt.pkt, payload, size);
        //////
        if (appPkt.data.toAddr == MY_ADDRESS){
            myNode.ID = 0;  ///// ??
            myNode.connected = true;
            myNode.longAddr = MY_ADDRESS;
            myNode.password = appPkt.data.password;
            myNode.short_Add = appPkt.data.short_Add;
            myNode.reportAfter = appPkt.data.time;

            //inform the nwk layer
            macEvents->MacNetworkJoined(VELAMAC_SUCCESSFUL);

            //sleep until timeslot
            //SleepFor(myNode.reportAfter);
            //macEvents->MacReportingCycle(REPORTING_PHASE);
        }
        break;

    case REQUEST_DENIAL:
        macEvents->MacNetworkJoined(VELAMAC_FAILURE);
        break;

    case ACKNOWLEDGE:
        //I am a Node I sent a message and just received the acknowledgment
        memcpy (&ackPkt.pkt, payload, size);

        // 1- increment the packet ID
        if (myNode.lastPktID < 65535)
            myNode.lastPktID++;
        else
            myNode.lastPktID = 0;

        // 2- extract next report time
        myNode.reportAfter = ackPkt.data.time;

        // finally- inform upper layer that report was sent successfully
        macEvents->MacReportSent(VELAMAC_SUCCESSFUL);

        // sleep until report time is close by
        SleepFor(myNode.reportAfter);
        macEvents->MacReportingCycle(REPORTING_PHASE);
        // 4-
        break;
    case NOT_ACKNOWLEDGED:
        SleepFor(myNode.reportAfter);
        macEvents->MacReportingCycle(REPORTING_PHASE);
        break;
	#else
	case JOIN_REQUEST:
        //only the coordinator will receive a join request
        //1- extract the data
        memcpy(&joinReqPkt.pkt, payload, size);

        //2- check if the node is already connected or not

        for (i = 0; i< numOfConnectedNodes; i++){
            if (joinReqPkt.data.myAddr == connectedNodes[i].longAddr){
                found = true;
                printf("CONNECTED AGAIN with ID: %d\n", i);
                SendJoinResult (VELAMAC_SUCCESSFUL, i, appPkt);
                break;
            }
        }
        if (!found){
            if (numOfConnectedNodes < MAX_NUM_OF_NODES){
                ProcessJoinReq (joinReqPkt);
                delay_ms(700); //remove later
                SendJoinResult (VELAMAC_SUCCESSFUL, numOfConnectedNodes-1, appPkt);
            }
            else{
                printf("FAILED TO CONNECT\n");
                SendJoinResult (VELAMAC_FAILURE, 0, appPkt);
            }
        }
	    break;

	case REPORT:
	    //I am a coordinator !!
	    // copy the information
	    memcpy (&repPkt.pkt, payload, size);

	    int i = 0;
	    for (i; i<numOfConnectedNodes; i++){
	        if (connectedNodes[i].short_Add == repPkt.data.short_Add){
	            if (repPkt.data.password == connectedNodes[i].password){
	                macEvents->MacMessageReceived(repPkt.data.msgType, sizeof(repPkt.data.payload), repPkt.data.short_Add, repPkt.data.payload, rssi, snr);
	                connectedNodes[i].lastPktID = repPkt.data.pktID;
	                SendAck(repPkt, rssi, snr);
	            }
	            else{
	                SendNAck(repPkt, rssi, snr);
	            }
	            break;
	        }
	    }
	    if (i > numOfConnectedNodes){
	        SendNAck(repPkt, rssi, snr);
	    }
	    break;
	#endif
	}
#endif
}










static void OnRadioRxError (void){
	printf("Received error Pkt MAC\n");
	//macEvents->MacRxError();
}

static void OnRadioTxTimeout (void){
	printf("TX timeout mac\n");
	//macEvents->MacTxTimeout();
}

static void OnRadioRxTimeout (VelaMacTimeouts timeout){
	printf("rx timedout \n");
//	switch (timeout){
//	case APPROVAL_TIMEOUT:
//	    macEvents->MacNetworkJoined(VELAMAC_FAILURE);
//		break;
//
//	case ACKNOWLEDGMENT_TIMEOUT:
//	    macEvents->MacReportSent(VELAMAC_FAILURE);
//		break;
//#ifdef NODE
//	case WAKEUP_TIMEOUT:
//	    myNode.connected = false;
//	    macEvents->MacNetworkJoined(VELAMAC_FAILURE);
//	    //do something else
//		break;
//#endif
	//}
}
