/**
******************************************************************************
* @author	Jong-Hyeon Kim
* @Mod		2024-09-18
* @brief	Embedded Controller:  LAB Digital In/Out
*					 - Toggle LED LD2 by Button B1 pressing
* 
******************************************************************************
*/

#include "stm32f4xx.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"

PinName_t LED_pin = PA_5;										// GPIOA Pin5
PinName_t button_pin = PC_13;								// GPIOC Pin13

void setup(void);
	
int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	int output = HIGH;
	// Inifinite Loop ----------------------------------------------------------
	while(1){
			if(GPIO_read(button_pin) == 0) 				// If the button(B1) is pushed
			{
				GPIO_write(LED_pin, output);				// Turn the LED(LD2) on
				output = ~output;										// Toggling
				while(GPIO_read(button_pin) == 0);  	// Examine if the button(B1) released.
			}
		}
	}


// Initialiization 
void setup(void)
{
	RCC_HSI_init();	
	// Setup for the button pin
	GPIO_init(button_pin, INPUT);  	// calls RCC_GPIOC_enable()
	GPIO_pupd(button_pin, EC_PU);	 	// Pull-Up
	
	// Setup for the LED
	GPIO_init(LED_pin, OUTPUT);   	// calls RCC_GPIOA_enable()
	GPIO_ospeed(LED_pin, MSPEED);		// Medium-Speed
	GPIO_pupd(LED_pin, EC_PU);			// Pull-Up
	GPIO_otype(LED_pin, OPDR);		 	// Open-Drain		
}

