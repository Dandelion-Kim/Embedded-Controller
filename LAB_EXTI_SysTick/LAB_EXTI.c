/**
******************************************************************************
* @course   		: Embedded Controller- HGU
* @author				: Jong-Hyeon Kim
* @Created			: 2024-09-30
* @mod					: 2024-10-01
* @brief				: [LAB]EXTI
******************************************************************************
*/

#include "ecSTM32F4v2.h"


int count = 0;													// Count for displaying 7-seg display
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
		
	int BCD_A= 0;															// PA_7 - A (LSB: Least Siginificant Bit)
	int BCD_B= 0;															// PB_6 - B
	int BCD_C= 0;															// PC_7 - C
	int BCD_D= 0;															// PA_9 - D (MSB: Least Siginificant Bit)
	
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
	if(count>9) count=0;											// The linmit of 7-seg display is '9'. Reset to display '0'
}