/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : SSS LAB
Created          : 2024-09-10
Modified         : 2024-09-18
Language/ver     : C++ in Keil uVision

Description      : Distributed to Students for LAB_GPIO
/----------------------------------------------------------------*/


#ifndef __ECGPIO2_H
#define __ECGPIO2_H

#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecPinNames.h"

//(GPIOx_MODER) GPIO port mode register
#define INPUT  0x00
#define OUTPUT 0x01 
#define AF     0x02		//Alternate function
#define ANALOG 0x03   //Analog
//(GPIOx_OTYPER) GPIO port output type register 
#define PSPL 	 0x0		//Push-Pull
#define OPDR	 0x1		//Open Drain
//(GPIOx_OSPEEDR) GPIO port output speed register
#define LSPEED 0x00		//Low speed
#define MSPEED 0x01		//Medium speed
#define FSPEED 0x02		//Fast speed
#define HSPEED 0x03		//High speed
//(GPIOx_PUPDR) GPIO port pull-up/pull-down register
#define EC_NP  0x00		//No pull-up, pull down
#define EC_PU  0x01		//Pull-up
#define EC_PD	 0x02		//Pull-down
#define EC_RE  0x03		//Reserved

#define HIGH 1
#define LOW  0

#define LED_PIN    //Find LED Port&Pin and Fill the blank	
#define BUTTON_PIN //Find BTN Port&Pin and Fill the blank

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
void GPIO_init(PinName_t pinName, uint32_t mode);     
void GPIO_write(PinName_t pinName, int Output);
int  GPIO_read(PinName_t pinName);
void GPIO_mode(PinName_t pinName, uint32_t mode);
void GPIO_ospeed(PinName_t pinName, int speed);
void GPIO_otype(PinName_t pinName, int type);
void GPIO_pupd(PinName_t pinName, int pupd);


 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __ECGPIO2_H
