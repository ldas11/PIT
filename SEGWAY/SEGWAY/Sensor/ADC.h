/*
	use this class for ADC implementation
*/

#ifndef ADC_H_
#define ADC_H_

#define ADC_NUM_CHANNELS 8

class ADC {
private:
	//ADC
		volatile unsigned char *ADC_ADDRESS;
		volatile unsigned int CR; //Control Register //start conversation/reset ADC
		volatile unsigned int MR; //Moder Register
		volatile unsigned int CHER; //Channel Enable Register
		volatile unsigned int CHDR; //Channel Disable Register
		volatile unsigned int CHSR; //Channel Status Register
		volatile unsigned int CDR0; //
		volatile unsigned int SR; //Status Register
		volatile unsigned int LCDR; //Last Converted Data Register
		
		//offsets MR_Register
		volatile unsigned int SHTIM_OFFSET;
		volatile unsigned int STARTUP_OFFSET;
		volatile unsigned int LOWRES_OFFSET;
		volatile unsigned int PRESCALE_OFFSET;
		volatile unsigned int DRDY_OFFSET; //data ready
	//GPIO
		volatile unsigned char *GPIO_ADDRESS;
		volatile unsigned int PORT[5];
		volatile unsigned int GPER; // GPIO Enable Register
		volatile unsigned int GPERS; // GPIO Set Register
		volatile unsigned int GPERC; // GPIO Clear Register
		volatile unsigned int PMR0; // Peripheral Mux Register 0
		volatile unsigned int PMR1; // Peripheral Mux Register 1
		volatile unsigned int PMR0S; // set Peripheral Mux Register 0
		volatile unsigned int PMR1S; // set Peripheral Mux Register 1
		volatile unsigned int PMR0C; // clear Peripheral Mux Register 0
		volatile unsigned int PMR1C; // clear Peripheral Mux Register 0

public:
	ADC();
	~ADC();
	bool init();
	bool enableChannel(unsigned long channelID);
	bool disableChannel(unsigned long channelID);
	//bool enableInPinSelector(int P, int PIN, int pmr);
	bool enableInPinSelector( unsigned long channelID, bool enabled);
	void startConversation();
	unsigned long getChannelValue( unsigned long channelID);
	void cleanUpChannel( unsigned char channelID );
};

#endif /* ADC_H_ */