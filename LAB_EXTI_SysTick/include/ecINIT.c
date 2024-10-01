/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Jong-Hyeon Kim
Created          : 2024-09-10
Modified         : 2024-09-24
Language/ver     : C++ in Keil uVision

Description      : Source file for Reset and Clock Control
/----------------------------------------------------------------*/

#include "ecINIT.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"


void custom_init(void) {
	RCC_PLL_init();                         // System Clock = 84MHz
	SysTick_init();
	// Initializing for the each OUTPUT
	GPIO_init(DIGIT_OUT0, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_pupd(DIGIT_OUT0, EC_NP);			// No Pull-Up & No Pull-Down
	GPIO_otype(DIGIT_OUT0, PSPL);			// Push-Pull
	GPIO_ospeed(DIGIT_OUT0, MSPEED);  // Medium Speed
	
	GPIO_init(DIGIT_OUT1, OUTPUT);    // calls RCC_GPIOB_enable()	
	GPIO_pupd(DIGIT_OUT1, EC_NP);			// No Pull-Up & No Pull-Down
	GPIO_otype(DIGIT_OUT1, PSPL);			// Push-Pull
	GPIO_ospeed(DIGIT_OUT1, MSPEED);  // Medium Speed
	
	GPIO_init(DIGIT_OUT2, OUTPUT);    // calls RCC_GPIOC_enable()	
	GPIO_pupd(DIGIT_OUT2, EC_NP);			// No Pull-Up & No Pull-Down
	GPIO_otype(DIGIT_OUT2, PSPL);			// Push-Pull
	GPIO_ospeed(DIGIT_OUT2, MSPEED);  // Medium Speed
	
	GPIO_init(DIGIT_OUT3, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_pupd(DIGIT_OUT3, EC_NP);			// No Pull-Up & No Pull-Down
	GPIO_otype(DIGIT_OUT3, PSPL);			// Push-Pull
	GPIO_ospeed(DIGIT_OUT3, MSPEED);  // Medium Speed
	
	

	// Initialize GPIOC_13 for Input Button
	GPIO_init(BUTTON_PIN, INPUT); 		// calls RCC_GPIOC_enable()
	GPIO_pupd(BUTTON_PIN, EC_PU);			// Pull-Up
	EXTI_init(BUTTON_PIN, FALL, 0);		// Button interrupt Falling Edge with the priority = 0
}