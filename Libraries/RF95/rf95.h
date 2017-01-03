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

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
#define RST_PORT    GPIO_PORT_P5
#define RST_PIN     GPIO_PIN2
#define CS_PORT     GPIO_PORT_P4
#define CS_PIN      GPIO_PIN6
#define INT_PORT    GPIO_PORT_P2
#define INT_PIN     GPIO_PIN5

#define RF_BROADCAST_ADDRESS 0xFF

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

typedef struct{
    uint8_t    reg_1d;   ///< Value for register REG_1D_MODEM_CONFIG1
    uint8_t    reg_1e;   ///< Value for register REG_1E_MODEM_CONFIG2
    uint8_t    reg_26;   ///< Value for register REG_26_MODEM_CONFIG3
} ModemConfig;
    
typedef enum{
    Bw125Cr45Sf128 = 0,    ///< Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
    Bw500Cr45Sf128,            ///< Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
    Bw31_25Cr48Sf512,      ///< Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
    Bw125Cr48Sf4096,           ///< Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range
} ModemConfigChoice;

typedef enum{
	RFModeInitialising = 0, ///< Transport is initialising. Initial default value until init() is called..
	RFModeSleep,            ///< Transport hardware is in low power sleep mode (if supported)
	RFModeIdle,             ///< Transport is idle.
	RFModeTx,               ///< Transport is in the process of transmitting a message.
	RFModeRx,               ///< Transport is in the process of receiving a message.
	RFModeCad               ///< Transport is in the process of detecting channel activity (if supported)
} RFMode;

/*******************************************************************************
 * Node specific definitions 
 ******************************************************************************/
/// The current transport operating mode
volatile RFMode     _mode;

/// This node id
uint8_t             _thisAddress;

/// Whether the transport is in promiscuous mode
bool                _promiscuous;

/// TO header in the last received mesasge
volatile uint8_t    _rxHeaderTo;

/// FROM header in the last received mesasge
volatile uint8_t    _rxHeaderFrom;

/// ID header in the last received mesasge
volatile uint8_t    _rxHeaderId;

/// FLAGS header in the last received mesasge
volatile uint8_t    _rxHeaderFlags;

/// TO header to send in all messages
uint8_t             _txHeaderTo;

/// FROM header to send in all messages
uint8_t             _txHeaderFrom;

/// ID header to send in all messages
uint8_t             _txHeaderId;

/// FLAGS header to send in all messages
uint8_t             _txHeaderFlags;

/// The value of the last received RSSI value, in some transport specific units
volatile int8_t     _lastRssi;

/// Count of the number of bad messages (eg bad checksum etc) received
volatile uint16_t   _rxBad;

/// Count of the number of successfully transmitted messaged
volatile uint16_t   _rxGood;

/// Count of the number of bad messages (correct checksum etc) received
volatile uint16_t   _txGood;

/// Channel activity detected
volatile bool       _cad;
unsigned int        _cad_timeout;
    

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/

/// Initialise the Driver transport hardware and software.
/// Make sure the Driver is properly configured before calling init().
/// \return true if initialisation succeeded.
bool    initRF(void);

/// Prints the value of all chip registers
/// to the Serial device if RH_HAVE_SERIAL is defined for the current platform
/// For debugging purposes only.
/// \return true on success
bool printRegisters(void);

/// Sets all the registered required to configure the data modem in the RF95/96/97/98, including the bandwidth, 
/// spreading factor etc. You can use this to configure the modem with custom configurations if none of the 
/// canned configurations in ModemConfigChoice suit you.
/// \param[in] config A ModemConfig structure containing values for the modem configuration registers.
void           setModemRegisters(const ModemConfig* config);

/// Select one of the predefined modem configurations. If you need a modem configuration not provided 
/// here, use setModemRegisters() with your own ModemConfig.
/// \param[in] index The configuration choice.
/// \return true if index is a valid choice.
bool        setModemConfig(ModemConfigChoice index);

/// Tests whether a new message is available
/// from the Driver. 
/// On most drivers, this will also put the Driver into RHModeRx mode until
/// a message is actually received by the transport, when it wil be returned to RHModeIdle.
/// This can be called multiple times in a timeout loop
/// \return true if a new, complete, error-free uncollected message is available to be retreived by recv()
bool    available();

/// Turns the receiver on if it not already on.
/// If there is a valid message available, copy it to buf and return true
/// else return false.
/// If a message is copied, *len is set to the length (Caution, 0 length messages are permitted).
/// You should be sure to call this function frequently enough to not miss any messages
/// It is recommended that you call it in your main loop.
/// \param[in] buf Location to copy the received message
/// \param[in,out] len Pointer to available space in buf. Set to the actual number of octets copied.
/// \return true if a valid message was copied to buf
bool    recv(uint8_t* buf, uint8_t* len);

/// Waits until any previous transmit packet is finished being transmitted with waitPacketSent().
/// Then optionally waits for Channel Activity Detection (CAD) 
/// to show the channnel is clear (if the radio supports CAD) by calling waitCAD().
/// Then loads a message into the transmitter and starts the transmitter. Note that a message length
/// of 0 is permitted. 
/// \param[in] data Array of data to be sent
/// \param[in] len Number of bytes of data to send
/// specify the maximum time in ms to wait. If 0 (the default) do not wait for CAD before transmitting.
/// \return true if the message length was valid and it was correctly queued for transmit. Return false
/// if CAD was requested and the CAD timeout timed out before clear channel was detected.
bool    send(const uint8_t* data, uint8_t len);

/// Sets the length of the preamble
/// in bytes. 
/// Caution: this should be set to the same 
/// value on all nodes in your network. Default is 8.
/// Sets the message preamble length in REG_??_PREAMBLE_?SB
/// \param[in] bytes Preamble length in bytes.  
void           setPreambleLength(uint16_t bytes);

/// Returns the maximum message length 
/// available in this Driver.
/// \return The maximum legal message length
uint8_t maxMessageLength();

/// Sets the transmitter and receiver 
/// centre frequency.
/// \param[in] centre Frequency in MHz. 137.0 to 1020.0. Caution: RFM95/96/97/98 comes in several
/// different frequency ranges, and setting a frequency outside that range of your radio will probably not work
/// \return true if the selected frquency centre is within range
bool        setFrequency(float centre);

/// If current mode is Rx or Tx changes it to Idle. If the transmitter or receiver is running, 
/// disables them.
void           setModeIdle();

/// If current mode is Tx or Idle, changes it to Rx. 
/// Starts the receiver in the RF95/96/97/98.
void           setModeRx();

/// If current mode is Rx or Idle, changes it to Rx. F
/// Starts the transmitter in the RF95/96/97/98.
void           setModeTx();

/// Wait until idle then sleep
void		   setModeSleep();

/// Sets the transmitter power output level, and configures the transmitter pin.
/// Be a good neighbour and set the lowest power level you need.
/// Some SX1276/77/78/79 and compatible modules (such as RFM95/96/97/98) 
/// use the PA_BOOST transmitter pin for high power output (and optionally the PA_DAC)
/// while some (such as the Modtronix inAir4 and inAir9) 
/// use the RFO transmitter pin for lower power but higher efficiency.
/// You must set the appropriate power level and useRFO argument for your module.
/// Check with your module manufacturer which transmtter pin is used on your module
/// to ensure you are setting useRFO correctly. 
/// Failure to do so will result in very low 
/// transmitter power output.
/// Caution: legal power limits may apply in certain countries.
/// After init(), the power will be set to 13dBm, with useRFO false (ie PA_BOOST enabled).
/// \param[in] power Transmitter power level in dBm. For RFM95/96/97/98 LORA with useRFO false, 
/// valid values are from +5 to +23.
/// For Modtronix inAir4 and inAir9 with useRFO true (ie RFO pins in use), 
/// valid values are from -1 to 14.
/// \param[in] useRFO If true, enables the use of the RFO transmitter pins instead of
/// the PA_BOOST pin (false). Choose the correct setting for your module.
void            setTxPower(int8_t power, bool useRFO);

/// Galal Hassan (galalmounir@gmail.com), 10/25/2016
/// Transmits on SPI
void            spiWrite(uint8_t reg, uint8_t val);

/// Galal Hassan (galalmounir@gmail.com), 10/25/2016
/// Reads from SPI
uint8_t         spiRead(uint8_t reg);

/// Sets the radio into low-power sleep mode.
/// If successful, the transport will stay in sleep mode until woken by 
/// changing mode it idle, transmit or receive (eg by calling send(), recv(), available() etc)
/// Caution: there is a time penalty as the radio takes a finite time to wake from sleep mode.
/// \return true if sleep mode was successfully entered.
bool    sleep();

// Bent G Christensen (bentor@gmail.com), 08/15/2016
/// Use the radio's Channel Activity Detect (CAD) function to detect channel activity.
/// Sets the RF95 radio into CAD mode and waits until CAD detection is complete.
/// To be used in a listen-before-talk mechanism (Collision Avoidance)
/// with a reasonable time backoff algorithm.
/// This is called automatically by waitCAD().
/// \return true if channel is in use.  
bool    isChannelActive();

/// This is a low level function to handle the interrupts for one instance of RH_RF95.
/// Called automatically by isr*()
/// Should not need to be called by user code.
void           handleInterrupt();

/// Examine the revceive buffer to determine whether the message is for this node
void validateRxBuf();

/// Clear our local receive buffer
void clearRxBuf();

bool waitPacketSent();

/// Number of octets in the buffer
volatile uint8_t    _bufLen;

/// The receiver/transmitter buffer
volatile uint8_t    _buf[MAX_PAYLOAD_LEN];

/// True when there is a valid message in the buffer
volatile bool       _rxBufValid;

#endif

