/*
** Four-channel mini logic analyzer (Firmware v1.01). 
**
** Target: ATmega8 at 16MHz crystal
**
** Input pins PD0-PD3
**
** Frimware v1.00 was written by Vassilis Serasidis on 01 January 2012. 
** http://www.serasidis.gr
** avrsite@yahoo.gr, info@serasidis.gr
**
** IDE: AVRstudio 4.18 with AVR-gcc plugin (WinAVR-20100110).
**
** Nokia 3310 / 5110 gLCD library (84*48 pixels) was written by Tony Myatt - 2007
** Quantum Torque - www.quantumtorque.com
** Small modifications to the gLCD source code library had been made by Vassilis Serasidis.
** 
** 3310/5110 LCD is connected on AVR's PORT B
** See <lcd.h> file for pins assignment between AVR and 3310/5110 LCD.
**
**   Nokia LCD    ATmega8
**  ------------  -------
**  LCD_CLK_PIN     PB4
**  LCD_DATA_PIN    PB3
**  LCD_DC_PIN      PB2
**  LCD_RST_PIN     PB1
**  LCD_CE_PIN      PB0
**	
**   - History -
**
**   v1.01 (03 February 2012) Capturing speed was increased from 100kHz to 400kHz.
**   v1.00 (01 January 2012)  Initioal version works at 100kHz cpturing speed.
**  
**
**   This source code is distributed under GPLv3 (General Public License version 3) 
**
*/
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include "introScreen.h"
#include "createdBy.h"

#define BUFFER_SIZE  255 //255 Sample transitions * 3 bytes for each sample = 765 bytes.

#define POSITIONS_LENGTH 16711170 //(BUFFER_SIZE/3) * MAX_SAMPLE_TIME = (765/3) * 65534 = 16711170
#define ONE_LINE_LENGTH  84
#define MAX_SAMPLE_TIME 65534

#define IN1 0 
#define IN2 1
#define IN3 2 
#define IN4 3
#define ONE  32
#define ZERO  4
#define TRANSITION 60
#define MIDDLE 36
#define FALSE 1
#define TRUE 0

#define BTN_INCREASE   PD4
#define BTN_DECREASE   PC5 
#define BTN_ZOOM_OUT   PD5
#define LED1           PB5

//unsigned char dataBuffer[BUFFER_SIZE];

unsigned int  lengthBuffer[BUFFER_SIZE];
unsigned char dataBuffer[BUFFER_SIZE];

volatile unsigned int  i, counter, bufferUsedSpace, zoom, minSampleTime;
volatile unsigned long  samplesPos;
char int2str[8];

void checkIfNoButtonIsPressed (void);
void checkDecreaseButton (void);
void checkIncreaseButton (void);
void printCapturedData (unsigned long position, unsigned char inputChannel);
void printRuler (void);
//void saveSampleToBuffer (void);
void checkInputs (void);
void sendChannelsDataOnLCD (void);
void getNextSampleString (void);

//char createdBy1 [] PROGMEM = {"Vassilis"};
//char createdBy2 [] PROGMEM = {"Serasidis"};
//char createdBy3 [] PROGMEM = {"(c)01 Jan 2012"};
//char createdBy4 [] PROGMEM = {"www.serasidis.gr"};

//========================================================================
//  Main program 
//========================================================================
int main(void)
{	
	
	DDRD = 0x00;
	PORTD = 0x00;

	DDRC = 0x00;
	PORTC = 0xff;

	DDRB |= (1<<LED1);

	PORTB &= ~(1<<LED1);
	PORTD |= (1<<BTN_INCREASE);    //Enable pull-up resistor on BTN_DECREASE pin.
	PORTC |= (1<<BTN_DECREASE);    //Enable pull-up resistor on BTN_INCREASE pin.
	PORTD |= (1<<BTN_ZOOM_OUT);    //Enable pull-up resistor on BTN_ZOOM_OUT pin.

///*
	_delay_ms(100);

	lcd_init();

	_delay_ms(100);

	lcd_contrast(0x40);

	printPictureOnLCD(introScreen);
	//_delay_ms(2000);
	printPictureOnLCD(createdBy);
	
	samplesPos = 0;
	bufferUsedSpace = 0;
	zoom = 1;
	minSampleTime = MAX_SAMPLE_TIME;
	counter = 0;

	lcd_clear();
	lcd_goto_xy(3,3);
	lcd_str("Waiting for");
	lcd_goto_xy(4,4);
	lcd_str("signal...");
//*/	
	checkInputs(); // Stay here until a logic level change will be made on PORT D.
	               // Then, read all changes on PORT D until dataBuffer will be full.

	lcd_clear();
	lcd_goto_xy(14,1);
	lcd_chr('1');
	printRuler();
	sendChannelsDataOnLCD();
	
	for(;;)
	{
		checkDecreaseButton();
		checkIncreaseButton();
		checkIfNoButtonIsPressed();
	
	}
}


//========================================================================
//
//========================================================================
//
//void getNextSampleString (void)
//{
//	unsigned int w;
//
//	if(bit_is_clear(PINC,BTN_DECREASE))
//	{
//		for(w=0; w<BUFFER_SIZE;w++) //Clear the buffer for next sample string.
//			dataBuffer[w] = 0xFF;
//
//		samplesPos = 0;
//		bufferUsedSpace = 0;
//		zoom = 0;
//		minSampleTime = MAX_SAMPLE_TIME;
//		counter = 0;
//		
//		lcd_clear();
//		lcd_goto_xy(3,3);
//		lcd_str("Waiting for");
//		lcd_goto_xy(4,4);
//		lcd_str("signal...");
//		checkInputs();
//		lcd_clear();
//		lcd_goto_xy(14,1);
//		lcd_chr('1');
//		printRuler();
//	}
//}
//
//========================================================================
//
//========================================================================
void checkInputs (void)
{

	unsigned char dataUsed = 0, dataPins;
	unsigned int counter2;

	dataPins = PIND; //Read the pins status on Port D (this is the initial PORT D status).
	while (dataPins == PIND); //Stay here until at least one pin on PORT D changes its Logic status.
	counter2 = 0;

	PORTB |= (1<<LED1); //Turn-ON the LED. Start recording sample lengths to the AVR's dataBuffer.
	while(dataUsed < (BUFFER_SIZE)) //While the used buffer is not full, repeat this loop.
	{
		if((dataPins == PIND)&&(counter2 < MAX_SAMPLE_TIME))// If the PORT D has the same status as before, just...
			counter2++;
		else  // or else,
		{
			dataPins = PIND;      //Read the pins status on Port D.
			lengthBuffer[dataUsed] = counter2; // HIGH byte of counter
			dataBuffer[dataUsed] = dataPins;  // Read the pins status on Port D and save it to the RAM buffer.
			counter2 = 0;
			dataUsed++;
		}
	}

	PORTB &= ~(1<<LED1); //Turn-OFF the LED. That means that AVR has filled its dataBuffer.
}
//========================================================================
//
//========================================================================
void checkIfNoButtonIsPressed (void)
{
	unsigned int w;

	if(bit_is_clear(PIND,BTN_ZOOM_OUT))
	{
		while(bit_is_clear(PIND,BTN_ZOOM_OUT));
		//getNextSampleString();
		if((zoom > 0)&&(zoom < 8192))
			zoom *= 2;
		else
			zoom = 1;
		
		lcd_clear_area(1,61,83);
		itoa(zoom,int2str,10);
		w = strlen(int2str);
		lcd_goto_xy(15 - w,1);
		lcd_str(int2str);

		sendChannelsDataOnLCD();
	}
}

//========================================================================
//
//========================================================================
void checkIncreaseButton (void)
{
	if(bit_is_clear(PIND,BTN_INCREASE)&&(samplesPos < POSITIONS_LENGTH)) //Scroll to the left the waveform content.
	{

		if(counter < MAX_SAMPLE_TIME)
		{
			if(samplesPos < (POSITIONS_LENGTH - zoom))
				samplesPos += zoom;
			sendChannelsDataOnLCD();
		}
	}
}		

//========================================================================
//
//========================================================================
void checkDecreaseButton (void)
{
	if(bit_is_clear(PINC,BTN_DECREASE)&&(samplesPos > 0)) //Scroll to the right the waveform content.
	{
		if(counter < MAX_SAMPLE_TIME)
		{
			if(samplesPos >= zoom)
				samplesPos -= zoom;
			sendChannelsDataOnLCD();
		}
	}
}

//========================================================================
//
//========================================================================
void printCapturedData (unsigned long position, unsigned char inputChannel)
{
	unsigned int m,j, b,sampleLength,n,samplLength;
	unsigned char w, sample, nextSample = 0;
	unsigned long lengthSum,oldLength;

	lcd_clear_area(1,1,42);

	ltoa(position,int2str,10);
	w = strlen(int2str);
	lcd_goto_xy(9-w,1);
	lcd_str(int2str);
	lcd_goto_xy(1,inputChannel + 3); //Go to LCD line 3, 4, 5 or 6.

	n = 0;
	w = 1;
	b = 0;
	lengthSum = 0;
	oldLength = 0;
	samplLength = 0;

	sampleLength = lengthBuffer[b];
	do
	{
		lengthSum += sampleLength;
		if(samplesPos > lengthSum)
		{
			oldLength += sampleLength;
			b++;
			sampleLength = lengthBuffer[b];
		}
	}while(samplesPos > lengthSum);

	for(m=b;m<BUFFER_SIZE;m++)
	{
		sampleLength = lengthBuffer[m];
		if(w > 0)
		{
			sampleLength -= (samplesPos - oldLength);
			w=0;			
		}
		
		sample = dataBuffer[m];
		if((m+1) < (BUFFER_SIZE))
		{
			nextSample = dataBuffer[m+1]; 
			nextSample ^= sample;
		}
		samplLength = sampleLength;
		if(zoom > 0)
			samplLength /= zoom; 

		for(j=0;j<samplLength;j++)
		{
			if(bit_is_set(sample,inputChannel))
				lcd_col(ONE); //1
			else
				lcd_col(ZERO);//0
			n++;

			if(n >= ONE_LINE_LENGTH)
				break;
		}
		if(bit_is_set(nextSample,inputChannel))
		{
				lcd_pixelBack();
				lcd_col(TRANSITION);
				nextSample = 0; //Set nextSample to <No Sample> status.
		}

		if(n >= ONE_LINE_LENGTH)
			break;
	}
}


//========================================================================
//
//========================================================================
void printRuler (void)
{
	unsigned char k;

	lcd_goto_xy(1,2); //Go to LCD line 2.

	for(k=0;k<8;k++)
	{
		lcd_col(TRANSITION);
		lcd_col(0b00000000);
		lcd_col(0b00011000);
		lcd_col(0b00000000);
		lcd_col(0b00011000);
		lcd_col(0b00000000);
		lcd_col(0b00011000); 
		lcd_col(0b00000000);
		lcd_col(0b00011000);
		lcd_col(0b00000000);
	}
	lcd_col(TRANSITION);
	lcd_col(0b00000000);
	lcd_col(0b00011000);
	lcd_col(0b00000000);
}

//========================================================================
//
//========================================================================
//void saveSampleToBuffer (void)
//{
//	if(minSampleTime > counter)
//		minSampleTime = counter;
//
//	dataBuffer[bufferUsedSpace++] = (counter/256);  // HIGH byte of counter		
//	dataBuffer[bufferUsedSpace++] = counter;        // LOW byte of counter	
//	dataBuffer[bufferUsedSpace++] = PIND;           // Read the pins status on Port D and save it to the RAM buffer.
//	samplesPos = 0;
//	counter = 0; 
//}

//========================================================================
//
//========================================================================
void sendChannelsDataOnLCD (void)
{
	printCapturedData(samplesPos, IN1);
	printCapturedData(samplesPos, IN2);
	printCapturedData(samplesPos, IN3);
	printCapturedData(samplesPos, IN4);
}

