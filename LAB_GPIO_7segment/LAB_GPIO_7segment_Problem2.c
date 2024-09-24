/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Jong-Hyeon Kim
Created          : 2024-09-10
Modified         : 2024-09-24
Language/ver     : C++ in Keil uVision

Description      : LAB_GPIO_7segment_Problem2
/----------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "ecGPIO2.h"
#include "ecRCC2.h"

void setup(void);

int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	unsigned int cnt = 0;
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
		sevensegment_decoder(cnt %10); 						// Give the argument "cnt%10" to the fucntion 
		if(GPIO_read(BUTTON_PIN) == 0) cnt++; 
        if (cnt > 9) cnt = 0;
		for(int i = 0; i < 500000;i++){} 				 		// delay_ms(500);
	}
}

// Initialization 
void setup(void){
	RCC_HSI_init();	
	GPIO_init(BUTTON_PIN, INPUT);  														// calls RCC_GPIOC_enable()
	sevensegment_decoder_init();															// Call for initialization
}


// Initialization for 7-segment_decoder function
void sevensegment_decoder_init(void){
	GPIO_init(PA_5, OUTPUT);   		// calls RCC_GPIOA_enable() / OUTPUT
	GPIO_ospeed(PA_5, MSPEED);		// Medium-Speed
	GPIO_pupd(PA_5, EC_NP);				// No Pull up, Pull down
	GPIO_otype(PA_5, PSPL);		 		// Push-Pull
	
	GPIO_init(PA_6, OUTPUT);   		// calls RCC_GPIOA_enable() / OUTPUT
	GPIO_ospeed(PA_6, MSPEED);		// Medium-Speed
	GPIO_pupd(PA_6, EC_NP);				// No Pull up, Pull down
	GPIO_otype(PA_6, PSPL);		 		// Push-Pull
	
	GPIO_init(PA_7, OUTPUT);   		// calls RCC_GPIOA_enable() / OUTPUT
	GPIO_ospeed(PA_7, MSPEED);		// Medium-Speed
	GPIO_pupd(PA_7, EC_NP);				// No Pull up, Pull down
	GPIO_otype(PA_7, PSPL);		 		// Push-Pull
	
	GPIO_init(PB_6, OUTPUT);   		// calls RCC_GPIOB_enable() / OUTPUT
	GPIO_ospeed(PB_6, MSPEED);		// Medium-Speed
	GPIO_pupd(PB_6, EC_NP);				// No Pull up, Pull down
	GPIO_otype(PB_6, PSPL);		 		// Push-Pull
	
	GPIO_init(PC_7, OUTPUT);   		// calls RCC_GPIOC_enable() / OUTPUT
	GPIO_ospeed(PC_7, MSPEED);		// Medium-Speed
	GPIO_pupd(PC_7, EC_NP);				// No Pull up, Pull down
	GPIO_otype(PC_7, PSPL);		 		// Push-Pull
	
	GPIO_init(PA_9, OUTPUT);   		// calls RCC_GPIOA_enable() / OUTPUT
	GPIO_ospeed(PA_9, MSPEED);		// Medium-Speed
	GPIO_pupd(PA_9, EC_NP);				// No Pull up, Pull down
	GPIO_otype(PA_9, PSPL);		 		// Push-Pull	
	
	GPIO_init(PA_8, OUTPUT);   		// calls RCC_GPIOA_enable() / OUTPUT
	GPIO_ospeed(PA_8, MSPEED);		// Medium-Speed
	GPIO_pupd(PA_8, EC_NP);				// No Pull up, Pull down
	GPIO_otype(PA_8, PSPL);		 		// Push-Pull
	/* We are not gonna use dp for point
	GPIO_init(PB_10, OUTPUT);   		// calls RCC_GPIOB_enable() / OUTPUT
	GPIO_ospeed(PB_10, MSPEED);		// Medium-Speed
	GPIO_pupd(PB_10, EC_NP);				// No Pull up, Pull down
	GPIO_otype(PB_10, PSPL);		 		// Push-Pull	
	*/
}

// Displaying "0"-"9" in 7-segment display by Code that play role as a decoder
void sevensegment_decoder(uint8_t num){
	// initialization
	// a~g for 7-seg Display
	int dispA = 1;
	int dispB = 1;
	int dispC = 1;
	int dispD = 1;
	int dispE = 1;
	int dispF = 1;
	int dispG = 1;
	int Goto[] = {63, 6, 91, 79, 102, 109, 124, 7, 127, 103};						// Navigation for Displaying Number of 7-segment display.

	for(int i=1;i<=Goto[num];i++){
		// Up counting in all possible 7-Segment displaying
								dispA = !dispA;					// Least Significant Bit (LSB)
		if(i%2==0)	dispB = !dispB;					// Bit 1
		if(i%4==0) 	dispC = !dispC;					// Bit 2
		if(i%8==0) 	dispD = !dispD;					// Bit 3
		if(i%16==0) dispE = !dispE;					// Bit 4
		if(i%32==0) dispF = !dispF;					// Bit 5
		if(i%64==0) dispG = !dispG;					// Most Significant Bit (MSB)
	}
	
	GPIO_write(PA_5,dispA);
	GPIO_write(PA_6,dispB);
	GPIO_write(PA_7,dispC);
	GPIO_write(PB_6,dispD);
	GPIO_write(PC_7,dispE);
	GPIO_write(PA_9,dispF);
	GPIO_write(PA_8,dispG);
}