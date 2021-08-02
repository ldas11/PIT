#include "Timer.h"
#include "../Configuration/Configuration.h"

// Timer/Counter controller address
const char* Timer::TC_CONTROLLER = (char*) 0xFFFF3800;
unsigned int Timer::TimerChannel;
//register offsets
//channel 0

const unsigned int Timer::TC_CCR = 0x00;
const unsigned int Timer::TC_CMR = 0x04;
const unsigned int Timer::TC_CV  = 0x10;
const unsigned int Timer::TC_RA  = 0x14;
const unsigned int Timer::TC_RB  = 0x18;
const unsigned int Timer::TC_RC  = 0x1C;
const unsigned int Timer::TC_SR  = 0x20;
const unsigned int Timer::TC_IER = 0x24;
const unsigned int Timer::TC_IDR = 0x28;
const unsigned int Timer::TC_IMR = 0x2C;
//block 
const unsigned int Timer::TC_BCR = 0xC0;
const unsigned int Timer::TC_BMR = 0xC4;



Timer::Timer() {

}

Timer::~Timer() {
	
	cleanup();

}


bool Timer::initTimer( unsigned long frequency ) {
	//stores TimerChannel value on a new variable to be used in the other methods of the class
	TimerChannel = Configuration::Timer_Channel; 

	//configures the timer
	//enables the interrupts if the configuration was successful
	if(prepareTimer(frequency)){
		setIsTimerInterruptEnabled(true);
	}
	else
	{
		setIsTimerInterruptEnabled(false);
	}
	
	//return 0;
	return 1;
}

void Timer::setIsTimerEnabled( bool enabled ) {
	//channel control register - enables Counter Clock (CLKEN bit high) if enabled is true, disables it if enabled is false
	if(enabled)
	{
		*(volatile unsigned int*) (TC_CONTROLLER + (TC_CCR + TimerChannel*0x40)) |= 1;
	}
	else 
	{
		*(volatile unsigned int*) (TC_CONTROLLER + (TC_CCR + TimerChannel*0x40)) |= 1<<1;

	}
}

void Timer::setIsTimerInterruptEnabled( bool enable ) {


	if (!enable)
	{
		//enables interrupts on CPCS bit
		*(volatile unsigned int*) (TC_CONTROLLER + (TC_IER + TimerChannel * 0x40)) |= 1<<4;
	} 
	else
	{
		//disables interrupts on CPCS bit
		*(volatile unsigned int*) (TC_CONTROLLER + (TC_IDR + TimerChannel * 0x40)) |= 1<<4;
	}

}
void Timer::resetInterruptFlag() {
 	//reading the status register will reset the interrupt bit
 	*(volatile unsigned int*) (TC_CONTROLLER +(TC_SR + TimerChannel*0x40)) & 1<<4;
}

bool Timer::prepareTimer(unsigned long frequency) {

	/*
	configuration of the channel mode register (CMR)
	1st: TIMER_CLOCK3 selection
	2nd: Waveform selection
	3rd: UP mode with automatic trigger on RC Compare selection
	 */
	*(volatile unsigned int*) (TC_CONTROLLER + (TC_CMR + TimerChannel*0x40)) |= 1<<1;
	*(volatile unsigned int*) (TC_CONTROLLER + (TC_CMR + TimerChannel*0x40)) |= 1<<15;
	*(volatile unsigned int*) (TC_CONTROLLER + (TC_CMR + TimerChannel*0x40)) |= 1<<14;

	//2MHz/frequency gives the number of cycles within 10ms. The interrupts will be enabled in 10ms intervalls.
	*(volatile unsigned int*) (TC_CONTROLLER + (TC_RC + TimerChannel*0x40)) = 2000000/frequency;

	//If the previous configuration was successful return true. 
	//0xC002 is the corresponding hex number for the CMR configurations, if they are all considered as one
	if ((*(volatile unsigned int*) (TC_CONTROLLER + TC_RC + TimerChannel*0x40) == 2000000/frequency) && (*(volatile unsigned int*)(TC_CONTROLLER + TC_CMR + TimerChannel*0x40) == 0xC002))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Timer::cleanupTimer() {
	//resets the counter and starts the clock
	*(volatile unsigned int*) (TC_CONTROLLER + (TC_CCR + TimerChannel*0x40)) |= 1<<2;
}

bool Timer::getIsTimerEnabled(){
	//returns true if the CLKSTA (clock status) bit on the status register is high, false if low
	return *(volatile unsigned int*) (TC_CONTROLLER + (TC_SR + TimerChannel*0x40)) & 1<<16;
}

bool Timer::getIsInterruptEnabled(){
	//returns true if the CPCS bit on the interrupt enable register is high, false if low
	return *(volatile unsigned int*) (TC_CONTROLLER + (TC_IMR + TimerChannel*0x40)) & 1<<4;
}

void Timer::cleanup(){
	//stops counter
	*(volatile unsigned int*) (TC_CONTROLLER +(TC_CCR +TimerChannel*0x40)) |= 1<<1;

	//clears Register C
	*(volatile unsigned int*) (TC_CONTROLLER +(TC_RC + TimerChannel*0x40)) = 0x00000000;
	
	//calls method
	resetInterruptFlag();
}
