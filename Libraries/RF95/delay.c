#include "delay.h"

volatile uint16_t   _msTime = 0;
volatile uint8_t    _sTime = 0;
volatile uint8_t    _mTime = 0;
volatile uint8_t    _hTime = 0;
volatile uint32_t   _totalms = 0;

void Init_SysTick(uint32_t freq){
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(freq/1000/8);
    MAP_Interrupt_setPriority(FAULT_SYSTICK, 0x20);
    MAP_SysTick_enableInterrupt();
    MAP_Interrupt_enableMaster();   
    
    _msTime = _sTime = _mTime = _hTime = _totalms = 0;
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

void SysTick_Handler(void){
    if (_totalms < 86400000)
        _totalms++;
    else
        _totalms = 0;

    GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN2);
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
