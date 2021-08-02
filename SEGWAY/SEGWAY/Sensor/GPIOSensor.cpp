#include "GPIOSensor.h"
#include "../bitmacros.h"
#include "../Configuration/Configuration.h"

//GPIO controller address
const char* GPIOSensor::GPIO_CONTROLLER = (char*) 0xFFFF1000;

//register offsets
const unsigned int GPIOSensor::GPIO_GPER            = 0x00;
const unsigned int GPIOSensor::GPIO_ODER            = 0x40;
const unsigned int GPIOSensor::GPIO_PVR             = 0x60;
const unsigned int GPIOSensor::GPIO_PUER            = 0x70;

GPIOSensor::GPIOSensor() {


}

GPIOSensor::~GPIOSensor() {
	//destructor calls cleanup() function
	cleanUp();

}

void GPIOSensor::init( Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_ ) {
	//gets the port and pin from the Configuration.cpp file and stores it as a variable of the GPIOSensor class
	GPIO_PORT = thisGPIOSensorConfig_->port;
	GPIO_PIN = thisGPIOSensorConfig_->pin;

	//initializes the variable ones with 0
	//ones = 0;
	//zeroes = 0; //si se usa el metodo 2 o el 0

	//enables the given pin
	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_PORT*0x0100 + GPIO_GPER) |= 1<<GPIO_PIN;
	
	//enables the pull up resistor if pullupEnabled (see Configuration.cpp) is true
	if(thisGPIOSensorConfig_->pullupEnabled){
		*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_PORT*0x0100 + GPIO_PUER) |= 1<<GPIO_PIN;	
	}

}

bool GPIOSensor::getValue() {	
	return *(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_PORT*0x0100+ GPIO_PVR) & 1<<GPIO_PIN;
	
	/* 
	The value of ones will increase by one, when the pin is high.
	If the high sequence reaches a certain value after exiting the for loop 
	(meaning the pedal was actually pressed), return true
	
	for (int i = 0; i < 10000; ++i)
	{
		if(*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_PORT*0x0100+ GPIO_PVR) & 1<<GPIO_PIN)
		{
			ones++;
		}
	}

	if(ones >=500)
	{
		ones=0;
		return true;
	}
	else 
	{
		ones=0;
		return false;
	}
	 
--------------------------------------------------------------------------------
	metodo alterno 1
	for (int i = 0; i<10; ++i)
	{
		if(*(volatile unsigned int*)(GPIO_CONTROLLER+GPIO_PORT*0x0100+GPIO_PVR)&1<<GPIO_PIN)
		{
			ones -= 1;
		}
		else{
			ones +=1;
		}
	}
	if (ones<=0)
	{
		ones=0;
		return true;
	}
	if (ones > 1)
	{
		ones=0;
		return false;
	}
----------------------------------------------------------------------------
	 metodo alterno 2
	 	if(*(volatile unsigned int*)(GPIO_CONTROLLER+GPIO_PORT*0x0100+GPIO_PVR)&1<<GPIO_PIN)
	 	{
	 		ones++;
	 		if(ones > 100)
	 		{
	 			ones=0;
	 			zeroes=0;
				return true;
	 		}
	 	}
	 	else
	 	{
			zeroes++;
			if(zeroes>100)
			{
				ones=0;
				zeroes=0;
				return false;
			}
	 	}
-----------------------------------------------------------------------------------------


	 */
}

void GPIOSensor::cleanUp() {
 
 	//disables the given pin
 	*(volatile unsigned int*) (GPIO_CONTROLLER + GPIO_PORT*0x0100 + GPIO_GPER) &= (1<<GPIO_PIN);
}


