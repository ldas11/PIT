#include "ADC.h"
#include <stdio.h>

ADC::ADC() {
	//ADC
		this->ADC_ADDRESS = (unsigned char*)0xFFFF3C00;
		this->CR   = 0x00; //Control Register
		this->MR   = 0x04; //Moder Register
		this->CHER = 0x10; //Channel Enable Register
		this->CHDR = 0x14; //Channel Disable Register
		this->CHSR = 0x18; //Channel Status Register
		this->CDR0 = 0x30; //Channel value Register
		this->SR   = 0x1C; //Status Register
		this->LCDR = 0x20; //Last Converted Data Register
		//offsets MR-Register
		this->SHTIM_OFFSET		= 24; //von 24 - 27
		this->STARTUP_OFFSET	= 16; //von 16 - 22
		this->LOWRES_OFFSET		= 4;
		this->PRESCALE_OFFSET	= 8; // von 8 - 15		
		this->DRDY_OFFSET		= 16;
	//GPIO
		this->GPIO_ADDRESS = (unsigned char *)0xFFFF1000;
		this->PORT[0]	= 0x0000; //GPIO port = floor((GPIO number) / 32) 
		this->PORT[1]	= 0x0100; // GPIO pin = GPIO number mod 32
		this->PORT[2]	= 0x0200; //Ja ich weiß unser Projekt bräuchte nur Port 0... ich mach das der Vollständigeit halber
		this->PORT[3]	= 0x0300; 
		this->PORT[4]	= 0x0400; 
		this->GPER		= 0x00;
		this->GPERS		= 0x04; 
		this->GPERC		= 0x08; 
		this->PMR0		= 0x10; //{PMR1, PMR0} | {Selected Peripheral Function}
		this->PMR1		= 0x20; //{(0,0),(0,1),(1,0),(1,1)} | {(A),(B),(C),(D)}
		this->PMR0S     = 0x14;
		this->PMR1S     = 0x24;
		this->PMR0C     = 0x18;
		this->PMR1C     = 0x28;
		this->init();

}
ADC::~ADC() {
	//Control Register
	*(ADC_ADDRESS+CR) |= 0x1;//0b01; //resets ADC
}

bool ADC::init() {
	
	//Mode Register
	//*(ADC_ADDRESS+MR) &= ~1<< LOWRES_OFFSET;//auf 10 Bit setzen ist aber default also wahrscheinlich unnötig
	*((volatile unsigned int*) (this->ADC_ADDRESS+this->MR)) |= 0xF << this->SHTIM_OFFSET;//0b0111 << this->SHTIM_OFFSET;//maximaler Wert für Sample & Hold Time = 20ms ich will alle 20 ms abgreifen-> 0,002*5000-3=7
	*((volatile unsigned int*) (this->ADC_ADDRESS+this->MR)) |= 0xA<< this->STARTUP_OFFSET;//maximaler Wert für Start Up Time=(STARTUP+1)*8/5000
	*((volatile unsigned int*) (this->ADC_ADDRESS+this->MR)) |= 0x1	<< this->PRESCALE_OFFSET; //(CLK_ADC/(2*ADCClock)-1)<<PRESCALE_OFFSET = 0,6 -->1 ; //ADCClock = CLK_ADC / ( (PRESCAL+1) * 2 )  !DIE Clock des ADC darf aber maximal bei 10bit 5MHz sein!
	return 1;
}
														
/*
bool ADC::enableInPinSelector(int P, int PIN, int pmr){	

	printf("ADC::PMR::Registeradresse: 0x%x\n", pmr);
	*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->GPER)) &= ~0x1 << PIN ;
	if(pmr == 0){//0b00
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->PMR0)) &= ~0x01 << PIN ;	
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->PMR1)) &= ~0x01 << PIN ;
	}else if(pmr == 1){//0b01
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->PMR0)) |= 0x01  << PIN ;
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->PMR1)) &= ~0x01 << PIN ;
	}else if(pmr == 2){//0b10
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->PMR0)) &= ~0x01 << PIN ;
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->PMR1)) |= 0x01  << PIN ;
	}else if(pmr == 3){//0b11
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->PMR0)) |= 0x01 << PIN ;
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[P]+this->PMR1)) |= 0x01 << PIN ;
	}
	return 1;
}
*/
bool ADC::enableInPinSelector( unsigned long channelID, bool enabled){	
	int PIN[8] = {3,4,5,6,7,8,30,31};
	*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[0]+this->GPERC)) |= 0x1 << PIN[channelID];	
	if(channelID<2 || channelID >5){//funktion A
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[0]+this->PMR0C)) |= 0x1 << PIN[channelID] ;
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[0]+this->PMR1C)) |= 0x1 << PIN[channelID] ;		
	}else{//Funktion B
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[0]+this->PMR0S)) |= 0x1 << PIN[channelID] ;
		*((volatile unsigned int*) (this->GPIO_ADDRESS+this->PORT[0]+this->PMR1C)) |= 0x1 << PIN[channelID] ;		
	}	
}
bool ADC::enableChannel(unsigned long channelID){
	if(channelID>7){
		return 0;
	}else{
	*((volatile unsigned int*) (this->ADC_ADDRESS+this->CHER)) |= (1<<channelID);
		return 1;
	}
}

void ADC::startConversation(){
	*((volatile unsigned int*) (this->ADC_ADDRESS+this->CR)) = 0x2;//0b10; //starten analog-digital conversation //der hat das im ADC von ATMEL auch hard rein geschrieben ohne ODER
}

bool ADC::disableChannel(unsigned long channelID){
	if(channelID>7){
		return 0;
	}else{
		*((volatile unsigned int*) (this->ADC_ADDRESS+this->CHDR)) |= (1<<channelID);
		return 1;
	}
}
																
unsigned long ADC::getChannelValue( unsigned long channelID) {
	if(!this->enableChannel(channelID)){
		//return 0b10000000000; //grüßerer Wert als 10 BIT dann weiß ich es gab einen Fehler
	}
	unsigned long returnValue;
	bool isfinnished = false;
	startConversation();
	while(!isfinnished){
		isfinnished = *((volatile unsigned int*) (this->ADC_ADDRESS+this->SR)) & (1<<16);//DRDY offset = 16 ?ist bit 16 eine 1?
		//isfinnished = *((volatile unsigned int*) (this->ADC_ADDRESS+this->SR))&(1<<channelID);	//?ist EOCx eine 1?
	}
	returnValue = *((volatile unsigned int*) (this->ADC_ADDRESS+this->LCDR));//return den WERT der im LCDR stehet
	//returnValue = *((volatile unsigned int*) (this->ADC_ADDRESS+((this->CDR0)+channelID)));//returnt den Wert der konkret im Channelvalueregister steht	
	this->disableChannel(channelID);//hier muss ich nicht noch mal prüfen, ob ein falscher Channel gegeben wurde, weil wenn, dann hat das Progrmm schon vorher den Fehler ausgegeben
	
	return returnValue;
}

void ADC::cleanUpChannel( unsigned char channelID ) {
	this->disableChannel(channelID);
}
