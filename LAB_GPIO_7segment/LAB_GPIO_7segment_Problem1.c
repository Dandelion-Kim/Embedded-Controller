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
		sevensegment_display(cnt % 10); 						// Give the argument "cnt%10" to the fucntion 
		if(GPIO_read(BUTTON_PIN) == 0) cnt++; 
        if (cnt > 9) cnt = 0;
		for(int i = 0; i < 500000;i++){} 				 		// delay_ms(500);
	}
}

// Initialiization 
void setup(void){
	RCC_HSI_init();	
	GPIO_init(BUTTON_PIN, INPUT);  														// calls RCC_GPIOC_enable()
	sevensegment_display_init(PA_7, PB_6, PC_7, PA_9);  			// Decoder input A,B,C,D
}

// Initialiization for 7-segment_display function
void sevensegment_display_init(PinName_t pinNameA, PinName_t pinNameB, PinName_t pinNameC, PinName_t pinNameD){
	// Setup for the A,B,C,D for 7-Segment Decoder
	// PA_7
	GPIO_init(pinNameA, OUTPUT);   		// calls RCC_GPIOA_enable() / OUTPUT
	GPIO_ospeed(pinNameA, MSPEED);		// Medium-Speed
	GPIO_pupd(pinNameA, EC_NP);				// No Pull up, Pull down
	GPIO_otype(pinNameA, PSPL);		 		// Push-Pull		
	// PB_6
	GPIO_init(pinNameB, OUTPUT);   		// calls RCC_GPIOB_enable() / OUTPUT
	GPIO_ospeed(pinNameB, MSPEED);		// Medium-Speed
	GPIO_pupd(pinNameB, EC_NP);				// No Pull up, Pull down
	GPIO_otype(pinNameB, PSPL);		 		// Push-Pull	
	// PC_7
	GPIO_init(pinNameC, OUTPUT);   		// calls RCC_GPIOC_enable() / OUTPUT
	GPIO_ospeed(pinNameC, MSPEED);		// Medium-Speed
	GPIO_pupd(pinNameC, EC_NP);				// No Pull up, Pull down
	GPIO_otype(pinNameC, PSPL);		 		// Push-Pull	
	// PA_9
	GPIO_init(pinNameD, OUTPUT);   		// calls RCC_GPIOA_enable() / OUTPUT
	GPIO_ospeed(pinNameD, MSPEED);		// Medium-Speed
	GPIO_pupd(pinNameD, EC_NP);				// No Pull up, Pull down
	GPIO_otype(pinNameD, PSPL);		 		// Push-Pull	
}

// Displaying the "0"-"9" in 7-segment display by Decoder A,B,C,D
void sevensegment_display(uint8_t num){			// num: Number for diplaying
	int BCD_A= 0;															// PA_7 - A (LSB: Least Siginificant Bit)
	int BCD_B= 0;															// PB_6 - B
	int BCD_C= 0;															// PC_7 - C
	int BCD_D= 0;															// PA_9 - D (MSB: Least Siginificant Bit)
		
	for(int i=1;i<=num;i++){
						   BCD_A = !BCD_A;							// Toggle every iteration.
		if(i%2==0) BCD_B = !BCD_B;							// Toggle at the even number
		if(i%4==0) BCD_C = !BCD_C;							// Toggle every 4th time.
		if(i%8==0) BCD_D = !BCD_D;							// Toggle every 8th time.
	}
	
	GPIO_write(PA_7, BCD_A);							
	GPIO_write(PB_6, BCD_B);
	GPIO_write(PC_7, BCD_C);
	GPIO_write(PA_9, BCD_D);
}

