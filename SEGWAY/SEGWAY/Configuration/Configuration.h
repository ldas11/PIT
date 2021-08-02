/*! \class	Configuration

    \brief	This class contains static variables only, which hold the configuration parameters for all other classes used by the segway project.

		In Configuration.h the variables and structs are declared.
		In Configuration.cpp the variables are defined and initialized with zero.
		In init() the variables are set to the configuration values.
		
		This behavior allows calculations to be made within init().
*/


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define ADC_NUM_CONFIGURED_CHANNELS 8

class Configuration {
private:
	static void init_Timer();
	static void init_PWM();
	static void init_Motor();
	static void init_Sensors();
	static void init_Sensor_FootSwitch();
	static void init_Sensor_OrientationAccelerometer();
	static void init_Sensor_OrientationGyrometer();
	static void init_Sensor_OrientationGyrometerReference();
	static void init_Sensor_SteeringPotentiometer();
	static void init_Sensor_BatteryVoltage();
	static void init_UART();
	static void init_StatusLEDs();

public:
	struct s_PWMConfig {
		unsigned char channelID;
		unsigned char maxPWMRatio;				//Value between 0 and 255. 255 equals always-on
		unsigned long frequency;
		unsigned char GPIO_port;
		unsigned char GPIO_pin;
		unsigned char GPIO_multiplexRegisterValue;
	};
	struct s_MotorConfig {
		unsigned char directionPinPort;
		unsigned long directionPinPin;
		bool directionPinForwardValue;			// 1: high on forward, 0: low on forward
		s_PWMConfig* PWMConfig;
	};
	struct s_GPIOSensorConfig {
		unsigned char port;
		unsigned long pin;
		bool pullupEnabled;
	};
	struct s_gpioMultiplexData {
		bool configured;
		unsigned char port;
		unsigned long pin;
		unsigned char multiplexRegisterValue;
	};
	struct s_ADCSensorConfig {
		unsigned long
			ADCChannelID;
		signed long
			zeroOffset;
		float
			slopeFactor;
		bool
			useZeroOffset,
			useSlopeFactor;
	};
	struct s_UARTConfig {
		volatile char* usart_address;
		unsigned long baudRate;
		unsigned char
			charlength,
			paritytype,
			channelmode,
			stopbits;
			
		unsigned char
			RX_GPIO_port,
			RX_GPIO_pin,
			RX_GPIO_multiplexRegisterValue,
			TX_GPIO_port,
			TX_GPIO_pin,
			TX_GPIO_multiplexRegisterValue;
	};
	struct s_StatusLED {
		unsigned char port;
		unsigned long pin;
	};
	
	
	// MISC	
	static unsigned long
		Oscillator_Freq,
		CPUCLK,									//processor clock
		PBACLK,									//peripheral clock
		PWMCLK,									//PWM clock
		ADCCLK;									//ADC clock (Clock provided by the Power Manager to the ADC)
	
	// TIMER
	static unsigned char
		Timer_Channel,
		Timer_Clock_Connection;
		

	// PWM
	static s_PWMConfig
		leftPWMConfig,
		rightPWMConfig;
		
	// MOTOR
	static s_MotorConfig
		leftMotorConfig,
		rightMotorConfig;
		
	static unsigned char Motor_enabledPinPort;
	static unsigned long Motor_enabledPinPin;
	static bool Motor_enabledPinEnabledValue;	// 1: high on enabled, 0: low on enabled

	
	// GPIOSENSOR
	static s_GPIOSensorConfig
		footSwitchConfig;
	
	// ADC(SENSOR)
	static unsigned long ADC_Internal_Clock;	//ADC internal clock (Clock used for the AD conversion, max 5 MHz at 10 bit)
	static s_gpioMultiplexData
		ADC_gpioMultiplexData[ADC_NUM_CONFIGURED_CHANNELS];
	
	static s_ADCSensorConfig
		orientationAccelerometerConfig,
		orientationGyrometerConfig,
		orientationGyrometerReferenceConfig,
		steeringPotentiometerConfig,
		batteryVoltageSensorConfig;
		
	// UART
	static s_UARTConfig
		rs232UARTConfig,
		bluetoothUARTConfig;
		
	// Status LEDs
	static s_StatusLED
		redStatusLEDConfig,
		greenStatusLEDConfig;

	static void init();
};


#endif /* CONFIGURATION_H_ */