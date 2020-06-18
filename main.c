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

int input;
int countR = 0;
int countL = 0;
int lastState = 3;
int currentState = 0;
int taste5;
int taste5alt;

int main(void)
{
  Init_TI_Board();
	
	while(1)
	{
		input = GPIOE->IDR;
		currentState = input & 0x03;
		GPIOG->BSRRH = 0xF;
		GPIOG->BSRRL = currentState;	
		taste5 = input & 0x20;
		
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
		lastState = currentState;
	}
}

void printCount()
{
	GPIOG->ODR = GPIOG->IDR & 0x000F;
	GPIOG->BSRRL = (countL << 4) + currentState;
	GPIOG->BSRRL = (countR << 12) + currentState;
}


// EOF
