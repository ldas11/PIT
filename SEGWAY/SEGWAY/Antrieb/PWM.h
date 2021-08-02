/* verwenden Sie diese Klasse für alle Funktionen die direkt auf die HW-PWM zugreifen. */

#ifndef PWM_H_
#define PWM_H_

#include "../Configuration/Configuration.h"

class PWM {
	private:
	
	Configuration::s_PWMConfig* ConfigPWM;

	public:
	PWM();
	~PWM();
	

	
	bool init( Configuration::s_PWMConfig* thisPWMConfig_ );
	bool enableInPinSelector( bool enabled );
	
	bool setChannelPWMRatio( unsigned char ratioOn, bool capRatioOn = false );
	bool setChannelEnabled( bool enabled );
	unsigned char getPWMratio();
	
	void cleanUp();
	
	
	static const char* PWM_MR;		                //PWM controller Adress
	static const unsigned int PWM_ER;				//Enable register Offset (only one register for all channels)
	static const unsigned int PWM_CH;				//Channel Offsset (register to set what the Update Register modifies)
	static const unsigned int PWM_DUTY;			//Duty Cicle register Offset
	static const unsigned int PERIOD;				//Period register Offset
	static const unsigned int COUNTER;				//Counter register Offset
	static const unsigned int UPDATE_R;			//Update register Offset
	
};

#endif /* PWM_H_ */