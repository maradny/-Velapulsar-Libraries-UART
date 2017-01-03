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
static const ModemConfig MODEM_CONFIG_TABLE[] ={
    //  1d,     1e,      26
	//{ 0x92,   0x90,    0x00}, // Bw500Cr45Sf512
	{ 0x92,   0xA4,    0x00}, // Bw500Cr45Sf1024
    //{ 0x72,   0x74,    0x00}, // Bw125Cr45Sf128 (the chip default)
    { 0x92,   0x74,    0x00}, // Bw500Cr45Sf128
    { 0x48,   0x94,    0x00}, // Bw31_25Cr48Sf512
    { 0x78,   0xc4,    0x00}, // Bw125Cr48Sf4096
    
};

/*****************************************************************************
 *                        FUNCTION IMPLEMENTATIONS
 *****************************************************************************/

/******************************************************************************
* Function: initRF
****************
* Initializes the radio module and sets all the header values.
*
* Parameters: Null
*
* Returns: True if initialized false otherwise.
******************************************************************************/
bool initRF(void){
    _mode = RFModeInitialising;
    _thisAddress = RF_BROADCAST_ADDRESS;
    _txHeaderTo = RF_BROADCAST_ADDRESS;
    _txHeaderFrom = RF_BROADCAST_ADDRESS;
    _txHeaderId = 0;
    _txHeaderFlags = 0;
    _rxBad = 0;
    _rxGood = 0;
    _txGood = 0;
    _cad_timeout = 0;
    
    /* Configure RST pin */
    GPIO_setAsOutputPin(RST_PORT, RST_PIN);
    GPIO_setOutputHighOnPin(RST_PORT, RST_PIN);
    
    GPIO_setOutputLowOnPin(RST_PORT, RST_PIN);
    delay_ms(100);
    GPIO_setOutputHighOnPin(RST_PORT, RST_PIN);
    delay_ms(100);
    
    initSPI();
    
    GPIO_setAsOutputPin(CS_PORT, CS_PIN);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
    
    spiWrite(REG_01_OP_MODE, MODE_SLEEP);
    spiWrite(REG_01_OP_MODE, LONG_RANGE_MODE);
    uint8_t val = spiRead(REG_01_OP_MODE);
    
    if (val != (LONG_RANGE_MODE)){
        return false;
    }
    
    _mode = RFModeSleep;
    
    //GPIO_setAsInputPin(INT_PORT, INT_PIN);
    MAP_GPIO_setAsInputPinWithPullDownResistor(INT_PORT, INT_PIN);
    MAP_GPIO_clearInterruptFlag(INT_PORT, INT_PIN);
    GPIO_interruptEdgeSelect(INT_PORT, INT_PIN, GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_enableInterrupt(INT_PORT, INT_PIN);
    
    MAP_Interrupt_setPriority(INT_PORT + 50, 0x40);
    MAP_Interrupt_enableInterrupt(INT_PORT + 50);
    MAP_Interrupt_enableMaster();
    
    spiWrite(REG_0E_FIFO_TX_BASE_ADDR, 0);
    spiWrite(REG_0F_FIFO_RX_BASE_ADDR, 0);
    
    setModeIdle();
    setModemConfig(Bw125Cr45Sf128); //default
    //setModemConfig(Bw125Cr48Sf4096); // slow and reliable?
    
    setPreambleLength(10); // Default is 8
    setFrequency(DEFAULT_FREQ);
    setTxPower(DEFAULT_POWER, false);
    
    return true;
}

/******************************************************************************
* Function: Main
****************
* Main program entry point
*
* Parameters: Null
*
* Returns: Null
******************************************************************************/
void setTxPower(int8_t power, bool useRFO)
{
    // Sigh, different behaviours depending on whther the module use PA_BOOST or the RFO pin
    // for the transmitter output
    if (useRFO)
    {
    	if (power > 14)
    	    power = 14;
    	if (power < -1)
    	    power = -1;
	    spiWrite(REG_09_PA_CONFIG,MAX_POWER | (power + 1));
    }
    else
    {
    	if (power > 23)
    	    power = 23;
    	if (power < 5)
    	    power = 5;

    	// For RH_RF95_PA_DAC_ENABLE, manual says '+20dBm on PA_BOOST when OutputPower=0xf'
    	// RH_RF95_PA_DAC_ENABLE actually adds about 3dBm to all power levels. We will us it
    	// for 21, 22 and 23dBm
    	if (power > 20)
    	{
    	    spiWrite(REG_4D_PA_DAC, PA_DAC_ENABLE);
    	    spiWrite(REG_0B_OCP, OCP_ON | OCP_TRIM);
    	    power -= 3;
    	}
    	else
    	{
    	    spiWrite(REG_4D_PA_DAC, PA_DAC_DISABLE);
    	}

    	// RFM95/96/97/98 does not have RFO pins connected to anything. Only PA_BOOST
    	// pin is connected, so must use PA_BOOST
    	// Pout = 2 + OutputPower.
    	// The documentation is pretty confusing on this topic: PaSelect says the max power is 20dBm,
    	// but OutputPower claims it would be 17dBm.
    	// My measurements show 20dBm is correct
    	spiWrite(REG_09_PA_CONFIG, PA_SELECT | (0x0f));//power-5));
    }
}

bool setFrequency(float centre){
    // Frf = FRF / FSTEP
    uint32_t frf = (centre * 1000000.0) / FSTEP;
    spiWrite(REG_06_FRF_MSB, (frf >> 16) & 0xff);
    spiWrite(REG_07_FRF_MID, (frf >> 8) & 0xff);
    spiWrite(REG_08_FRF_LSB, frf & 0xff);

    return true;
}

// bool send(const uint8_t* data, uint8_t len)
// {
//     if (len > MAX_MESSAGE_LEN)
// 	    return false;

//     waitPacketSent(); // Make sure we dont interrupt an outgoing message
//     setModeIdle();

//     if (!waitCAD()) 
// 	return false;  // Check channel activity

//     // Position at the beginning of the FIFO
//     spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);
//     // The headers
//     spiWrite(RH_RF95_REG_00_FIFO, _txHeaderTo);
//     spiWrite(RH_RF95_REG_00_FIFO, _txHeaderFrom);
//     spiWrite(RH_RF95_REG_00_FIFO, _txHeaderId);
//     spiWrite(RH_RF95_REG_00_FIFO, _txHeaderFlags);
//     // The message data
//     spiBurstWrite(RH_RF95_REG_00_FIFO, data, len);
//     spiWrite(RH_RF95_REG_22_PAYLOAD_LENGTH, len + RH_RF95_HEADER_LEN);

//     setModeTx(); // Start the transmitter
//     // when Tx is done, interruptHandler will fire and radio mode will return to STANDBY
//     return true;
// }

bool waitPacketSent(){
    while (_mode == RFModeTx);
    return true;
}

void setPreambleLength(uint16_t bytes){
    spiWrite(REG_20_PREAMBLE_MSB, bytes >> 8);
    spiWrite(REG_21_PREAMBLE_LSB, bytes & 0xff);
}

bool setModemConfig(ModemConfigChoice index){
    if (index > (signed int)(sizeof(MODEM_CONFIG_TABLE)/sizeof(ModemConfig)))
        return false;
        
    ModemConfig cfg;
    memcpy(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(ModemConfig));
    setModemRegisters(&cfg);
    
    return true;
}

void setModemRegisters(const ModemConfig* config){
    spiWrite(REG_1D_MODEM_CONFIG1, config->reg_1d);
    spiWrite(REG_1E_MODEM_CONFIG2, config->reg_1e);
    spiWrite(REG_26_MODEM_CONFIG3, config->reg_26);
}

uint8_t spiRead(uint8_t reg){
    uint8_t val = 0;
    GPIO_setOutputLowOnPin(CS_PORT, CS_PIN);
    SPI_transmitData(EUSCI_B0_BASE, reg & ~RF_SPI_WRITE_MASK);
    SPI_transmitData(EUSCI_B0_BASE, 0);
    delay_ms(5);
    val = SPI_receiveData(EUSCI_B0_BASE);
    delay_ms(10);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
    return val;
}

void spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len){
    GPIO_setOutputLowOnPin(CS_PORT, CS_PIN);
    SPI_transmitData(EUSCI_B0_BASE, reg & ~RF_SPI_WRITE_MASK);
    while (len--){
        SPI_transmitData(EUSCI_B0_BASE, 0);
        delay_ms(2);
        *dest++ = SPI_receiveData(EUSCI_B0_BASE);
    }
    delay_ms(10);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
}

void spiWrite(uint8_t reg, uint8_t val){
    GPIO_setOutputLowOnPin(CS_PORT, CS_PIN);
    SPI_transmitData(EUSCI_B0_BASE, reg | RF_SPI_WRITE_MASK);
    SPI_transmitData(EUSCI_B0_BASE, val);
    delay_ms(10);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
}

void spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len)
{
    GPIO_setOutputLowOnPin(CS_PORT, CS_PIN);
    SPI_transmitData(EUSCI_B0_BASE, reg | RF_SPI_WRITE_MASK);
    while (len--){
        SPI_transmitData(EUSCI_B0_BASE, *src++);
        delay_ms(1);
    }
    delay_ms(10);
    GPIO_setOutputHighOnPin(CS_PORT, CS_PIN);
}

bool available(){
    if (_mode == RFModeTx){
        return false;
    }
    else if (_mode != RFModeRx){
        setModeRx();
    }
    if (_rxBufValid)
        return true;
    else
        return false;
}

bool waitAvailableTimeout(uint16_t timeout){
    uint32_t startTime = getTotal_ms();
    while (getTotal_ms() - startTime < timeout){
        if (available()){
            return true;
        }
    }
    return false;
}

void setModeIdle(){
    if (_mode != RFModeIdle){
        spiWrite(REG_01_OP_MODE,MODE_STDBY);
        _mode = RFModeIdle;
    }
}

void setModeRx(){
    if (_mode != RFModeRx){
    	spiWrite(REG_01_OP_MODE, MODE_RXCONTINUOUS);
        spiWrite(REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone

	    _mode = RFModeRx;
    }
}

void setModeTx(){
    if (_mode != RFModeTx){
	    spiWrite(REG_01_OP_MODE, MODE_TX);
	    spiWrite(REG_40_DIO_MAPPING1, 0x40); // Interrupt on TxDone
	    _mode = RFModeTx;
    }
}

void setModeSleep(){
	while (_mode != RFModeIdle);
	spiWrite(REG_01_OP_MODE, MODE_SLEEP);
	_mode = RFModeSleep;
}

void validateRxBuf(){
    if (_bufLen < 4)
	    return; // Too short to be a real message
    // Extract the 4 headers
    _rxHeaderTo    = _buf[0];
    _rxHeaderFrom  = _buf[1];
    _rxHeaderId    = _buf[2];
    _rxHeaderFlags = _buf[3];
    if (_promiscuous || _rxHeaderTo == _thisAddress || _rxHeaderTo == 255){
	    _rxGood++;
	    _rxBufValid = true;
    }
}

bool sendBuffer(const uint8_t* data, uint8_t len)
{
    if (len > MAX_MESSAGE_LEN)
	    return false;

    waitPacketSent(); // Make sure we dont interrupt an outgoing message
    setModeIdle();

    // Position at the beginning of the FIFO
    spiWrite(REG_0D_FIFO_ADDR_PTR, 0);
    // The headers
    spiWrite(REG_00_FIFO, _txHeaderTo);
    spiWrite(REG_00_FIFO, _txHeaderFrom);
    spiWrite(REG_00_FIFO, _txHeaderId);
    spiWrite(REG_00_FIFO, _txHeaderFlags);
    // The message data
    spiBurstWrite(REG_00_FIFO, data, len);
    spiWrite(REG_22_PAYLOAD_LENGTH, len + HEADER_LEN);

    setModeTx(); // Start the transmitter
    // when Tx is done, interruptHandler will fire and radio mode will return to STANDBY
    return true;
}

bool receiveBuffer (uint8_t* buf, uint8_t* len){
    if (!available())
        return false;
    
    if (buf && len){
        if (*len > _bufLen-HEADER_LEN)
            *len = _bufLen-HEADER_LEN;
        memcpy(buf, _buf+HEADER_LEN, *len);
    }
    clearRxBuf();
    return true;
}

void clearRxBuf(){
    _rxBufValid = false;
    _bufLen = 0;
}

void PORT2_IRQHandler(void)
{
    uint32_t status;
    status = MAP_GPIO_getEnabledInterruptStatus(INT_PORT);
    if (status == INT_PIN){
        MAP_GPIO_clearInterruptFlag(INT_PORT, status);
        uint8_t irq_flags = spiRead(REG_12_IRQ_FLAGS);
        
        if (_mode == RFModeRx && irq_flags & (RX_TIMEOUT | PAYLOAD_CRC_ERROR)){
    	    _rxBad++;
        }
        else if (_mode == RFModeRx && irq_flags & RX_DONE){
            // Have received a packet
    	    uint8_t len = spiRead(REG_13_RX_NB_BYTES);
    	    
    	    // Reset the fifo read ptr to the beginning of the packet
        	spiWrite(REG_0D_FIFO_ADDR_PTR, spiRead(REG_10_FIFO_RX_CURRENT_ADDR));
        	spiBurstRead(REG_00_FIFO, _buf, len);
        	_bufLen = len;
        	spiWrite(REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
        	
        	// Remember the RSSI of this packet
        	// this is according to the doc, but is it really correct?
        	// weakest receiveable signals are reported RSSI at about -66
        	_lastRssi = spiRead(REG_1A_PKT_RSSI_VALUE) - 137;
        	
        	// We have received a message.
        	validateRxBuf(); 
    	    if (_rxBufValid)
    	        setModeIdle(); // Got one 
        }
        else if (_mode == RFModeTx && irq_flags & TX_DONE){
            _txGood++;
            setModeIdle();
        }
        else if (_mode == RFModeCad && irq_flags & CAD_DONE){
            _cad = irq_flags & CAD_DETECTED;
            setModeIdle();
        }
        spiWrite(REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
    }
}
