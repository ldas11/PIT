#include "../Configuration/Configuration.h"
#include "ADC.h"

/*
	Requires the ADC to be initialized.
*/


#ifndef ADCSENSOR_H_
#define ADCSENSOR_H_

class ADCSensor {
private:
	ADC* myADC;
	Configuration::s_ADCSensorConfig* myConfig;
	

public:
	ADCSensor();
	~ADCSensor();
	bool activatePort(char portNr); //aktiviert Port zum Ablesen eines Wertes
	void deactiveatePort(void);
	bool isPortactive(char portNr); //überprüft, ob Port aktiv ist
	bool readActivePort(void); //liest den aktiven Port aus
	
	unsigned long readLCDR(void); //ließt den letzten ermittelten Wert des ADC aus dem Speicher des LCDR
	//duno if I need dis 2 here:
	float f_average();
	signed long l_average();
	
	bool init( Configuration::s_ADCSensorConfig* thisADCSensorConfig_, ADC* ADCController_ );
	long getIntegerValue( bool average = 0, unsigned long numberOfValuesForAverage = 0);
	float getFloatValue( bool average = 0, unsigned long numberOfValuesForAverage = 0 );
	void setSlopeFactor( bool active, float factor );
	bool getSlopeFactorIsActive( void );
	float getSlopeFactor( void );
	void setZeroOffset( bool active, signed long offset );
	bool getZeroOffsetIsActive();
	signed long getZeroOffset();
	void cleanUp( void );
};



#endif /* ADCSENSOR_H_ */