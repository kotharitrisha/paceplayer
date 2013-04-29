#include <inttypes.h>

#define _BV(bit) (1 << (bit)) 
#define _delay_ms(t)	usleep(1000*t)
/******************************
 ** Beagle Bone GPIO Library **
 **                          **
 **      Francois Sugny      **
 **         01/07/12         **
 **                          **
 **          v1.0            ** 
 ******************************/
 
//=======================================================
//=======================================================
 

 
//=======================================================
//=======================================================

#include "Beagle_GPIO.hh"

void setup(void);
void loop(void);



//=======================================================
//=======================================================

class Beagle_GPIO_ST7565
{
public:
	// Regular pins
	Beagle_GPIO_ST7565(
			Beagle_GPIO * gpio,
			unsigned short _pin_CS,
			unsigned short _pin_SCLK,
			unsigned short _pin_SDATA,
			unsigned short _pin_RESET,
			unsigned short _pin_A0);


	// Initialize Screen
	void initScreen();

	enum
	{
		// Epson S1D15G10 Command Set 
		CMD_DISPLAY_OFF =   0xAE,
	CMD_DISPLAY_ON  =  0xAF,

	CMD_SET_DISP_START_LINE = 0x40,
	CMD_SET_PAGE = 0xB0,

	CMD_SET_COLUMN_UPPER = 0x10,
	CMD_SET_COLUMN_LOWER = 0x00,

	CMD_SET_ADC_NORMAL = 0xA0,
	CMD_SET_ADC_REVERSE = 0xA1,

	CMD_SET_DISP_NORMAL = 0xA6,
	CMD_SET_DISP_REVERSE = 0xA7,

	CMD_SET_ALLPTS_NORMAL = 0xA4,
	CMD_SET_ALLPTS_ON = 0xA5,
	CMD_SET_BIAS_9= 0xA2 ,
	CMD_SET_BIAS_7 =0xA3,

	CMD_RMW = 0xE0,
	CMD_RMW_CLEAR= 0xEE,
	CMD_INTERNAL_RESET = 0xE2,
	CMD_SET_COM_NORMAL = 0xC0,
	CMD_SET_COM_REVERSE = 0xC8,
	CMD_SET_POWER_CONTROL = 0x28,
	CMD_SET_RESISTOR_RATIO = 0x20,
	CMD_SET_VOLUME_FIRST=  0x81,
	CMD_SET_VOLUME_SECOND = 0,
	CMD_SET_STATIC_OFF = 0xAC,
  CMD_SET_STATIC_ON = 0xAD,
	CMD_SET_STATIC_REG = 0x0,
	CMD_SET_BOOSTER_FIRST=  0xF8,
	CMD_SET_BOOSTER_234 = 0,
  CMD_SET_BOOSTER_5 = 1,
  CMD_SET_BOOSTER_6 = 3,
	CMD_NOP  =0xE3,
	CMD_TEST = 0xF0
	} Commands;

public:


	Beagle_GPIO * 	m_gpio;
	unsigned short 	m_pin_BL;
	unsigned short 	m_pin_CS;
	unsigned short 	m_pin_SCLK;
	unsigned short 	m_pin_SDATA;
	unsigned short 	m_pin_RESET;
	unsigned short  m_pin_A0;

	
	// Shift a byte to the screen
	void shiftBits( unsigned char _c );

	// Send a command to the screen
	void sendCommand( unsigned char _cmd );

	// Send data to the screen
	void sendData( unsigned char _data );

	// Array storing the font
	static const unsigned char font_5x8[];

	void write_buffer(uint8_t *buffer);
	void clear_screen(void);

};

	
//=======================================================
//=======================================================



//=======================================================
//=======================================================


