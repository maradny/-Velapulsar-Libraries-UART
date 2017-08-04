/*
 * Description: Radio Module interface, provides an interface for all the
 * 				radio functions
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
#include "rf95.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/

/*!
 * Radio registers definition
 */
typedef struct
{
    RadioModems_t Modem;
    uint8_t       Addr;
    uint8_t       Value;
}RadioRegisters_t;

/*!
 * FSK bandwidth definition
 */
typedef struct
{
    uint32_t bandwidth;
    uint8_t  RegValue;
}FskBandwidth_t;

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
	RFInit,
	RFGetStatus,
	RFInitModem,
	RFSetChannel,
	RFIsChannelFree,
	RFRandom,
	RFSetRxConfig,
	RFSetTxConfig,
	RFCheckRfFrequency,
	RFGetTimeOnAir,
	RFSend,
	RFSetSleep,
	RFSetStby,
	RFSetRx,
	RFStartCad,
	RFReadRssi,
	RFWrite,
	RFRead,
	RFWriteBuffer,
	RFReadBuffer,
	RFSetMaxPayloadLength
};

/*!
 * Radio callbacks variable
 */
static RadioEvents_t *RadioEvents;

/*!
 * Reception buffer
 */
static uint8_t RxTxBuffer[FIFO_SIZE];

/*!
 * Radio Settings
 */
static RadioSettings_t settings;

/*****************************************************************************
 *                        LOCAL FUNCTION PROTOYPES
 *****************************************************************************/
void RFIrqInit(void);
void RFReset (void);
void RFSetOpMode (uint8_t opMode);
void RFSetTx (uint32_t timout);
void RFWriteFifo (uint8_t *buffer, uint8_t size);

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/

bool RFCheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}

bool RFInit(RadioEvents_t *events){
	RadioEvents = events;

	RFReset();

	initSPI();

	GPIO_setAsOutputPin(CS_PORT, CS_PIN);
	GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);

    // Initialize driver timeout timers
//    TimerInit( &TxTimeoutTimer, SX1276OnTimeoutIrq );
//    TimerInit( &RxTimeoutTimer, SX1276OnTimeoutIrq );
//    TimerInit( &RxTimeoutSyncWord, SX1276OnTimeoutIrq );

	if (!RFInitModem(MODEM_LORA)){
		printf ("Failed, trying again...\n");
		if(!RFInitModem(MODEM_LORA))
			return false;
	}


	RFIrqInit();

	settings.State = RF_SLEEP;

	return true;
}

RadioState_t RFGetStatus (void){
	return settings.State;
}

void RFSetChannel(uint32_t freq){
	settings.Channel = freq;
	uint32_t frf = (DEFAULT_FREQ * 1000000.0) / FSTEP;
	RFWrite(REG_06_FRF_MSB, (frf >> 16) & 0xff);
	RFWrite(REG_07_FRF_MID, (frf >> 8) & 0xff);
	RFWrite(REG_08_FRF_LSB, frf & 0xff);
}

bool RFIsChannelFree (uint32_t freq, int16_t rssiThresh){
	int16_t rssi = 0;
	RFSetChannel(freq);
	RFSetOpMode(MODE_RXCONTINUOUS);
	delay_ms(10);
	rssi = RFReadRssi();
	RFSetSleep();

	if (rssi > rssiThresh){
		return false;
	}
	return true;
}

void RFSetSleep(void){
	RFSetOpMode(MODE_SLEEP);
	settings.State = RF_SLEEP;
}

void RFSetStby (void){
	RFSetOpMode (MODE_STDBY);
	settings.State = RF_IDLE;
}

uint32_t RFRandom(void){
	uint8_t i;
	uint32_t rnd =0;

	// Disable LORA interrupts
	RFWrite(REG_11_IRQ_FLAGS_MASK, RX_TIMEOUT |
			RX_DONE |
			PAYLOAD_CRC_ERROR |
			VALID_HEADER |
			TX_DONE |
			CAD_DONE |
			FHSS_CHANGE_CHANNEL |
			CAD_DETECTED);

	RFSetOpMode (MODE_RXCONTINUOUS);

	for (i = 0; i<32; i++){
		delay_ms(5);

		rnd |= ((uint32_t)RFRead(REG_1B_RSSI_VALUE) & 0x01) << i;
	}

	RFSetSleep();

	return rnd;
}

// bw = 7 -> 125, 8-> 250, 9-> 500
void RFSetRxConfig( uint32_t bandwidth, uint32_t datarate,
					uint8_t coderate, uint16_t preambleLen,
					uint16_t symbTimeout, bool fixLen,
					uint8_t payloadLen,	bool crcOn, bool rxContinuous){

	settings.LoRa.Bandwidth = bandwidth;
	settings.LoRa.Datarate = datarate;
	settings.LoRa.Coderate = coderate;
	settings.LoRa.PreambleLen = preambleLen;
	settings.LoRa.FixLen = fixLen;
	settings.LoRa.PayloadLen = payloadLen;
	settings.LoRa.CrcOn = crcOn;
	settings.LoRa.RxTimeout = symbTimeout;
	settings.LoRa.RxContinuous = rxContinuous;

	if (datarate > 12){
		datarate = 12;
	}
	else if (datarate <6){
		datarate = 6;
	}
	if( ( ( bandwidth == 7 ) && ( ( datarate == 11 ) || ( datarate == 12 ) ) ) ||
		( ( bandwidth == 8 ) && ( datarate == 12 ) ) ){
		settings.LoRa.LowDatarateOptimize = 0x01;
	}
	else{
		settings.LoRa.LowDatarateOptimize = 0x00;
	}

    RFWrite(REG_1D_MODEM_CONFIG1, (bandwidth << 4) | (coderate << 1) | fixLen);
    RFWrite(REG_1E_MODEM_CONFIG2, (datarate << 4) | (crcOn << 2) | ((symbTimeout >> 8) & ~0xFC));
    RFWrite(REG_26_MODEM_CONFIG3, (settings.LoRa.LowDatarateOptimize << 3));
    RFWrite(REG_1F_SYMB_TIMEOUT_LSB, (uint8_t)(symbTimeout & 0xFF));
    RFWrite(REG_20_PREAMBLE_MSB, (uint8_t)((preambleLen >> 8) & 0xFF));
    RFWrite(REG_21_PREAMBLE_LSB, (uint8_t)(preambleLen &0xFF));

    if (fixLen == 1){
    	RFWrite(REG_22_PAYLOAD_LENGTH, payloadLen);
    }
}

void RFSetTxConfig(int8_t power, uint32_t bandwidth, uint32_t datarate,
                        uint8_t coderate, uint16_t preambleLen,
                        bool fixLen, bool crcOn, uint32_t timeout ){
	uint8_t paConfig = 0;
	uint8_t paDac = 0;

	paConfig = RFRead(REG_09_PA_CONFIG);
	paDac = RFRead(REG_4D_PA_DAC);

	paConfig = paConfig | 0x80; // select PA Boost
	paConfig = paConfig | 0x70; // select max power

	if (power > 17){
		//paDac = paDac | 0x07; // Turn on DAC for 20dbm
		paDac = 0x87;
		if (power > 20){
			power = 20;
		}
		paConfig = paConfig | ((uint8_t)((uint16_t)(power - 5) & 0x0F));
	}
	else {
		paDac = (paDac & 0xF8) | 0x04; // Turn off DAC for low power
		if (power < 2){
			power = 2;
		}
		if (power > 17){
			power = 17;
		}
		paConfig = paConfig | ((uint8_t)((uint16_t)(power - 2) & 0x0F));
	}
	RFWrite (REG_09_PA_CONFIG, paConfig);
	RFWrite (REG_4D_PA_DAC, paDac);

	settings.LoRa.Power = power;
	settings.LoRa.Bandwidth = bandwidth;
	settings.LoRa.Datarate = datarate;
	settings.LoRa.Coderate = coderate;
	settings.LoRa.PreambleLen = preambleLen;
	settings.LoRa.FixLen = fixLen;
	settings.LoRa.CrcOn = crcOn;
	settings.LoRa.TxTimeout = timeout;

	if (datarate > 12){
		datarate = 12;
	}
	else if (datarate < 6){
		datarate = 6;
	}
	if ( ( (bandwidth == 7) && ((datarate == 11) || (datarate == 12)) ) ||
		 ( (bandwidth == 8) && (datarate == 12) ) ){
		settings.LoRa.LowDatarateOptimize = 0x01;
	}
	else{
		settings.LoRa.LowDatarateOptimize = 0x00;
	}

    RFWrite(REG_1D_MODEM_CONFIG1, (bandwidth << 4) | (coderate << 1) | fixLen);
    RFWrite(REG_1E_MODEM_CONFIG2, (RFRead(REG_1E_MODEM_CONFIG2) & 0x0B) | (datarate << 4) | (crcOn << 2));
    RFWrite(REG_26_MODEM_CONFIG3, (settings.LoRa.LowDatarateOptimize << 3));
    RFWrite(REG_20_PREAMBLE_MSB, (uint8_t)((preambleLen >> 8) & 0xFF));
    RFWrite(REG_21_PREAMBLE_LSB, (uint8_t)(preambleLen &0xFF));
    RFSetChannel (DEFAULT_FREQ);
}

uint32_t RFGetTimeOnAir (uint8_t pktLen){
	uint32_t airTime = 0;
	double bw = 0.0;
	switch(settings.LoRa.Bandwidth){
	case 7:
		bw = 125;
		break;
	case 8:
		bw = 250;
		break;
	case 9:
		bw = 500;
		break;
	}

	// Symbol rate : time for one symbol (secs)
	double rs = bw / (1 << settings.LoRa.Datarate);
	double ts = 1 / rs;
	printf("symbolic time: %f\n",ts);
	// time of preamble
	double tPreamble = ts;
	tPreamble = tPreamble* ((double)settings.LoRa.PreambleLen + 4.25) ;
	printf("preamble length: %d\n",settings.LoRa.PreambleLen);
	printf("preamble time: %d\n",tPreamble);
	// Symbol length of payload and time
	double tmp = ceil( ( 8 * pktLen - 4 * settings.LoRa.Datarate +
						 28 + 16 * settings.LoRa.CrcOn -
						 ( settings.LoRa.FixLen ? 20 : 0) ) /
						 ( double )( 4 * settings.LoRa.Datarate -
						 ( ( settings.LoRa.LowDatarateOptimize > 0) ? 2 : 0 ) ) ) *
						 ( settings.LoRa.Coderate + 4);
	double nPayload = 8 + ( ( tmp > 0) ? tmp : 0);
	double tPayload = nPayload * ts;
	// Time on air
	double tOnAir = tPreamble + tPayload;
	// return us seconds
	airTime = floor( tOnAir * 1e3 + 0.999);

	return airTime;
}

uint32_t RFGetSymbolTime (void){

}

void RFSend (uint8_t *buffer, uint8_t size){
	uint32_t txTimeout = 0;

	settings.LoRaPacketHandler.Size = size;

	//Initialize payload size
	RFWrite (REG_22_PAYLOAD_LENGTH, size);

	// full buffer for TX
	RFWrite (REG_0E_FIFO_TX_BASE_ADDR, 0);
	RFWrite (REG_0D_FIFO_ADDR_PTR, 0);

	RFSetStby();
	delay_ms(10);

	// write payload buffer
	RFWriteFifo (buffer, size);
	txTimeout = settings.LoRa.TxTimeout;
	//printf("sending %d bytes\n", size);
	RFSetTx (txTimeout);
}

void RFSendWithDefault( uint8_t *buffer, uint8_t size, uint8_t msgtype){
    RFSetTxConfig(23, 9, 12,1, 20, false, true, 1000);
    msgType = msgtype;
    RFSend (buffer, size);
}

void RFRxWithDefault(void){
    RFSetRxConfig(9, 12,1, 30, ACK_TIMEOUT, false, 20, true, false);
    RFSetRx(1000);
}

void RFRxWithDefaultContinuous(void){
    RFSetRxConfig(9, 12,1, 30, ACK_TIMEOUT, false, 20, true, true);
    RFSetRx(1000);
}

void RFSetRx (uint32_t timeout){
	bool rxContinuous = false;

	// Clear all flags
	RFWrite(REG_12_IRQ_FLAGS, 0xff);

	rxContinuous = settings.LoRa.RxContinuous;

	RFWrite (REG_0F_FIFO_RX_BASE_ADDR, 0);
	RFWrite (REG_0D_FIFO_ADDR_PTR, 0);

	RFWrite(REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone & RxTimeout

	memset(RxTxBuffer, 0, FIFO_SIZE);

	settings.State = RF_RX_RUNNING;

	//Start timer here

	if (rxContinuous == true){
		RFWrite(REG_01_OP_MODE, MODE_RXCONTINUOUS);
	}
	else{
		RFWrite(REG_01_OP_MODE, MODE_RXSINGLE);
	}
}

void RFStartCad(void){
	// Clear all flags
	RFWrite(REG_12_IRQ_FLAGS, 0xff);

	RFWrite(REG_40_DIO_MAPPING1, 0xA0); // Interrupt on CADDone & CADDetected

	settings.State = RF_CAD;
	RFSetOpMode (MODE_CAD);
}

int16_t RFReadRssi (void){
	int16_t rssi = 0;

	if (settings.Channel > RF_MID_BAND_THRESH){
		rssi = RSSI_OFFSET_HF + RFRead(REG_1B_RSSI_VALUE);
	}
	else{
		rssi = RSSI_OFFSET_LF + RFRead(REG_1B_RSSI_VALUE);
	}
	return rssi;
}

bool RFInitModem (RadioModems_t modem){
	RFSetOpMode(MODE_SLEEP);
	uint8_t val = RFRead(REG_01_OP_MODE);
	printf("val: %x\n", val);
	val = RFRead(REG_24_HOP_PERIOD);
	printf("val: %x\n", val);
	RFWrite(REG_01_OP_MODE, LONG_RANGE_MODE);
    val = RFRead(REG_01_OP_MODE);
    printf("val: %x\n", val);
    if (val != LONG_RANGE_MODE){
        return false;
    }

    RFWrite(REG_0E_FIFO_TX_BASE_ADDR, 0);
    RFWrite(REG_0F_FIFO_RX_BASE_ADDR, 0);

    RFSetChannel (DEFAULT_FREQ);
    //setPreambleLength(20); // Default is 8
    //setFrequency(DEFAULT_FREQ);
    //setTxPower(DEFAULT_POWER, false);
    return true;
}

void RFWrite(uint8_t reg, uint8_t val){
    GPIO_setOutputLowOnPin(CS_PORT, CS_PIN);
    SPI_transmitData(SPI_PORT, reg | RF_SPI_WRITE_MASK);
    SPI_transmitData(SPI_PORT, val);
    delay_ms(10);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
}

void RFWriteBuffer(uint8_t reg, uint8_t* src, uint8_t len)
{
    GPIO_setOutputLowOnPin(CS_PORT, CS_PIN);
    SPI_transmitData(SPI_PORT, reg | RF_SPI_WRITE_MASK);
    while (len--){
        SPI_transmitData(SPI_PORT, *src++);
        delay_ms(1);
    }
    delay_ms(10);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
}

uint8_t RFRead(uint8_t reg){
    uint8_t val = 0;
    GPIO_setOutputLowOnPin(CS_PORT, CS_PIN);
    SPI_transmitData(SPI_PORT, reg & ~RF_SPI_WRITE_MASK);
    SPI_transmitData(SPI_PORT, 0);
    delay_ms(5);
    val = SPI_receiveData(SPI_PORT);
    delay_ms(10);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
    return val;
}
//
void RFReadBuffer(uint8_t reg, uint8_t* dest, uint8_t len){
    GPIO_setOutputLowOnPin(CS_PORT, CS_PIN);
    SPI_transmitData(SPI_PORT, reg & ~RF_SPI_WRITE_MASK);
    while (len--){
        SPI_transmitData(SPI_PORT, 0);
        delay_ms(2);
        *dest++ = SPI_receiveData(SPI_PORT);
    }
    delay_ms(10);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
}

void PORT2_IRQHandler(void){
	uint32_t status;
	status = MAP_GPIO_getEnabledInterruptStatus(INT_PORT);
	//printf("port interrupt is: %x\n", status);
	uint8_t irq_flags;
	if (status == INT_PIN || status == INT1_PIN){
		MAP_GPIO_clearInterruptFlag(INT_PORT, status);
		irq_flags = RFRead(REG_12_IRQ_FLAGS);
		//printf("flags are: %x\n", irq_flags);
		//printf("current state: %s", settings.State);
		switch (settings.State){
		case RF_RX_RUNNING:
			if(irq_flags & PAYLOAD_CRC_ERROR){
				printf("CRC Error\n");
				//if (!settings.LoRa.RxContinuous){
					//settings.State = RF_IDLE;
					RFSetStby();
				//}
				if ( (RadioEvents != 0) && (RadioEvents->RxError != 0)){
					//Get Pkt
					settings.LoRaPacketHandler.Size = RFRead(REG_13_RX_NB_BYTES);
					//printf("received %d bytes\n", settings.LoRaPacketHandler.Size);
					RFWrite(REG_0D_FIFO_ADDR_PTR, RFRead(REG_10_FIFO_RX_CURRENT_ADDR));
					RFReadBuffer(REG_00_FIFO, RxTxBuffer, settings.LoRaPacketHandler.Size);
					debug_print_pkt(RxTxBuffer , settings.LoRaPacketHandler.Size);
					RadioEvents->RxError();
				}
			}
			else if (irq_flags & RX_TIMEOUT){
				settings.State = RF_IDLE;

				if ( (RadioEvents != 0) && (RadioEvents->RxTimeout != 0)){
					RadioEvents->RxTimeout(settings.LoRa.RxTimeout);
				}
			}
			else if(irq_flags & RX_DONE){
				//Get SNR
				int8_t snr = 0;
				settings.LoRaPacketHandler.SnrValue = RFRead(REG_19_PKT_SNR_VALUE);
				if (settings.LoRaPacketHandler.SnrValue & 0x80){
					snr = ( (~settings.LoRaPacketHandler.SnrValue + 1) & 0xFF) >> 2;
					snr = -snr;
				}
				else{
					snr = (settings.LoRaPacketHandler.SnrValue & 0xFF) >>2;
				}

				//Get RSSI
				int16_t rssi = RFRead(REG_1A_PKT_RSSI_VALUE);
				if (snr <0){
					if (settings.Channel > RF_MID_BAND_THRESH){
						settings.LoRaPacketHandler.RssiValue = RSSI_OFFSET_HF + rssi + (rssi >> 4) + snr;
					}
					else{
						settings.LoRaPacketHandler.RssiValue = RSSI_OFFSET_LF + rssi + (rssi >> 4) + snr;
					}
				}
				else{
					if (settings.Channel > RF_MID_BAND_THRESH){
						settings.LoRaPacketHandler.RssiValue = RSSI_OFFSET_HF + rssi + (rssi >> 4);
					}
					else{
						settings.LoRaPacketHandler.RssiValue = RSSI_OFFSET_LF + rssi + (rssi >> 4);
					}
				}

				//Get Pkt
				settings.LoRaPacketHandler.Size = RFRead(REG_13_RX_NB_BYTES);
				//printf("received %d bytes\n", settings.LoRaPacketHandler.Size);
				RFWrite(REG_0D_FIFO_ADDR_PTR, RFRead(REG_10_FIFO_RX_CURRENT_ADDR));
				RFReadBuffer(REG_00_FIFO, RxTxBuffer, settings.LoRaPacketHandler.Size);
				if (!settings.LoRa.RxContinuous){
					settings.State = RF_IDLE;
				}
				if ((RadioEvents != 0) && (RadioEvents->RxDone != 0)){
					RadioEvents->RxDone(RxTxBuffer, settings.LoRaPacketHandler.Size, settings.LoRaPacketHandler.RssiValue, settings.LoRaPacketHandler.SnrValue);
				}
			}
			break;
		case RF_TX_RUNNING:
			if (irq_flags & TX_DONE){
				settings.State = RF_IDLE;
				if ( (RadioEvents != 0) && (RadioEvents->TxDone != 0)){
					RadioEvents->TxDone(false);
				}
			}
			break;
		case RF_TX_ACK_RUNNING:
			if (irq_flags & TX_DONE){
				settings.State = RF_IDLE;
				if ( (RadioEvents != 0) && (RadioEvents->TxDone != 0)){
					RadioEvents->TxDone(true);
				}
			}
			break;
		}
	}
	//RFSetStby();
	RFWrite (REG_0F_FIFO_RX_BASE_ADDR, 0);
	RFWrite (REG_0D_FIFO_ADDR_PTR, 0);
	while (irq_flags != 0){
		RFWrite(REG_12_IRQ_FLAGS, 0xFF); // Clear all IRQ flags
		irq_flags = RFRead(REG_12_IRQ_FLAGS);
		//printf("flags are: %x\n", irq_flags);
		delay_ms(5);
	}
}

/*****************************************************************************
 *                            LOCAL FUNCTIONS
 *****************************************************************************/
void RFIrqInit(void){
	MAP_GPIO_setAsInputPinWithPullDownResistor(INT_PORT, INT_PIN);
	MAP_GPIO_clearInterruptFlag(INT_PORT, INT_PIN);
	GPIO_interruptEdgeSelect(INT_PORT, INT_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
	MAP_GPIO_enableInterrupt(INT_PORT, INT_PIN);

	MAP_Interrupt_setPriority(INT_PORT + 50, 0x40);
	MAP_Interrupt_enableInterrupt(INT_PORT + 50);

	MAP_GPIO_setAsInputPinWithPullDownResistor(INT1_PORT, INT1_PIN);
	MAP_GPIO_clearInterruptFlag(INT1_PORT, INT1_PIN);
	GPIO_interruptEdgeSelect(INT1_PORT, INT1_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
	MAP_GPIO_enableInterrupt(INT1_PORT, INT1_PIN);

	MAP_Interrupt_setPriority(INT1_PORT + 50, 0x40);
	MAP_Interrupt_enableInterrupt(INT1_PORT + 50);
	MAP_Interrupt_enableMaster();
}

void RFReset (){
	/* Configure RST pin */
	GPIO_setAsOutputPin(RST_PORT, RST_PIN);
	GPIO_setOutputHighOnPin(RST_PORT, RST_PIN);

	GPIO_setOutputLowOnPin(RST_PORT, RST_PIN);
	delay_ms(100);
	GPIO_setOutputHighOnPin(RST_PORT, RST_PIN);
	delay_ms(100);
}

void RFSetOpMode (uint8_t opMode){
	RFWrite(REG_01_OP_MODE, opMode);
}

void RFSetTx (uint32_t timeout){
	// Clear all flags
//	RFWrite(REG_12_IRQ_FLAGS, 0xff);

	// Set interrupt on tx
	RFWrite(REG_40_DIO_MAPPING1, 0x40); // Interrupt on TxDone

	if (timeout == 5555){
		settings.State = RF_TX_ACK_RUNNING;
	}
	else{
		settings.State = RF_TX_RUNNING;
	}
//	uint8_t val = RFRead(REG_01_OP_MODE);
//	printf("val: %x\n", val);
//	val = RFRead(REG_24_HOP_PERIOD);
//	printf("val: %x\n", val);
	// Start timer with timeout
	RFSetOpMode (MODE_TX);
	//delay_ms(timeout);
}

void RFWriteFifo (uint8_t *buffer, uint8_t size){
	RFWriteBuffer (REG_00_FIFO, buffer, size);
}
