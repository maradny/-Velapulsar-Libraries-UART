/*
 * Description: Sensor interface API for Velapulsar
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
#ifndef LIBRARIES_VELAPULSAR_SENSOR_INTERFACE_H_
#define LIBRARIES_VELAPULSAR_SENSOR_INTERFACE_H_


#include "driverlib.h"

/*****************************************************************************
 *                                DEFINES
 *****************************************************************************/
#define LIGHT_PORT    GPIO_PORT_P4
#define LIGHT_PIN     GPIO_PIN7
#define MAGNETIC_PORT GPIO_PORT_P4
#define MAGNETIC_PIN  GPIO_PIN6
#define CLAW_PORT	  GPIO_PORT_P4
#define CLAW_PIN	  GPIO_PIN5

typedef enum{
	FULL,
	HIGH90,
	HIGH80,
	AVG70,
	AVG60,
	AVG50,
	AVG40,
	LOW30,
	LOW20,
	LOW10,
	EMPTY
}batteryLevel;

typedef struct{
  uint16_t      light;
  uint16_t      magnetic;
  bool      claw;
  batteryLevel    battery;
}sensorData;

uint16_t lastMagnetic;
uint16_t lastLight;

/*****************************************************************************
 *                             Functions - API
 *****************************************************************************/
void Init_Sensors(void);
uint16_t Get_Magnetic(void);
bool Get_Claw(void);
uint16_t Get_Light(void);

#endif /* LIBRARIES_VELAPULSAR_SENSOR_INTERFACE_H_ */
