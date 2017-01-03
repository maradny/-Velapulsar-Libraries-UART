/*
 * velapulsar.h
 *
 *  Created on: Jan 3, 2017
 *      Author: Galal Hassan
 */

#ifndef LIBRARIES_VELAPULSAR_VELAPULSAR_H_
#define LIBRARIES_VELAPULSAR_VELAPULSAR_H_

/* DriverLib Includes */
#include "driverlib.h"

/* Application includes */
#include "rf95.h"
#include "delay.h"

/* Defines */
#define CLK_FREQ 10000000

/* Function defines */
void initPeripherals (void);
void initPorts (void);
void initClocks (void);
void sleepFor(int minutes);
void initCalendar (void);
void RTC_AlarmHandler(void);


#endif /* LIBRARIES_VELAPULSAR_VELAPULSAR_H_ */
