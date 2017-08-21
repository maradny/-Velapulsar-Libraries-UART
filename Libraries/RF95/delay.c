#include "delay.h"

volatile uint16_t   _msTime = 0;
volatile uint8_t    _sTime = 0;
volatile uint8_t    _mTime = 0;
volatile uint8_t    _hTime = 0;
volatile uint32_t   _totalms = 0;
volatile uint16_t	_totalDays = 0;

volatile bool alarmSet = false;
volatile int alarmIn = 0;

volatile int tickFrequency;

void (*AlarmWentOff)(void);

void Init_SysTick(uint32_t freq){
    MAP_SysTick_enableModule();
    tickFrequency = freq/1000; // ticks for 1 millisecond
    MAP_SysTick_setPeriod(tickFrequency);
    MAP_Interrupt_setPriority(FAULT_SYSTICK, 0x20);
    MAP_SysTick_enableInterrupt();
    MAP_Interrupt_enableMaster();   
    
    _msTime = _sTime = _mTime = _hTime = _totalms = 0;

    //init alarm:
	/* Configuring Timer32 to 128000 (1s) of MCLK in periodic mode */
	MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
			TIMER32_PERIODIC_MODE);
	MAP_Interrupt_enableInterrupt(INT_T32_INT1);
	MAP_Interrupt_setPriority(INT_T32_INT1, 0x40);
}

uint32_t getTotal_ms(void){
    return _totalms;
}

//******************************************************************************
//
//period is in milliseconds
//
//******************************************************************************
void delay_ms(int period){
    volatile uint32_t currentTime = _totalms;
    while (_totalms - currentTime < period);
}

//bool Set_Alarm (int period, void *callBack()){
//	if (alarmSet){
//		return false;
//	}
//
//	AlarmWentOff = callBack;
//	alarmSet = true;
//	alarmIn = period;
//	return true;
//}

bool Set_Alarm (int period, void *callBack()){
	if (alarmSet){
		return false;
	}
	MAP_Timer32_setCount(TIMER32_BASE,tickFrequency * period);
	MAP_Timer32_enableInterrupt(TIMER32_BASE);
	MAP_Timer32_startTimer(TIMER32_BASE, true);
	AlarmWentOff = callBack;
	alarmSet = true;
	return true;
}


/* Timer32 ISR */
void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
    //printf("ALAAAARM!!!!\n");
	alarmSet = false;
	GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN2);
	AlarmWentOff();
}

void SysTick_Handler(void){
    if (_totalms < 86400000){
        _totalms++;
    }
    else{
        _totalms = 0;
        _totalDays++;
    }

//    if (alarmSet){
//    	if (alarmIn > 0){
//    		alarmIn--;
//    	}
//    	if (alarmIn == 0){
//    		printf("ALAAAARM!!!!\n");
//    		alarmSet = false;
//    		GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN2);
//    		AlarmWentOff();
//    	}
//
//    }

    // if (_msTime <1000){
    //     _msTime++;
    // }
    // else if(_sTime <60){
    //     _msTime = 0;
    //     _sTime++;
    // }
    // else if(_mTime <60){
    //     _sTime = 0;
    //     _mTime++;
    // }
    // else if(_hTime <24){
    //     _mTime = 0;
    //     _hTime++;
    // }
    // else{
    //     _msTime = _sTime = _mTime = _hTime = _totalms = 0;
    // }
}
