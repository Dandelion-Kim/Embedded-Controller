/**
******************************************************************************
* @author	Jong-Hyeon Kim
* @Mod		2024-09-16
* @brief	Embedded Controller:  LAB Digital In/Out
*					 - Toggle multiple LEDs by Button B1 pressing
* 
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"

PinName_t LED_pin0 = PA_5;									// GPIOA Pin5
PinName_t LED_pin1 = PA_6;									// GPIOA Pin6
PinName_t LED_pin2 = PA_7;									// GPIOA Pin7
PinName_t LED_pin3 = PB_6;									// GPIOB Pin6

PinName_t button_pin = PC_13;								// GPIOC Pin13

void setup(void);

int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	int count=0;
	int LED_ary[4]={LED_pin0, LED_pin1, LED_pin2, LED_pin3};		// Build a Toggling Range
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
			if((GPIO_read(button_pin) == 0)){
				
				if(count==0){													// Start with the LED_pin0
				GPIO_write(LED_ary[3],LOW);
				}
				else{
				GPIO_write(LED_ary[count-1],LOW);			// Turn off the previous LED
				}
				GPIO_write(LED_ary[count],HIGH);			// Turn on the present LED
				count++;															// Go to next count
				
				if(count==4) count=0;									// It is the last index. Go to first index of LED array.
				
				while(GPIO_read(button_pin)==0);			// Examine if the button(B1) released.
		}
	}
}


// Initialiization 
void setup(void)
{
	RCC_HSI_init();

	// Configuration button pin
	GPIO_init(button_pin, INPUT);  	// calls RCC_GPIOC_enable()
	GPIO_pupd(button_pin, EC_PU);		// Pull-Up

	// Configuration LEDs
	//PA_5
	GPIO_init(LED_pin0, OUTPUT);   	// calls RCC_GPIOA_enable()	
	GPIO_otype(LED_pin0, PSPL);			// Push-Pull
	GPIO_ospeed(LED_pin0, MSPEED);	// Medium-Speed
	GPIO_pupd(LED_pin0, EC_PU);			// Pull-Up
	
	//PA_6
	GPIO_init(LED_pin1, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_otype(LED_pin1, PSPL);			// Push-Pull
	GPIO_ospeed(LED_pin1, MSPEED);	// Medium-Speed
	GPIO_pupd(LED_pin1, EC_PU);			// Pull-Up
	
	//PA_7
	GPIO_init(LED_pin2, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_otype(LED_pin2, PSPL);			// Push-Pull
	GPIO_ospeed(LED_pin2, MSPEED);	// Medium-Speed
	GPIO_pupd(LED_pin2, EC_PU);			// Pull-Up
	
	//PB_6
	GPIO_init(LED_pin3, OUTPUT);    // calls RCC_GPIOA_enable()	
	GPIO_otype(LED_pin3, PSPL);			// Push-Pull
	GPIO_ospeed(LED_pin3, MSPEED);	// Medium-Speed
	GPIO_pupd(LED_pin3, EC_PU);			// Pull-Up
}