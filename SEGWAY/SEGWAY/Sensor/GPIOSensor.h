/*
 * GPIOSensor.h
 *
 * Created: 18.04.2013 15:21:45
 *  Author: Florian
 */ 


#ifndef GPIOSENSOR_H_
#define GPIOSENSOR_H_

#include "../Configuration/Configuration.h"

class GPIOSensor {
private:
	
	static const char* GPIO_CONTROLLER; //GPIO controller 
	static const unsigned int GPIO_GPER; //GPIO enable register	
	static const unsigned int GPIO_PMR0; //Peripheral Mux 0 register	
	static const unsigned int GPIO_PMR1; //Peripheral Mux 1 register
	static const unsigned int GPIO_ODER; //Output Drive Enable register
	static const unsigned int GPIO_OVR; // Output Value register
	static const unsigned int GPIO_PUER; //Pull Up Enable register
	static const unsigned int GPIO_PVR; //Pin Value Register

	unsigned int GPIO_PORT; //stores the selected GPIO port
	unsigned int GPIO_PIN; //stores the selected PIN
	int ones; //stores the amount of consecutive high signals

public:
	GPIOSensor();
	~GPIOSensor();
	void init( Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_ );
	bool getValue();
	void cleanUp();
};


#endif /* GPIOSENSOR_H_ */