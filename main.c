/**
  ******************************************************************************
  * @file    	main.c 
  * @author  	Alfred Lohmann
  *        	  HAW-Hamburg
  *          	Labor für technische Informatik
  *          	Berliner Tor  7
  *          	D-20099 Hamburg
  * @version V1.0
  * @date    23.05.2013
  * @brief   Main program body
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"

#include "header.h"

//--- For GPIOs -----------------------------
//Include instead of "stm32f4xx.h" for
//compatibility between Simulation and Board
//#include "TI_memory_map.h"

//--- For Touch Keypad ----------------------
//#include "keypad.h"

//--- For Timer -----------------------------
#include "timer.h"


int countR = 0;
int countL = 0;
int currentState = 0;

int main(void)
{
  Init_TI_Board();
	timerinit();
	
	TIM2->ARR = 0xDAC0;   //84000000
	TIM2->PSC = 300;
	TIM2->SR &= ~0x1;
	
	int input;
	int leds;
	
	int lastState = 3;
	int taste5;
	int taste5alt;
	
	TFTausgabe();
	
	while(1)
	{
		input = GPIOE->IDR;
		currentState = input & 0x03;
		taste5 = input & 0x20;
		
		leds = GPIOG->IDR & 0xFFF0;
		leds = leds | currentState;
		//GPIOG->BSRRH = 0xF;
		//GPIOG->BSRRL = currentState;	
		GPIOG->ODR = leds;
		
		
		
		if(taste5 != taste5alt)
		{
			countL = 0;
			countR = 0;
			GPIOG->BSRRH = 0xFFF0;
			taste5alt = taste5;
		}
		
		if(currentState != lastState)
		{
			switch(currentState)
			{
				case 0: 
					if(lastState == 2)
					{
						countL++;
						printCount();
					}
					else
					{
						countR++;
						printCount();
					}
					break;
				case 1:
					if(lastState == 0)
					{
						countL++;
						printCount();
					}
					else 
					{
						countR++;
						printCount();
					}
					break;
				case 2:
					if(lastState == 3)
					{
						countL++;
						printCount();
					}
					else
					{
						countR++;
						printCount();
					}
					break;
				case 3:
					if(lastState == 1)
					{
						countL++;
						printCount();
					}
					else
					{
						countR++;
						printCount();
					}
					break;
			}
		}
		
		if((TIM2->SR & 0x01) == 1)
		{
			TIM2->SR &= ~0x01;
			TFTausgabe();
		}
		
		lastState = currentState;
	}
}

void printCount()
{
	int leds = GPIOG->IDR & 0x000F;
	leds = leds + (countL << 4);
	leds = leds + (countR << 12);
	GPIOG->ODR = leds;
	//GPIOG->BSRRL = (countL << 4) + currentState;
	//GPIOG->BSRRL = (countR << 12) + currentState;
}

void TFTausgabe()
{
	TFT_cls();
	char ostr[100];
	
	sprintf(ostr, "Links Rotationen: %d \n\rRechts Rotationen: %d", countL, countR);
	
	TFT_puts(ostr);
}

// EOF
