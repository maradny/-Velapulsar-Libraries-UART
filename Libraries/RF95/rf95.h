/*
 * Description: Radio Module header file
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

#ifndef rf95_h
#define rf95_h

/*****************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include "driverlib.h"
#include "spi_interface.h"
#include "delay.h"
#include "radio.h"
#include "math.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
#define RST_PORT    GPIO_PORT_P5
#define RST_PIN     GPIO_PIN2
#define CS_PORT     GPIO_PORT_P4
#define CS_PIN      GPIO_PIN6
#define INT_PORT    GPIO_PORT_P2
#define INT_PIN     GPIO_PIN5

#define DEFAULT_FREQ    915.0
#define DEFAULT_POWER   23

// This is the maximum number of interrupts the driver can support
// Most Arduinos can handle 2, Megas can handle more
//#define NUM_INTERRUPTS 3

// Max number of octets the LORA Rx/Tx FIFO can hold
#define FIFO_SIZE 255

// This is the maximum number of bytes that can be carried by the LORA.
// We use some for headers, keeping fewer for RadioHead messages
#define MAX_PAYLOAD_LEN FIFO_SIZE

// The length of the headers we add.
// The headers are inside the LORA's payload
#define HEADER_LEN 4

// This is the maximum message length that can be supported by this driver. 
// Can be pre-defined to a smaller size (to save SRAM) prior to including this header
// Here we allow for 1 byte message length, 4 bytes headers, user data and 2 bytes of FCS
#ifndef MAX_MESSAGE_LEN
 #define MAX_MESSAGE_LEN (MAX_PAYLOAD_LEN - HEADER_LEN)
#endif

// The crystal oscillator frequency of the module
#define FXOSC 32000000.0

// The Frequency Synthesizer step = FXOSC / 2^^19
#define FSTEP  (FXOSC / 524288)

#define RF_MID_BAND_THRESH                          525

/*!
 * Constant values need to compute the RSSI value
 */
#define RSSI_OFFSET_LF                              -164
#define RSSI_OFFSET_HF                              -157

#define RF_SPI_WRITE_MASK                           0x80

// Register names (LoRa Mode, from table 85)
#define REG_00_FIFO                                 0x00
#define REG_01_OP_MODE                              0x01
#define REG_02_RESERVED                             0x02
#define REG_03_RESERVED                             0x03
#define REG_04_RESERVED                             0x04
#define REG_05_RESERVED                             0x05
#define REG_06_FRF_MSB                              0x06
#define REG_07_FRF_MID                              0x07
#define REG_08_FRF_LSB                              0x08
#define REG_09_PA_CONFIG                            0x09
#define REG_0A_PA_RAMP                              0x0a
#define REG_0B_OCP                                  0x0b
#define REG_0C_LNA                                  0x0c
#define REG_0D_FIFO_ADDR_PTR                        0x0d
#define REG_0E_FIFO_TX_BASE_ADDR                    0x0e
#define REG_0F_FIFO_RX_BASE_ADDR                    0x0f
#define REG_10_FIFO_RX_CURRENT_ADDR                 0x10
#define REG_11_IRQ_FLAGS_MASK                       0x11
#define REG_12_IRQ_FLAGS                            0x12
#define REG_13_RX_NB_BYTES                          0x13
#define REG_14_RX_HEADER_CNT_VALUE_MSB              0x14
#define REG_15_RX_HEADER_CNT_VALUE_LSB              0x15
#define REG_16_RX_PACKET_CNT_VALUE_MSB              0x16
#define REG_17_RX_PACKET_CNT_VALUE_LSB              0x17
#define REG_18_MODEM_STAT                           0x18
#define REG_19_PKT_SNR_VALUE                        0x19
#define REG_1A_PKT_RSSI_VALUE                       0x1a
#define REG_1B_RSSI_VALUE                           0x1b
#define REG_1C_HOP_CHANNEL                          0x1c
#define REG_1D_MODEM_CONFIG1                        0x1d
#define REG_1E_MODEM_CONFIG2                        0x1e
#define REG_1F_SYMB_TIMEOUT_LSB                     0x1f
#define REG_20_PREAMBLE_MSB                         0x20
#define REG_21_PREAMBLE_LSB                         0x21
#define REG_22_PAYLOAD_LENGTH                       0x22
#define REG_23_MAX_PAYLOAD_LENGTH                   0x23
#define REG_24_HOP_PERIOD                           0x24
#define REG_25_FIFO_RX_BYTE_ADDR                    0x25
#define REG_26_MODEM_CONFIG3                        0x26

#define REG_40_DIO_MAPPING1                         0x40
#define REG_41_DIO_MAPPING2                         0x41
#define REG_42_VERSION                              0x42

#define REG_4B_TCXO                                 0x4b
#define REG_4D_PA_DAC                               0x4d
#define REG_5B_FORMER_TEMP                          0x5b
#define REG_61_AGC_REF                              0x61
#define REG_62_AGC_THRESH1                          0x62
#define REG_63_AGC_THRESH2                          0x63
#define REG_64_AGC_THRESH3                          0x64

// REG_01_OP_MODE                             0x01
#define LONG_RANGE_MODE                       0x80
#define ACCESS_SHARED_REG                     0x40
#define MODE                                  0x07
#define MODE_SLEEP                            0x00
#define MODE_STDBY                            0x01
#define MODE_FSTX                             0x02
#define MODE_TX                               0x03
#define MODE_FSRX                             0x04
#define MODE_RXCONTINUOUS                     0x05
#define MODE_RXSINGLE                         0x06
#define MODE_CAD                              0x07

// REG_09_PA_CONFIG                           0x09
#define PA_SELECT                             0x80
#define MAX_POWER                             0x70
#define OUTPUT_POWER                          0x0f

// REG_0A_PA_RAMP                             0x0a
#define LOW_PN_TX_PLL_OFF                     0x10
#define PA_RAMP                               0x0f
#define PA_RAMP_3_4MS                         0x00
#define PA_RAMP_2MS                           0x01
#define PA_RAMP_1MS                           0x02
#define PA_RAMP_500US                         0x03
#define PA_RAMP_250US                         0x0
#define PA_RAMP_125US                         0x05
#define PA_RAMP_100US                         0x06
#define PA_RAMP_62US                          0x07
#define PA_RAMP_50US                          0x08
#define PA_RAMP_40US                          0x09
#define PA_RAMP_31US                          0x0a
#define PA_RAMP_25US                          0x0b
#define PA_RAMP_20US                          0x0c
#define PA_RAMP_15US                          0x0d
#define PA_RAMP_12US                          0x0e
#define PA_RAMP_10US                          0x0f

// REG_0B_OCP                                 0x0b
#define OCP_ON                                0x20
#define OCP_TRIM                              0x1f

// REG_0C_LNA                                 0x0c
#define LNA_GAIN                              0xe0
#define LNA_BOOST                             0x03
#define LNA_BOOST_DEFAULT                     0x00
#define LNA_BOOST_150PC                       0x11

// REG_11_IRQ_FLAGS_MASK                      0x11
#define RX_TIMEOUT_MASK                       0x80
#define RX_DONE_MASK                          0x40
#define PAYLOAD_CRC_ERROR_MASK                0x20
#define VALID_HEADER_MASK                     0x10
#define TX_DONE_MASK                          0x08
#define CAD_DONE_MASK                         0x04
#define FHSS_CHANGE_CHANNEL_MASK              0x02
#define CAD_DETECTED_MASK                     0x01

// REG_12_IRQ_FLAGS                           0x12
#define RX_TIMEOUT                            0x80
#define RX_DONE                               0x40
#define PAYLOAD_CRC_ERROR                     0x20
#define VALID_HEADER                          0x10
#define TX_DONE                               0x08
#define CAD_DONE                              0x04
#define FHSS_CHANGE_CHANNEL                   0x02
#define CAD_DETECTED                          0x01

// REG_18_MODEM_STAT                          0x18
#define RX_CODING_RATE                        0xe0
#define MODEM_STATUS_CLEAR                    0x10
#define MODEM_STATUS_HEADER_INFO_VALID        0x08
#define MODEM_STATUS_RX_ONGOING               0x04
#define MODEM_STATUS_SIGNAL_SYNCHRONIZED      0x02
#define MODEM_STATUS_SIGNAL_DETECTED          0x01

// REG_1C_HOP_CHANNEL                         0x1c
#define PLL_TIMEOUT                           0x80
#define RX_PAYLOAD_CRC_IS_ON                  0x40
#define FHSS_PRESENT_CHANNEL                  0x3f

// REG_1D_MODEM_CONFIG1                       0x1d
#define BW                                    0xc0
#define BW_125KHZ                             0x00
#define BW_250KHZ                             0x40
#define BW_500KHZ                             0x80
#define BW_RESERVED                           0xc0
#define CODING_RATE                           0x38
#define CODING_RATE_4_5                       0x00
#define CODING_RATE_4_6                       0x08
#define CODING_RATE_4_7                       0x10
#define CODING_RATE_4_8                       0x18
#define IMPLICIT_HEADER_MODE_ON               0x04
#define RX_PAYLOAD_CRC_ON                     0x02
#define LOW_DATA_RATE_OPTIMIZE                0x01

// REG_1E_MODEM_CONFIG2                       0x1e
#define SPREADING_FACTOR                      0xf0
#define SPREADING_FACTOR_64CPS                0x60
#define SPREADING_FACTOR_128CPS               0x70
#define SPREADING_FACTOR_256CPS               0x80
#define SPREADING_FACTOR_512CPS               0x90
#define SPREADING_FACTOR_1024CPS              0xa0
#define SPREADING_FACTOR_2048CPS              0xb0
#define SPREADING_FACTOR_4096CPS              0xc0
#define TX_CONTINUOUS_MOE                     0x08
#define AGC_AUTO_ON                           0x04
#define SYM_TIMEOUT_MSB                       0x03

// REG_4D_PA_DAC                              0x4d
#define PA_DAC_DISABLE                        0x84//0x04
#define PA_DAC_ENABLE                         0x87//0x07

/*!
 * Radio wakeup time from SLEEP mode
 */
#define RADIO_OSC_STARTUP                           1 // [ms]

/*!
 * Radio PLL lock and Mode Ready delay which can vary with the temperature
 */
#define RADIO_SLEEP_TO_RX                           2 // [ms]

/*!
 * Radio complete Wake-up Time with margin for temperature compensation
 */
#define RADIO_WAKEUP_TIME                           ( RADIO_OSC_STARTUP + RADIO_SLEEP_TO_RX )

/*!
 * Radio LoRa modem parameters
 */
typedef struct
{
    int8_t   Power;
    uint32_t Bandwidth;
    uint32_t Datarate;
    bool     LowDatarateOptimize;
    uint8_t  Coderate;
    uint16_t PreambleLen;
    bool     FixLen;
    uint8_t  PayloadLen;
    bool     CrcOn;
    bool     RxContinuous;
    uint32_t TxTimeout;
}RadioLoRaSettings_t;

/*!
 * Radio LoRa packet handler state
 */
typedef struct
{
    int8_t SnrValue;
    int16_t RssiValue;
    uint8_t Size;
}RadioLoRaPacketHandler_t;

/*!
 * Radio Settings
 */
typedef struct
{
    RadioState_t             State;
    RadioModems_t            Modem;
    uint32_t                 Channel;
    RadioLoRaSettings_t      LoRa;
    RadioLoRaPacketHandler_t LoRaPacketHandler;
}RadioSettings_t;

/*******************************************************************************
 * 							Node specific definitions
 ******************************************************************************/
    

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/

/// Initialise the Driver transport hardware and software.
/// Make sure the Driver is properly configured before calling init().
/// \return true if initialisation succeeded.
bool    RFInit(RadioEvents_t *events);

/*!
 * Return current radio status
 *
 * \param status Radio status.[RF_IDLE, RF_RX_RUNNING, RF_TX_RUNNING]
 */
RadioState_t RFGetStatus(void);

/*!
 * \brief Configures the radio with the given modem
 *
 * \param [IN] modem Modem to be used [0: FSK, 1: LoRa]
 */
bool RFInitModem( RadioModems_t modem );

/*!
 * \brief Sets the channels configuration
 *
 * \param [IN] freq         Channel RF frequency
 */
void RFSetChannel( uint32_t freq );

/*!
 * \brief Sets the channels configuration
 *
 * \param [IN] modem      Radio modem to be used [0: FSK, 1: LoRa]
 * \param [IN] freq       Channel RF frequency
 * \param [IN] rssiThresh RSSI threshold
 *
 * \retval isFree         [true: Channel is free, false: Channel is not free]
 */
bool RFIsChannelFree (uint32_t freq, int16_t rssiThresh);

/*!
 * \brief Generates a 32 bits random value based on the RSSI readings
 *
 * \remark This function sets the radio in LoRa modem mode and disables
 *         all interrupts.
 *         After calling this function either SX1276SetRxConfig or
 *         SX1276SetTxConfig functions must be called.
 *
 * \retval randomValue    32 bits random value
 */
uint32_t RFRandom( void );

/*!
 * \brief Sets the reception parameters
 *
 * \remark When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
 *
 * \param [IN] modem        Radio modem to be used [0: FSK, 1: LoRa]
 * \param [IN] bandwidth    Sets the bandwidth
 *                          FSK : >= 2600 and <= 250000 Hz
 *                          LoRa: [0: 125 kHz, 1: 250 kHz,
 *                                 2: 500 kHz, 3: Reserved]
 * \param [IN] datarate     Sets the Datarate
 *                          FSK : 600..300000 bits/s
 *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
 *                                10: 1024, 11: 2048, 12: 4096  chips]
 * \param [IN] coderate     Sets the coding rate (LoRa only)
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
 * \param [IN] bandwidthAfc Sets the AFC Bandwidth (FSK only)
 *                          FSK : >= 2600 and <= 250000 Hz
 *                          LoRa: N/A ( set to 0 )
 * \param [IN] preambleLen  Sets the Preamble length
 *                          FSK : Number of bytes
 *                          LoRa: Length in symbols (the hardware adds 4 more symbols)
 * \param [IN] symbTimeout  Sets the RxSingle timeout value (LoRa only)
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: timeout in symbols
 * \param [IN] fixLen       Fixed length packets [0: variable, 1: fixed]
 * \param [IN] payloadLen   Sets payload length when fixed lenght is used
 * \param [IN] crcOn        Enables/Disables the CRC [0: OFF, 1: ON]
 * \param [IN] FreqHopOn    Enables disables the intra-packet frequency hopping
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: [0: OFF, 1: ON]
 * \param [IN] HopPeriod    Number of symbols bewteen each hop
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: Number of symbols
 * \param [IN] iqInverted   Inverts IQ signals (LoRa only)
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: [0: not inverted, 1: inverted]
 * \param [IN] rxContinuous Sets the reception in continuous mode
 *                          [false: single mode, true: continuous mode]
 */
void RFSetRxConfig( uint32_t bandwidth, uint32_t datarate,
					uint8_t coderate, uint16_t preambleLen,
					uint16_t symbTimeout, bool fixLen,
					uint8_t payloadLen,	bool crcOn, bool rxContinuous);

/*!
 * \brief Sets the transmission parameters
 *
 * \remark When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
 *
 * \param [IN] modem        Radio modem to be used [0: FSK, 1: LoRa]
 * \param [IN] power        Sets the output power [dBm]
 * \param [IN] fdev         Sets the frequency deviation (FSK only)
 *                          FSK : [Hz]
 *                          LoRa: 0
 * \param [IN] bandwidth    Sets the bandwidth (LoRa only)
 *                          FSK : 0
 *                          LoRa: [0: 125 kHz, 1: 250 kHz,
 *                                 2: 500 kHz, 3: Reserved]
 * \param [IN] datarate     Sets the Datarate
 *                          FSK : 600..300000 bits/s
 *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
 *                                10: 1024, 11: 2048, 12: 4096  chips]
 * \param [IN] coderate     Sets the coding rate (LoRa only)
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
 * \param [IN] preambleLen  Sets the preamble length
 *                          FSK : Number of bytes
 *                          LoRa: Length in symbols (the hardware adds 4 more symbols)
 * \param [IN] fixLen       Fixed length packets [0: variable, 1: fixed]
 * \param [IN] crcOn        Enables disables the CRC [0: OFF, 1: ON]
 * \param [IN] FreqHopOn    Enables disables the intra-packet frequency hopping
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: [0: OFF, 1: ON]
 * \param [IN] HopPeriod    Number of symbols bewteen each hop
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: Number of symbols
 * \param [IN] iqInverted   Inverts IQ signals (LoRa only)
 *                          FSK : N/A ( set to 0 )
 *                          LoRa: [0: not inverted, 1: inverted]
 * \param [IN] timeout      Transmission timeout [ms]
 */
void RFSetTxConfig(int8_t power, uint32_t bandwidth, uint32_t datarate,
                        uint8_t coderate, uint16_t preambleLen,
                        bool fixLen, bool crcOn, uint32_t timeout );

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool RFCheckRfFrequency( uint32_t frequency );

/*!
 * \brief Computes the packet time on air in us for the given payload
 *
 * \Remark Can only be called once SetRxConfig or SetTxConfig have been called
 *
 * \param [IN] modem      Radio modem to be used [0: FSK, 1: LoRa]
 * \param [IN] pktLen     Packet payload length
 *
 * \retval airTime        Computed airTime (us) for the given packet payload length
 */
uint32_t RFGetTimeOnAir(uint8_t pktLen );

/*!
 * \brief Sends the buffer of size. Prepares the packet to be sent and sets
 *        the radio in transmission
 *
 * \param [IN]: buffer     Buffer pointer
 * \param [IN]: size       Buffer size
 */
void RFSend( uint8_t *buffer, uint8_t size );

/*!
 * \brief Sets the radio in sleep mode
 */
void RFSetSleep( void );

/*!
 * \brief Sets the radio in standby mode
 */
void RFSetStby( void );

/*!
 * \brief Sets the radio in reception mode for the given time
 * \param [IN] timeout Reception timeout [ms] [0: continuous, others timeout]
 */
void RFSetRx( uint32_t timeout );

/*!
 * \brief Start a Channel Activity Detection
 */
void RFStartCad( void );

/*!
 * \brief Reads the current RSSI value
 *
 * \retval rssiValue Current RSSI value in [dBm]
 */
int16_t RFReadRssi(void);

/*!
 * \brief Writes the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \param [IN]: data New register value
 */
void RFWrite( uint8_t addr, uint8_t data );

/*!
 * \brief Reads the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \retval data Register value
 */
uint8_t RFRead( uint8_t addr );

/*!
 * \brief Writes multiple radio registers starting at address
 *
 * \param [IN] addr   First Radio register address
 * \param [IN] buffer Buffer containing the new register's values
 * \param [IN] size   Number of registers to be written
 */
void RFWriteBuffer( uint8_t reg, uint8_t* src, uint8_t len );

/*!
 * \brief Reads multiple radio registers starting at address
 *
 * \param [IN] addr First Radio register address
 * \param [OUT] buffer Buffer where to copy the registers data
 * \param [IN] size Number of registers to be read
 */
void RFReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );

/*!
 * \brief Sets the maximum payload length.
 *
 * \param [IN] modem      Radio modem to be used [0: FSK, 1: LoRa]
 * \param [IN] max        Maximum payload length in bytes
 */
void RFSetMaxPayloadLength( RadioModems_t modem, uint8_t max );

#endif

