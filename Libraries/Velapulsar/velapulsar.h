/*
 * Description: Velapulsar layer header file
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
#ifndef LIBRARIES_VELAPULSAR_VELAPULSAR_H_
#define LIBRARIES_VELAPULSAR_VELAPULSAR_H_

/* DriverLib Includes */
#include "driverlib.h"

/* Application includes */
#include "comms_interface.h"
#include "sensor_interface.h"
#include "delay.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"
#include "stdlib.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
#define CLK_FREQ 				  48000000

#define FAIL_JOIN                 10   //Number of times to fail joining a network
#define FAIL_LINK                 3    //Number of times to fail linking a network
#define FAIL_JOIN_DELAY           1000 //Number of milliseconds to wait before retrying to join
#define FAIL_LINK_DELAY           1000 //Number of miliseconds to wait before retrying to link
#define OPERATIONAL_SLEEP_TIMER   2000 //Number of miliseconds to sleep in operational mode
#define DETACHED_SLEEP_TIMER      1000 //Number of miliseconds to sleep in detached mode
#define MAX_FAIL_SEND             5    //Number of failed attempts to send a pkg

//state definitions
typedef enum{
	  UP_FROM_RESET = 0x1,		// 0x01
	  UP_FROM_SLEEP,			// 0x02
	  DEEP_SLEEP,				// 0x03
	  CONNECTING,				// 0x04
	  DISCONNECTING,			// 0x05
	  SLEEP_JOIN_FAIL,			// 0x06
	  SLEEP_LINK_FAIL,			// 0x07

	  /* OPERATIONAL */
	  OPERATIONAL,				// 0x08
	  STATIONARY,				// 0x09
	  ENGAGED,					// 0x0A
	  VERTICAL_UP,				// 0x0B
	  VERTICAL_DOWN,			// 0x0C
	  HORIZONTAL,				// 0x0D
	  DIAGONAL,					// 0x0E
	  OPERATIONAL_SLEEPING,		// 0x0F

	  /* FAULTY */
	  DIAGNOSIS,				// 0x10
	  DETACHED,					// 0x11
	  ON_GROUND,				// 0x12
	  ON_TRUCK,					// 0x13
	  BURIED,					// 0x14
	  CANT_LOCATE				// 0x15
}states;

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/
void initPeripherals (void);
void initPorts (void);
void initClocks (void);
void sleepFor(int minutes);
void initCalendar (void);
void RTC_AlarmHandler(void);

void debug_print_pkt (uint8_t pkt[], int size);

#endif /* LIBRARIES_VELAPULSAR_VELAPULSAR_H_ */
