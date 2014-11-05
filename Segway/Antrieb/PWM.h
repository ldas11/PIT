/* verwenden Sie diese Klasse f�r alle Funktionen die direkt auf die HW-PWM zugreifen. */

#ifndef PWM_H_
#define PWM_H_

#include "../Configuration/Configuration.h"

class PWM {
private:


public:
	PWM();
	~PWM();
	bool init( Configuration::s_PWMConfig* thisPWMConfig_ );
	bool enableInPinSelector( bool enabled );
	
	bool setChannelPWMRatio( unsigned char ratioOn, bool capRatioOn = false );
	bool setChannelEnabled( bool enabled );
	
	void cleanUp();	
};

#endif /* PWM_H_ */