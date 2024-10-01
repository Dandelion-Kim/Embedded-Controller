/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Jong-Hyeon Kim
Created          : 2024-09-27
Modified         : 2024-10-01
Language/ver     : C++ in Keil uVision

Description      : Source file for External Interrupt
/----------------------------------------------------------------*/

#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"


void EXTI_init(PinName_t pinName, int trig_type,int priority){

	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName,&port,&pin);
	// SYSCFG peripheral clock enable	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	
	// Connect External Line to the GPIO
	int EXTICR_port;
	if			(port == GPIOA) EXTICR_port = 0;
	else if	(port == GPIOB) EXTICR_port = 1;
	else if	(port == GPIOC) EXTICR_port = 2;
	else if	(port == GPIOD) EXTICR_port = 3;
	else 										EXTICR_port = 4;
	
	SYSCFG->EXTICR[pin/4] &= ~SYSCFG_EXTICR4_EXTI13 ;			// clear 4 bits
	SYSCFG->EXTICR[pin/4] |= EXTICR_port<<SYSCFG_EXTICR4_EXTI13_Pos;			// set 4 bits
	
	// Configure Trigger edge
	if (trig_type == FALL) EXTI->FTSR |=  1 << pin;   // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= 1 << pin;   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable
		EXTI->RTSR |= 1 << pin; 
		EXTI->FTSR |= 1 << pin;
	} 
	
	// Configure Interrupt Mask (Interrupt enabled)
	EXTI->IMR  |= 1 << pin;     // not masked
	
	
	// NVIC(IRQ) Setting
	int EXTI_IRQn = 0;
	
	if (pin < 5) 	EXTI_IRQn = pin+6;
	else if	(pin < 10) 	EXTI_IRQn = 23;
	else 			EXTI_IRQn = 40;
								
	NVIC_SetPriority(EXTI_IRQn, EXTI_IRQn+7);	// EXTI priority
	NVIC_EnableIRQ(EXTI_IRQn); 	// EXTI IRQ enable
}

// Enable EXTI
void EXTI_enable(PinName_t pinName) {
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port , &pin);
	
	EXTI->IMR |= 1<<pin;     // not masked (i.e., Interrupt enabled)
}

// Disable EXTI
void EXTI_disable(PinName_t pinName) { 
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port , &pin);
	
	EXTI->IMR &= ~(1<<pin);     // masked (i.e., Interrupt disabled)
}

/*
*	When an interrupt starts and it's ready, you have to send a pending signal that you're ready.
*	So in this function, flag the pending with the signal bit 1 that's ready
*/
uint32_t is_pending_EXTI(PinName_t pinName) {
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port , &pin);
	
	uint32_t EXTI_PRx = 1<<pin;     	// check  EXTI pending 	
	return ((EXTI->PR & EXTI_PRx) == EXTI_PRx);
}
/*
*	If you have sent the fending, you need to clear it again to make it work normally.
* At this time, put bit 1 in accordance with arm Cortex to clear it
*/
void clear_pending_EXTI(PinName_t pinName) {
	GPIO_TypeDef *port;
	unsigned int pin;
	ecPinmap(pinName, &port , &pin);
	EXTI->PR |= 1<<pin;     // clear EXTI pending 
}
