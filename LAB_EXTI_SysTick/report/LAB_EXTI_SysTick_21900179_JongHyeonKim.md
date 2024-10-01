# LAB: EXTI & SysTick

**Date:** 2024-10-01

**Author/Partner:** Jong-Hyeon Kim

**Github:** [repository link](https://github.com/Dandelion-Kim/Embedded-Controller/tree/main/LAB_EXTI_SysTick)

**Demo Video:** [Problem1](https://youtu.be/iBhi3L1FzqQ) / [Problem2](https://youtu.be/fU-yqGHGPMI)



## Introduction

In this lab, you are required to create two simple programs using interrupt:

(1) displaying the number counting from 0 to 9 with Button Press

(2) counting at a rate of 1 second (If button is pushed, RESET.)



### Requirement

#### Hardware

- MCU
  - NUCLEO-F411RE
- Actuator/Sensor/Others:
  - 4 LEDs and load resistance
  - 7-segment display(5101ASR)
  - Array resistor (330 ohm)
  - breadboard

#### Software

- Keil uVision, CMSIS, EC_HAL library

------

## Problem 1: Counting numbers on 7-Segment using EXTI Button

### 1-1. Create HAL library

**ecEXTI.h**

```c
#ifndef __EC_EXTI2_H
#define __EC_EXTI2_H

#include "stm32f411xe.h"
#include "ecPinNames.h"

#define FALL 0
#define RISE 1
#define BOTH 2

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


void EXTI_init(PinName_t pinName, int trig_type,int priority);
void EXTI_enable(PinName_t pinName);
void EXTI_disable(PinName_t pinName);
uint32_t is_pending_EXTI(PinName_t pinName);
void clear_pending_EXTI(PinName_t pinName);


#ifdef __cplusplus
}
#endif /* __cplusplus */
	 
#endif // __EC_EXTI2_H
```

**ecEXTI.c**

```c
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

```



### 1-2. Procedure

1. Use the decoder chip (**74LS47**). Connect it to the breadboard and 7-segment display.

   > Then, you need only 4 Digital out pins of MCU to display from 0 to 9.

2. First, check if every number, 0 to 9, can be displayed properly on the 7-segment.

3. Then, create a code to display the number counting from 0 to 9 and repeating.

   - Count up only by pressing the push button (External Interrupt)



### Configuration

| Digital In for Button (B1) | Digital Out for 7-Segment decoder           |
| :------------------------- | :------------------------------------------ |
| Digital In                 | Digital Out                                 |
| PC13                       | PA7, PB6, PC7, PA9                          |
| PULL-UP                    | Push-Pull, No PullUp-PullDown, Medium Speed |



### Circuit Diagram

![[LAB2]problem1](.\img\[LAB2]problem1,2.png)

### Discussion

1. We can use two different methods to detect an external signal: polling and interrupt. What are the advantages and disadvantages of each approach?

> The advantage of polling is that it is simple to implement and can make requests collectively. However, there is a disadvantage of great time loss, and even if an urgent request is received until the loop being executed is finished, it cannot be accepted.
> Conversely, the advantage of interrupt is that it is efficient. For example, if you always wait for someone's phone, or if you end up checking it every 10 seconds, it consumes a lot of energy. This is polling. However, at the point of receiving the bell only, you may be continuously doing other tasks and there is no energy consumption to confirm. This is the advantage of interrupt. It only intervenes when a call comes in. On the other hand, I think the disadvantage of interrupt lies in complex implementations rather than polling.



2. What would happen if the EXTI interrupt handler does not clear the interrupt pending flag? Check with your code

> ``` c
> /*
> *	When an interrupt starts and it's ready, you have to send a pending signal that you're ready.
> *	So in this function, flag the pending with the signal bit 1 that's ready
> */
> uint32_t is_pending_EXTI(PinName_t pinName) {
> 	GPIO_TypeDef *port;
> 	unsigned int pin;
> 	ecPinmap(pinName, &port , &pin);
> 	
> 	uint32_t EXTI_PRx = 1<<pin;     	// check  EXTI pending 	
> 	return ((EXTI->PR & EXTI_PRx) == EXTI_PRx);
> }
> /*
> *	If you have sent the fending, you need to clear it again to make it work normally.
> * At this time, put bit 1 in accordance with arm Cortex to clear it
> */
> void clear_pending_EXTI(PinName_t pinName) {
> 	GPIO_TypeDef *port;
> 	unsigned int pin;
> 	ecPinmap(pinName, &port , &pin);
> 	//EXTI->PR |= 1<<pin;     // clear EXTI pending 
> }
> ```
>
> I annotated the clear bit part to see what happens if the bending bit is not cleared at the end. As a result, it was confirmed that no matter how many times you press the button, it is stopped at the number '0'.
>
> ```c
> /*
> 	---EXTI15_10_IRQHandler---
> 	The EXTI_handler stops the main and the EXTI_handler is called in hardware when the interrupt is created with the ISR routine.
> 	After processing the subroutines in this, you start again with the main function line that was previously finished by unstacking.
> */
> void EXTI15_10_IRQHandler(void) {
> 	if (is_pending_EXTI(BUTTON_PIN)) {
> 		LED_upCounter();
> 		clear_pending_EXTI(BUTTON_PIN); 
> 	}
> }
> ```
>
> When I analyzed the code, if (is_pending_EXTI(BUTTON_PIN)) didn't clear the pending bit here, so it kept repeating indefinitely, and no action worked. So display keeps outputting 0.



### Code

Your code goes here.

Explain your source code with the necessary comments.

Copy

```c
#include "ecSTM32F4v2.h"


int count = 0;								// Count for displaying 7-seg display
void LED_upCounter(void);								


int main(void) {
	
	// Initialiization 
	custom_init();

	while (1) {}
}


/*
	---EXTI15_10_IRQHandler---
	The EXTI_handler stops the main and the EXTI_handler is called in hardware when the interrupt is created with the ISR routine.
	After processing the subroutines in this, you start again with the main function line that was previously finished by unstacking.
*/
void EXTI15_10_IRQHandler(void) {
	if (is_pending_EXTI(BUTTON_PIN)) {
		LED_upCounter();
		clear_pending_EXTI(BUTTON_PIN); 
	}
}

/*	
	---Function - LED_upCounter---
	When the loop below receives a decimal number, it toggles to a binary number.
	Count is the number to be displayed, and up to that number,
	A, B, C, and D toggles 0 and 1 in the order of bits to form a binary number.
	If you signal A, B, C, and D from that result to the 7-seg decoder,
	you will get the number you want to output accurately.
	
	BCD_A : A pin for 7-segment-Decoder												
	BCD_B : B pin for 7-segment-Decoder														
	BCD_C : C pin for 7-segment-Decoder											
	BCD_D : A pin for 7-segment-Decoder

	DIGIT_OUT0 : PA_7
	DIGIT_OUT1 : PB_6
	DIGIT_OUT2 : PC_7
	DIGIT_OUT3 : PA_9
*/
void LED_upCounter(void)	{
		
	int BCD_A= 0;								// PA_7 - A (LSB: Least Siginificant Bit)
	int BCD_B= 0;								// PB_6 - B
	int BCD_C= 0;								// PC_7 - C
	int BCD_D= 0;								// PA_9 - D (MSB: Least Siginificant Bit)
	
	for(int i=1;i<=count;i++){
				 BCD_A = !BCD_A;							// Toggle every iteration.
		if(i%2==0) BCD_B = !BCD_B;							// Toggle at the even number
		if(i%4==0) BCD_C = !BCD_C;							// Toggle every 4th time.
		if(i%8==0) BCD_D = !BCD_D;							// Toggle every 8th time.
	}

	GPIO_write(DIGIT_OUT0, BCD_A);							
	GPIO_write(DIGIT_OUT1, BCD_B);
	GPIO_write(DIGIT_OUT2, BCD_C);
	GPIO_write(DIGIT_OUT3, BCD_D);

	count++;
	if(count>9) count=0;	// The linmit of 7-seg display is '9'. Reset to display '0'
}
```



### Results

* Circuit Img

<div style="display: flex; flex-wrap: wrap;">     
    <img src=".\img\[LAB2]'0'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]'1'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]'2'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]'3'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]'4'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]'5'.jpg" alt="17112_07" style="width: 30%; margin: 5px;">     
    <img src=".\img\[LAB2]'6'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">     
    <img src=".\img\[LAB2]'7'.jpg" alt="이미지3" style="width: 30%; margin: 5px;"> 
    <img src=".\img\[LAB2]'8'.jpg" alt="이미지3" style="width: 30%; margin: 5px;"> 
    <img src=".\img\[LAB2]'9'.jpg" alt="이미지3" style="width: 30%; margin: 5px;"> 
</div>
* Video

[Watch the video of Problem1](https://youtu.be/iBhi3L1FzqQ)

---------

## Problem 2: Counting numbers on 7-Segment using SysTick

Display the number 0 to 9 on the 7-segment LED at the rate of 1 sec. After displaying up to 9, then it should display ‘0’ and continue counting.

When the button is pressed, the number should be reset ‘0’ and start counting again.



### 2-1. Create HAL library

**ecSysTick.h**

```c
#ifndef __EC_SYSTICK2_H
#define __EC_SYSTICK2_H

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

void SysTick_init(void);
void SysTick_Handler(void);
void SysTick_counter();
void delay_ms(uint32_t msec);
void SysTick_reset(void);
uint32_t SysTick_val(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __EC_SYSTICK2_H

```

**ecSysTick.c**

```c
#include "ecSysTick2.h"

#define MCU_CLK_PLL 84000000
#define MCU_CLK_HSI 16000000

volatile uint32_t msTicks;

//EC_SYSTEM_CLK

void SysTick_init(void){	
	//  SysTick Control and Status Register
	SysTick->CTRL = 0;											// Disable SysTick IRQ and SysTick Counter

	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

	// uint32_t MCU_CLK=EC_SYSTEM_CLK
	// SysTick Reload Value Register
	SysTick->LOAD = MCU_CLK_PLL / 1000 - 1;						// 1ms, for HSI PLL = 84MHz.

	// SysTick Current Value Register
	SysTick->VAL = 0;

	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
		
	NVIC_SetPriority(SysTick_IRQn, 16);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);			// Enable interrupt in NVIC
}



void SysTick_Handler(void){
	SysTick_counter();	
}

void SysTick_counter(){
	msTicks++;
}	


void delay_ms (uint32_t mesc){
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < mesc);
	
  msTicks = 0;
}




void SysTick_reset(void)
{
	// SysTick Current Value Register
	SysTick->VAL = 0;
}

uint32_t SysTick_val(void) {
	return SysTick->VAL;
}



```



### 2-2. Procedure

1. Use the decoder chip (**74LS47**). Connect it to the bread board and 7-segment display.

   > Then, you need only 4 Digital out pins of MCU to display from 0 to 9.

2. First, check if every number, 0 to 9, can be displayed properly on the 7-segment.

3. Then, create a code to display the number counting from 0 to 9 and repeats at the rate of 1 second.

4. When the button is pressed, it should start from '0' again.

   > Use EXTI for this button reset.



### Configuration

| Digital In for Button (B1) | Digital Out for 7-Segment decoder             |
| :------------------------- | :-------------------------------------------- |
| Digital In                 | Digital Out                                   |
| PC13                       | PA7, PB6, PC7, PA9                            |
| PULL-UP                    | Push-Pull, No Pull-up-Pull-down, Medium Speed |



### Circuit Diagram

![[LAB2]problem1](.\img\[LAB2]problem1,2.png)

### Code

Your code goes here.

Explain your source code with necessary comments.

Copy

```c
#include "ecSTM32F4v2.h"

// Initialiization 
int count=0;

int main(void) { 
	// Initialiization --------------------------------------------------------
	custom_init();
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		sevensegment_display(count);
		count++;
		delay_ms(1000);
		
		if (count >9) count =0;
		SysTick_reset();
		//if (BUTTON_PIN==0) count=0;
	}
}
/*
	---EXTI15_10_IRQHandler---
	The EXTI_handler stops the main and the EXTI_handler is called in hardware when the interrupt is created with the ISR routine.
	After processing the subroutines in this, you start again with the main function line that was previously finished by unstacking.
*/
void EXTI15_10_IRQHandler(void) {
	if (is_pending_EXTI(BUTTON_PIN)) {				// If it is ready to pending,
		count=0;
		clear_pending_EXTI(BUTTON_PIN); 				// Clear the pendding bit
	}
}

/*	
	---Function - sevensegment_display---
	When the loop below receives a decimal number, it toggles to a binary number.
	Count is the number to be displayed, and up to that number,
	A, B, C, and D toggles 0 and 1 in the order of bits to form a binary number.
	If you signal A, B, C, and D from that result to the 7-seg decoder,
	you will get the number you want to output accurately.
	
	BCD_A : A pin for 7-segment-Decoder												
	BCD_B : B pin for 7-segment-Decoder														
	BCD_C : C pin for 7-segment-Decoder											
	BCD_D : A pin for 7-segment-Decoder
*/
void sevensegment_display(uint8_t count)	{
		
	int BCD_A= 0;								// PA_7 - A (LSB: Least Siginificant Bit)
	int BCD_B= 0;								// PB_6 - B
	int BCD_C= 0;								// PC_7 - C
	int BCD_D= 0;								// PA_9 - D (MSB: Least Siginificant Bit)
	
	for(int i=1;i<=count;i++){
				   BCD_A = !BCD_A;							// Toggle every iteration.
		if(i%2==0) BCD_B = !BCD_B;							// Toggle at the even number
		if(i%4==0) BCD_C = !BCD_C;							// Toggle every 4th time.
		if(i%8==0) BCD_D = !BCD_D;							// Toggle every 8th time.
	}

	GPIO_write(DIGIT_OUT0, BCD_A);							
	GPIO_write(DIGIT_OUT1, BCD_B);
	GPIO_write(DIGIT_OUT2, BCD_C);
	GPIO_write(DIGIT_OUT3, BCD_D);
}
```



### Results

* Circuit IMG

<div style="display: flex; flex-wrap: wrap;">     
    <img src=".\img\[LAB2]Pro2_'0'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]Pro2_'1'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]Pro2_'2'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]Pro2_'3'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]Pro2_'4'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">  
    <img src=".\img\[LAB2]Pro2_'5'.jpg" alt="17112_" style="width: 30%; margin: 5px;">   
    <img src=".\img\[LAB2]Pro2_'6'.jpg" alt="이미지2" style="width: 30%; margin: 5px;">   
    <img src=".\img\[LAB2]Pro2_'7'.jpg" alt="이미지3" style="width: 30%; margin: 5px;"> 
    <img src=".\img\[LAB2]Pro2_'8'.jpg" alt="이미지3" style="width: 30%; margin: 5px;"> 
    <img src=".\img\[LAB2]Pro2_'9'.jpg" alt="이미지3" style="width: 30%; margin: 5px;">
    <img src=".\img\[LAB2]Pro2_'reset'.jpg" alt="이미지" style="width: 30%; margin: 5px;">
    <img src=".\img\[LAB2]Pro2_'interrupt'.jpg" alt="img" style="width: 30%; margin: 5px;">
</div>

* Video

[Watch the video of Problem2](https://youtu.be/fU-yqGHGPMI)



## Reference

```
- https://ykkim.gitbook.io/ec
- Y.Zhu(2017). Embedded Systems with Arm Cortex-M Microcontrollers in Assembly Language and C: Third Edition
```



