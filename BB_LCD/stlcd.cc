/*
$Id:$
ST7565 LCD library!
Copyright (C) 2010 Limor Fried, Adafruit Industries

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

vYou should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 // some of this code was written by <cstone@pobox.com> originally; it is in the public domain.


 Edits for BeagleBone and ST7565 LCD Compatibility  made by Trisha Kothari and Mark Eisen (2013)
*/


#define _BSD_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stlcd.hh"
#include "glcd.hh"
#include <unistd.h>


#define VAL 0x18;

#define CMD_DISPLAY_OFF 0xAE
#define CMD_DISPLAY_ON 0xAF

#define CMD_SET_DISP_START_LINE 0x40
#define CMD_SET_PAGE 0xB0

#define CMD_SET_COLUMN_UPPER 0x10
#define CMD_SET_COLUMN_LOWER 0x00

#define CMD_SET_ADC_NORMAL 0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON 0xA5
#define CMD_SET_BIAS_9 0xA2
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW 0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET 0xE2
#define CMD_SET_COM_NORMAL 0xC0
#define CMD_SET_COM_REVERSE 0xC8
#define CMD_SET_POWER_CONTROL 0x28
#define CMD_SET_RESISTOR_RATIO 0x20
#define CMD_SET_VOLUME_FIRST 0x81
#define CMD_SET_VOLUME_SECOND 0
#define CMD_SET_STATIC_OFF 0xAC
#define CMD_SET_STATIC_ON 0xAD
#define CMD_SET_STATIC_REG 0x0
#define CMD_SET_BOOSTER_FIRST 0xF8
#define CMD_SET_BOOSTER_234 0
#define CMD_SET_BOOSTER_5 1
#define CMD_SET_BOOSTER_6 3
#define CMD_NOP 0xE3
#define CMD_TEST 0xF0


uint8_t is_reversed = 0;

Beagle_GPIO	gpio;


int pagemap[] = { 3, 2, 1, 0, 7, 6, 5, 4 };

uint8_t buffer[128*64/8] = {
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 

0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x3, 0x7, 0xF, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x7F, 0x3F, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x1F, 0x3F, 0x70, 0x70, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0x6, 0x0, 0x0, 0x0, 0x3, 0x3, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 

0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0xF, 0x7, 0x7, 
0x7, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0x3F, 
0x70, 0x60, 0x60, 0x60, 0x60, 0x30, 0x7F, 0x3F, 0x0, 0x0, 0x1F, 0x3F, 0x70, 0x60, 0x60, 0x60, 
0x60, 0x39, 0xFF, 0xFF, 0x0, 0x6, 0x1F, 0x39, 0x60, 0x60, 0x60, 0x60, 0x30, 0x3F, 0x7F, 0x0, 
0x0, 0x60, 0xFF, 0xFF, 0x60, 0x60, 0x0, 0x7F, 0x7F, 0x70, 0x60, 0x60, 0x40, 0x0, 0x7F, 0x7F, 
0x0, 0x0, 0x0, 0x0, 0x7F, 0x7F, 0x0, 0x0, 0x0, 0x7F, 0x7F, 0x0, 0x0, 0x60, 0xFF, 0xFF, 
0x60, 0x60, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 

0x80, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xE7, 0xE7, 0xE3, 
0xF3, 0xF9, 0xFF, 0xFF, 0xFF, 0xF7, 0x7, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x1F, 0xF, 0x7, 0x3, 0x0, 0x0, 0x0, 0xC0, 
0xE0, 0x60, 0x20, 0x20, 0x60, 0xE0, 0xE0, 0xE0, 0x0, 0x0, 0x80, 0xC0, 0xE0, 0x60, 0x20, 0x60, 
0x60, 0xE0, 0xE0, 0xE0, 0x0, 0x0, 0x80, 0xC0, 0x60, 0x60, 0x20, 0x60, 0x60, 0xE0, 0xE0, 0x0, 
0x0, 0x0, 0xE0, 0xE0, 0x0, 0x0, 0x0, 0xE0, 0xE0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0xE0, 
0x60, 0x60, 0x60, 0x60, 0xE0, 0x80, 0x0, 0x0, 0x0, 0xE0, 0xE0, 0x0, 0x0, 0x0, 0xE0, 0xE0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 

0x0, 0x0, 0x0, 0x3, 0x7, 0x1F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xF1, 0xE3, 
0xE3, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFC, 0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF0, 0xE0, 0x80, 0x0, 0x0, 0x0, 0xC, 
0x1C, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7F, 0x7F, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x7, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1C, 0xC, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 

0x0, 0x7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0xF8, 
0xF8, 0xF0, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 
0xFF, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xE0, 0xC0, 0xC0, 0xC0, 0xFF, 0x7F, 0x0, 0x0, 0x1E, 0x7F, 
0xE1, 0xC0, 0xC0, 0xC0, 0xC0, 0x61, 0xFF, 0xFF, 0x0, 0x0, 0xFE, 0xFF, 0x1, 0x0, 0x0, 0x0, 
0xFF, 0xFF, 0x0, 0x0, 0x21, 0xF9, 0xF8, 0xDC, 0xCC, 0xCF, 0x7, 0x0, 0xC0, 0xFF, 0xFF, 0xC0, 
0x80, 0x0, 0xFF, 0xFF, 0xC0, 0xC0, 0x80, 0x0, 0x0, 0xFF, 0xFF, 0x0, 0x0, 0x1F, 0x7F, 0xF9, 
0xC8, 0xC8, 0xC8, 0xC8, 0x79, 0x39, 0x0, 0x0, 0x71, 0xF9, 0xD8, 0xCC, 0xCE, 0x47, 0x3, 0x0, 

0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF0, 0xC0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xC0, 
0xC0, 0x0, 0x0, 0x0, 0xC0, 0xC0, 0x0, 0x0, 0x0, 0x0, 0xC0, 0xC0, 0x0, 0x0, 0x0, 0x80, 
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0xC0, 0xC0, 0x0, 0x0, 0x0, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 
0xC0, 0x80, 0x0, 0x0, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x0, 0x0, 0x0, 0xC0, 0xC0, 0x0, 
0x0, 0x0, 0xC0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0xC0, 0xC0, 0x0, 0x0, 0x0, 0x80, 0xC0, 
0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x0, 0x0, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x0, 0x0, 

0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 

};

Beagle_GPIO_ST7565::Beagle_GPIO_ST7565(
		Beagle_GPIO * gpio,
		unsigned short _pin_CS,
		unsigned short _pin_SCLK,
		unsigned short _pin_SDATA,
		unsigned short _pin_RESET,
		unsigned short _pin_A0)
{
	if (!gpio || !gpio->isActive())
	{
		GPIO_ERROR( "Invalid or Inactive GPIO Module" );
		m_gpio = NULL;
		return;
	}
	m_gpio = gpio;

	m_pin_CS    = _pin_CS;
	m_pin_SCLK  = _pin_SCLK;
	m_pin_SDATA = _pin_SDATA;
	m_pin_RESET = _pin_RESET;
	m_pin_A0 = _pin_A0;

	// Initialize pins on the board
	gpio->configurePin( m_pin_CS,    Beagle_GPIO::kOUTPUT );
	gpio->configurePin( m_pin_SCLK,  Beagle_GPIO::kOUTPUT );
	gpio->configurePin( m_pin_SDATA, Beagle_GPIO::kOUTPUT );
	gpio->configurePin( m_pin_RESET, Beagle_GPIO::kOUTPUT );
	gpio->configurePin( m_pin_A0, Beagle_GPIO::kOUTPUT );
	m_gpio->writePin( m_pin_CS,    1);
	m_gpio->writePin( m_pin_SCLK,  0);
	m_gpio->writePin( m_pin_SDATA, 0);
	m_gpio->writePin( m_pin_RESET, 1);
	m_gpio->writePin( m_pin_A0, 0);
}


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
    

void testdrawcircle(uint8_t *buff) {
  for (uint8_t i=0; i<64; i+=2) {
    drawcircle(buffer, 63, 31, i, 1);
  }
}

void testdrawline(Beagle_GPIO_ST7565 lcd,uint8_t *buff) {
  for (uint8_t i=0; i<128; i+=4) {
    drawline(buffer, 0, 0, i, 63, 1);
  }
  for (uint8_t i=0; i<64; i+=4) {
    drawline(buffer, 0, 0, 127, i, 1);
  }

  lcd.write_buffer(buffer);
  _delay_ms(1000);

  for (uint8_t i=0; i<128; i+=4) {
    drawline(buffer, i, 63, 0, 0, 0);
  }
  for (uint8_t i=0; i<64; i+=4) {
    drawline(buffer, 127, i, 0, 0, 0);
  }
 }

void testdrawrect(uint8_t *buff) {
   GPIO_PRINT( "Test draw rectangle has been called" );
  for (uint8_t i=0; i<64; i+=2) {
    drawrect(buff, i, i, 128-i, 64-i, 1);

  }
}

void testchar(uint8_t *buff) {
    for (uint8_t i=0; i <65; i+=2)
	printChar(buff, i*5, 60, 't', 1);
}

void teststring(uint8_t *buff){
printstring(buff, 30, 50, "Hello World", 1); 
}

void testfillrect(uint8_t *buff) {
  for (uint8_t i=0; i<64; i++) {
    fillrect(buff, i, i, 128-i, 64-i, i%2);

  }
}




void Beagle_GPIO_ST7565::initScreen() {
	GPIO_PRINT( "Initializing ST7565 Screen" );
			// Perform a reset
	    m_gpio->writePin( m_pin_CS, 0 );
		m_gpio->writePin( m_pin_RESET, 1 );
		usleep(10000);
		m_gpio->writePin( m_pin_RESET, 0 );
		usleep(10000);
		m_gpio->writePin( m_pin_RESET, 1 );
		usleep(10000);

		sendCommand(CMD_SET_BIAS_7);
		sendCommand(CMD_SET_ADC_NORMAL);
		sendCommand(CMD_SET_COM_NORMAL);
		sendCommand(CMD_SET_DISP_START_LINE);
		sendCommand(CMD_SET_POWER_CONTROL | 0x4);
		usleep(50000);
		sendCommand(CMD_SET_POWER_CONTROL | 0x6);
		usleep(50000);
		sendCommand(CMD_SET_POWER_CONTROL | 0x7);
		usleep(50000);
		sendCommand(CMD_SET_RESISTOR_RATIO | 0x6);
		usleep(10000);
}

void Beagle_GPIO_ST7565::shiftBits( unsigned char _c )
{
 	for (int i=7; i>=0; i--)
	{
		unsigned char bit = !!(_c & ( 0x01 << i ) );
		m_gpio->writePin( m_pin_SDATA, bit);		
		m_gpio->writePin(m_pin_SCLK, 1);
		m_gpio->writePin( m_pin_SCLK,  0);
	}
}

//=======================================================
//=======================================================

// Send a command to the screen
void Beagle_GPIO_ST7565::sendCommand( unsigned char _cmd )
{
	m_gpio->writePin( m_pin_A0, 0);

	shiftBits( _cmd );
}

//=======================================================
//=======================================================

// Send data to the screen
void Beagle_GPIO_ST7565::sendData( unsigned char _data )
{
	m_gpio->writePin( m_pin_A0, 1);


	shiftBits( _data );
}

void Beagle_GPIO_ST7565::clear_screen(void) {
  uint8_t p, c;
  
  for(p = 0; p < 8; p++) {


    sendCommand(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {

      sendCommand(CMD_SET_COLUMN_LOWER | (c & 0xf));
      sendCommand(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      sendData(0x0);
    }     
  }
}

void Beagle_GPIO_ST7565::write_buffer(uint8_t *buffer) {
  uint8_t c, p;

  for(p = 0; p < 8; p++) {
    /*
      putstring("new page! ");
      uart_putw_dec(p);
      //putstring_nl("");
    */

    sendCommand(CMD_SET_PAGE | pagemap[p]);
    sendCommand(CMD_SET_COLUMN_LOWER | (0x1 & 0xf));
    sendCommand(CMD_SET_COLUMN_UPPER | ((0x1 >> 4) & 0xf));
    sendCommand(CMD_RMW);
    sendData(0xff);
    
    //st7565_data(0x80);
    //continue;
    
    for(c = 0; c < 128; c++) {
      //uart_putw_dec(c);
      //uart_putchar(' ');
      sendData(buffer[(128*p)+c]);
    }
  }
}

int main(int argc, char* argv[]) {

GPIO_PRINT( "==============================" );
GPIO_PRINT( "BeagleBone GPIO Test" );
GPIO_PRINT( "==============================" );

char speed[30];
char step[30];
char song[30];

/*speed = "Speed: " + argv[1];
step = "Step: " + argv[2];
song = "Song: " + argv[3]; 
*/

strcpy(speed,"Speed: ");
strcpy(step, "Step: ");
strcpy(song, "Now Playing ");
strcat(speed,argv[1]);
strcat(step,argv[2]);
strcat(song,argv[3]);

printf( "Argument speed: %s\n", argv[1]);
printf( "Argument step count: %s\n", argv[2]);
printf( "Song argument: %s \n", argv[3]);
printf("Here is speed: %s \n", speed);
printf("Here is step: %s \n", step);
printf("Here is song: %s \n", song);


// Regular pins interface
Beagle_GPIO_ST7565 lcd(&gpio,
Beagle_GPIO::P8_45, // CS - yellow
Beagle_GPIO::P8_44, // SCLK  yellow- 
Beagle_GPIO::P8_43, // SDATA -green
Beagle_GPIO::P8_42, //RESET - white
Beagle_GPIO::P8_41); // A0 orange

gpio.writePin( Beagle_GPIO::P8_46, 1 );
// Init LCD screen
  lcd.initScreen();

  lcd.write_buffer(buffer);
      lcd.sendCommand(CMD_DISPLAY_ON);

    lcd.sendCommand(CMD_SET_ALLPTS_NORMAL);
    lcd.sendCommand(CMD_SET_VOLUME_FIRST);
    lcd.sendCommand(0x28);

    lcd.clear_screen();

    lcd.write_buffer(buffer);
    _delay_ms(250);

   clear_buffer(buffer);

    printstring(buffer, 10, 24, speed,1);
      printstring(buffer, 10, 8, step, 1);
     printstring(buffer,10 ,40, song, 1);
//	testdrawcircle(buffer);
   //  testfillrect(buffer);
//      testdrawline(lcd,buffer);
  //  testdrawcircle(buffer);
   // fillcircle(buffer, 32, 32, 10, 1);
//	testchar(buffer);
	lcd.write_buffer(buffer);

  return 0;
}
