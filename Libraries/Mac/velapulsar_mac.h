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

/*****************************************************************************
 *                                INCLUDES
 *****************************************************************************/

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
/*!
 * LoRaMac maximum number of channels
 */
#define VELA_MAX_NB_CHANNELS                        72

/*!
 * Minimal datarate that can be used by the node
 */
#define VELAMAC_TX_MIN_DATARATE                     DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define VELAMAC_TX_MAX_DATARATE                     DR_4

/*!
 * Minimal datarate that can be used by the node
 */
#define VELAMAC_RX_MIN_DATARATE                     DR_8

/*!
 * Maximal datarate that can be used by the node
 */
#define VELAMAC_RX_MAX_DATARATE                     DR_13

/*!
 * Default datarate used by the node
 */
#define VELAMAC_DEFAULT_DATARATE                    DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define VELAMAC_MIN_RX1_DR_OFFSET                   0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define VELAMAC_MAX_RX1_DR_OFFSET                   3

/*!
 * Minimal Tx output power that can be used by the node
 */
#define VELAMAC_MIN_TX_POWER                        TX_POWER_10_DBM

/*!
 * Maximal Tx output power that can be used by the node
 */
#define VELAMAC_MAX_TX_POWER                        TX_POWER_30_DBM

/*!
 * Default Tx output power used by the node
 */
#define VELAMAC_DEFAULT_TX_POWER                    TX_POWER_20_DBM

/*!
 * LoRaMac TxPower definition
 */
#define TX_POWER_30_DBM                             0
#define TX_POWER_28_DBM                             1
#define TX_POWER_26_DBM                             2
#define TX_POWER_24_DBM                             3
#define TX_POWER_22_DBM                             4
#define TX_POWER_20_DBM                             5
#define TX_POWER_18_DBM                             6
#define TX_POWER_16_DBM                             7
#define TX_POWER_14_DBM                             8
#define TX_POWER_12_DBM                             9
#define TX_POWER_10_DBM                             10

/*!
 * LoRaMac datarates definition
 */
#define DR_0                                        0  // SF10 - BW125 |
#define DR_1                                        1  // SF9  - BW125 |
#define DR_2                                        2  // SF8  - BW125 +-> Up link
#define DR_3                                        3  // SF7  - BW125 |
#define DR_4                                        4  // SF8  - BW500 |
#define DR_5                                        5  // RFU
#define DR_6                                        6  // RFU
#define DR_7                                        7  // RFU
#define DR_8                                        8  // SF12 - BW500 |
#define DR_9                                        9  // SF11 - BW500 |
#define DR_10                                       10 // SF10 - BW500 |
#define DR_11                                       11 // SF9  - BW500 |
#define DR_12                                       12 // SF8  - BW500 +-> Down link
#define DR_13                                       13 // SF7  - BW500 |
#define DR_14                                       14 // RFU          |
#define DR_15                                       15 // RFU          |

/*!
 * Second reception window channel definition.
 */
// Channel = { Frequency [Hz], Datarate }
#define RX_WND_2_CHANNEL                                  { 923300000, DR_8 }

/*!
 * VelaMac maximum number of bands
 */
#define VELA_MAX_NB_BANDS                           1

// Band = { DutyCycle, TxMaxPower, LastTxDoneTime, TimeOff }
#define BAND0              { 1, TX_POWER_20_DBM, 0,  0 } //  100.0 %

#define BACKOFF_RND_OFFSET      600000

/*!
 * LoRaMac default channels
 */
// Channel = { Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
/*
 * US band channels are initialized using a loop in LoRaMacInit function
 * \code
 * // 125 kHz channels
 * for( uint8_t i = 0; i < LORA_MAX_NB_CHANNELS - 8; i++ )
 * {
 *     Channels[i].Frequency = 902.3e6 + i * 200e3;
 *     Channels[i].DrRange.Value = ( DR_3 << 4 ) | DR_0;
 *     Channels[i].Band = 0;
 * }
 * // 500 kHz channels
 * for( uint8_t i = LORA_MAX_NB_CHANNELS - 8; i < LORA_MAX_NB_CHANNELS; i++ )
 * {
 *     Channels[i].Frequency = 903.0e6 + ( i - ( LORA_MAX_NB_CHANNELS - 8 ) ) * 1.6e6;
 *     Channels[i].DrRange.Value = ( DR_4 << 4 ) | DR_4;
 *     Channels[i].Band = 0;
 * }
 */

#endif /* LIBRARIES_MAC_VELAPULSAR_MAC_H_ */
