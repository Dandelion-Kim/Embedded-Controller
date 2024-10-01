/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Jong-Hyeon Kim
Created          : 2024-09-27
Modified         : 2024-10-01
Language/ver     : C++ in Keil uVision

Description      : Source file for SysTick
/----------------------------------------------------------------*/

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
