#include "ADCSensor.h"
#include <stdio.h>

ADCSensor::ADCSensor() {
}

ADCSensor::~ADCSensor() {
	this->cleanUp();
	//~myADC;//oder werden die beiden wo anders zerstört? immerhin werden die mir von ausen in der INIT funktion überheben
	//~myConfig;
}

bool ADCSensor::init( Configuration::s_ADCSensorConfig* thisADCSensorConfig_, ADC* ADCController_ ) {//muss hier was Abgefragt werden? Weil waum sonst solln bool zurückgegeben werden?
	this->myConfig = thisADCSensorConfig_;
	this->myADC = ADCController_;
	//this->myADC->enableInPinSelector( Configuration::ADC_gpioMultiplexData[this->myConfig->ADCChannelID].port , Configuration::ADC_gpioMultiplexData[this->myConfig->ADCChannelID].pin ,  Configuration::ADC_gpioMultiplexData[this->myConfig->ADCChannelID].multiplexRegisterValue);//int P, int PIN, int pmr
	this->myADC->enableInPinSelector(this->myConfig->ADCChannelID,true);
	//this->myADC->init();
	return 1;
}

signed long ADCSensor::getIntegerValue( bool average, unsigned long numberOfValuesForAverage) {
	signed long returnValue;
	if(average){
		signed long summe = 0;
		for(unsigned int i = 0;i<numberOfValuesForAverage;i++){
			summe += this->myADC->getChannelValue(this->myConfig->ADCChannelID);
		}
		returnValue = summe/numberOfValuesForAverage;
		}else{
		returnValue = this->myADC->getChannelValue(this->myConfig->ADCChannelID);
	}
	if(this->myConfig->useZeroOffset){
		returnValue -= this->myConfig->zeroOffset;
	}
	return returnValue;
}

float ADCSensor::getFloatValue( bool average, unsigned long numberOfValuesForAverage ){ // soll die Funktion wie  long ADCSensor::getIntegerValue benutzt werden????
	float returnValue;
	if(average){
		float summe = 0;
		for(unsigned int i = 0;i<numberOfValuesForAverage;i++){
			summe += this->myADC->getChannelValue(this->myConfig->ADCChannelID);
		}
		returnValue = summe/numberOfValuesForAverage;
		}else{
		returnValue = this->myADC->getChannelValue(this->myConfig->ADCChannelID);
	}
	if(this->myConfig->useZeroOffset){
		returnValue -= this->myConfig->zeroOffset;
	}
	return returnValue;
}

void ADCSensor::setSlopeFactor( bool active, float factor ){
	this->myConfig->slopeFactor		= factor;
	this->myConfig->useSlopeFactor	= active;
}

bool ADCSensor::getSlopeFactorIsActive( void ){
	return this->myConfig->useSlopeFactor;
}

float ADCSensor::getSlopeFactor( void ){
	return this->myConfig->slopeFactor;
}

void ADCSensor::setZeroOffset( bool active, signed long offset ) {
	this->myConfig->useZeroOffset	= active;
	this->myConfig->zeroOffset		= offset;
}

bool ADCSensor::getZeroOffsetIsActive() {
	return this->myConfig->useZeroOffset;
}

signed long ADCSensor::getZeroOffset() {
	return this->myConfig->zeroOffset;
}

void ADCSensor::cleanUp( void ){
	delete this->myADC;
}