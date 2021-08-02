#include "Motor.h"
#include <stdio.h>


const char* Motor::GPIO_CONTROLLER = (char*) 0xFFFF1000;
const unsigned int Motor::GPIO_GPER            = 0x00;
const unsigned int Motor::GPIO_ODER            = 0x40;
const unsigned int Motor::GPIO_OVR             = 0x50;
const unsigned int Motor::PORT			       = 0x100;
const unsigned int Motor::MUX0                 = 0x10;
const unsigned int Motor::MUX1                 = 0x20;


Motor::Motor() {

}

Motor::~Motor() {
	cleanUp();
}

void Motor::initEnablePin() {
//	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_GPER + Configuration::Motor_enabledPinPin * PORT) |= 1 << Configuration::Motor_enabledPinPin;
//	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_ODER + Configuration::Motor_enabledPinPin * PORT) |= 1 << Configuration::Motor_enabledPinPin;
//	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_OVR  + Configuration::Motor_enabledPinPin * PORT) &= 0 << Configuration::Motor_enabledPinPin;
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_GPER) |= 1 << Configuration::Motor_enabledPinPin;
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_ODER) |= 1 << Configuration::Motor_enabledPinPin;
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_OVR ) &= 0 << Configuration::Motor_enabledPinPin;
}

bool Motor::init( Configuration::s_MotorConfig* thisMotorConfig_ ) {
	Motor::Pointer2Struct = thisMotorConfig_;
	
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_GPER + PORT * Pointer2Struct->directionPinPort) |= 1 << (thisMotorConfig_->directionPinPin);	//Enable GPIO Pin direction bit
	
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_ODER) |= 1 << (thisMotorConfig_->directionPinPin);
	
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_OVR) = thisMotorConfig_->directionPinForwardValue << thisMotorConfig_->directionPinPin;		//Wert zuweisen
	
	
	//Read value of PWM's GPIO_muliplexregistervalue and pick correct funktion
	if(thisMotorConfig_->PWMConfig->GPIO_multiplexRegisterValue == 1){
		
		*(volatile unsigned int*) (GPIO_CONTROLLER + MUX0) |= 1 << thisMotorConfig_->PWMConfig->GPIO_pin;
	}
	else if(thisMotorConfig_->PWMConfig->GPIO_multiplexRegisterValue == 2){
		
		*(volatile unsigned int*) (GPIO_CONTROLLER + MUX1) |= 1 << thisMotorConfig_->PWMConfig->GPIO_pin;
	}
	else if (thisMotorConfig_->PWMConfig->GPIO_multiplexRegisterValue == 3){
		
		*(volatile unsigned int*) (GPIO_CONTROLLER + MUX0) |= 1 << thisMotorConfig_->PWMConfig->GPIO_pin;
		*(volatile unsigned int*) (GPIO_CONTROLLER + MUX1) |= 1 << thisMotorConfig_->PWMConfig->GPIO_pin;
	}
	else{
		
		*(volatile unsigned int*) (GPIO_CONTROLLER + MUX1) &= 0 << thisMotorConfig_->PWMConfig->GPIO_pin;
		*(volatile unsigned int*) (GPIO_CONTROLLER + MUX1) &= 0 << thisMotorConfig_->PWMConfig->GPIO_pin;
	}

	
	if(Configuration::Motor_enabledPinEnabledValue == 1){
		return true;
	}
	return false;
}

bool Motor::setSpeed( unsigned char ratioOn ) {
	
	return pwmobj.setChannelPWMRatio(ratioOn, false) ;
}


unsigned char Motor::getSpeed() {

	return pwmobj.getPWMratio();
}

void Motor::setDirection( bool forward ) {

	*(volatile unsigned int*)(GPIO_CONTROLLER + GPIO_OVR + PORT * Pointer2Struct->directionPinPort) = forward << Motor::Pointer2Struct->directionPinPin; 		//Set Direction in correct Pin
}

void Motor::setEnabled( bool enabled ) {
	
	if(enabled){
		*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_OVR + PORT) |= 1 << 16;
	}
	else{
		*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_OVR) &= 0 << 16;
	}
}

bool Motor::getIsEnabled() {
	
	if ( *(volatile unsigned int*)(GPIO_CONTROLLER + GPIO_OVR) & 1 << 16 ) {
		return true;
	}

	return false;
}


void Motor::cleanUpEnablePin() {
	
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_GPER + Configuration::Motor_enabledPinPin * PORT) &= 0 << Configuration::Motor_enabledPinPin;
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_ODER + Configuration::Motor_enabledPinPin * PORT) &= 0 << Configuration::Motor_enabledPinPin;
	
}

void Motor::cleanUp(){
	
	delete Pointer2Struct;
	
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_GPER + PORT * Pointer2Struct->directionPinPort) &= 0 << (Motor::Pointer2Struct->directionPinPin);
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_GPER + PORT * Pointer2Struct->directionPinPort) &= 0 << (Motor::Pointer2Struct->PWMConfig->GPIO_pin);
	
}