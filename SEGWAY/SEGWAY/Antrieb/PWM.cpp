#include "PWM.h"
#include <stdio.h>

const char* PWM::PWM_MR = (char*) 0xFFFF3000;		//PWM controller Adress
const unsigned int PWM::PWM_ER = 0x004;				//Enable register Offset (only one register for all channels)
const unsigned int PWM::PWM_CH = 0x200;				//Channel Offsset (register to set what the Update Register modifies)
const unsigned int PWM::PWM_DUTY = 0x004;			//Duty Cicle register Offset
const unsigned int PWM::PERIOD = 0x008;				//Period register Offset
const unsigned int PWM::COUNTER = 0x00C;				//Counter register Offset
const unsigned int PWM::UPDATE_R = 0x010;			//Update register offset



PWM::PWM() {

}


PWM::~PWM() {
	
}

bool PWM::init( Configuration::s_PWMConfig* thisPWMConfig_ ) {
	printf("PWM:: ICH WURDE INITIALISIERT");
	*(volatile unsigned int*)(PWM_MR + PWM_ER) |=1 << thisPWMConfig_->channelID	;	//Enable Channel
	*(volatile unsigned int*)(PWM_MR + PERIOD) = Configuration::PWMCLK / thisPWMConfig_->frequency;	 // Clock gesetzt
	*(volatile unsigned int*)(PWM_MR + PWM_CH + thisPWMConfig_->channelID * 0x020) |=1 << 10;		//Tell Update register to change on Duty cycle register
	
	ConfigPWM = thisPWMConfig_;
	
	//return 0;
	return 1;
}



bool PWM::setChannelPWMRatio( unsigned char ratioOn, bool capRatioOn ) { //capratioOn bool ???
	
	if(ratioOn <= ConfigPWM->maxPWMRatio){
		*(volatile unsigned int*)(PWM_MR + PWM_CH + UPDATE_R + ConfigPWM->channelID * 0x020) = ratioOn;
		return true;
	}
	else{

		return false;
	}

	return false;
}

bool PWM::setChannelEnabled( bool enabled ) {
	
	*(volatile unsigned int*)(PWM_MR + PWM_ER) = enabled << ConfigPWM->channelID	;	//Enable or disable Channel

	return true;
}
bool PWM::enableInPinSelector( bool enabled ) {  //Done in Motor::init();

	return 0;
}

void PWM::cleanUp() {

}

unsigned char PWM::getPWMratio(){
	return *(volatile unsigned int*)(PWM_MR + PWM_CH + UPDATE_R + PWM::ConfigPWM->channelID * 0x020);
}
