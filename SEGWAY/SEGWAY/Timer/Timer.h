/*
	Timer class for AVR32UC3B
	
	DEPENDENCIES:
	An interrupt handling system has to be set up. This class only manages the timer registers.
	
	EXAMPLE INTERRUPT HANDLING SYSTEM:
	See main.cpp.example, exception.s.example

 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer {
private:

static const char* TC_CONTROLLER;

//channel registers
static const unsigned int TC_CCR; //control register
static const unsigned int TC_CMR; //mode register
static const unsigned int TC_CV; //counter value
static const unsigned int TC_RA; //register A
static const unsigned int TC_RB; //register B
static const unsigned int TC_RC; //register C
static const unsigned int TC_SR; //status register
static const unsigned int TC_IER; //interrupt enable register
static const unsigned int TC_IDR; //interrupt disable register
static const unsigned int TC_IMR; //interrupt mask register

//block registers
static const unsigned int TC_BCR; // block control register
static const unsigned int TC_BMR; // block mode register

static unsigned int TimerChannel; // stores TimerChannel value 

public:
	Timer();
	~Timer();
	static void resetInterruptFlag();
	bool initTimer( unsigned long frequency );
	void setIsTimerEnabled( bool enabled );
	void setIsTimerInterruptEnabled( bool enabled );
	
	bool prepareTimer(unsigned long frequency);
	void cleanupTimer(void);
	bool getIsTimerEnabled(void);
	bool getIsInterruptEnabled(void);
	void cleanup(void);
};
#endif /* TIMER_H_ */