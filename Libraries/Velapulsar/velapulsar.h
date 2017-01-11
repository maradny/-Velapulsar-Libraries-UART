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

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"
#include "stdlib.h"

/* Defines */
#define CLK_FREQ 48000000

/* Function defines */
void initPeripherals (void);
void initPorts (void);
void initClocks (void);
void sleepFor(int minutes);
void initCalendar (void);
void RTC_AlarmHandler(void);


#endif /* LIBRARIES_VELAPULSAR_VELAPULSAR_H_ */
