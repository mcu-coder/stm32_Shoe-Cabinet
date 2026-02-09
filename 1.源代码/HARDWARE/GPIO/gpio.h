#ifndef __GPIO_H
#define __GPIO_H


/**********************************
包含头文件
**********************************/
#include "sys.h"


/**********************************
重定义关键词
**********************************/
#define LEDS_GPIO_CLK                 RCC_APB2Periph_GPIOC        //最小系统LED
#define LEDS_GPIO_PORT                GPIOC
#define LEDS_GPIO_PIN                 GPIO_Pin_13
#define LED_SYS                       PCout(13)

#define RELAY_JR_GPIO_CLK             RCC_APB2Periph_GPIOA				//蜂鸣器引脚
#define RELAY_JR_PORT                 GPIOA
#define RELAY_JR_PIN                  GPIO_Pin_5
#define RELAY_JR 									    PAout(5)
#define RELAY_TF_GPIO_CLK             RCC_APB2Periph_GPIOA				//蜂鸣器引脚
#define RELAY_TF_PORT                 GPIOA
#define RELAY_TF_PIN                  GPIO_Pin_6
#define RELAY_TF 									    PAout(6)

#define RELAY_XD_GPIO_CLK             RCC_APB2Periph_GPIOA				//蜂鸣器引脚
#define RELAY_XD_PORT                 GPIOA
#define RELAY_XD_PIN                  GPIO_Pin_7
#define RELAY_XD 									    PAout(7)

#define RELAY_ZM_GPIO_CLK             RCC_APB2Periph_GPIOA				//蜂鸣器引脚
#define RELAY_ZM_PORT                 GPIOA
#define RELAY_ZM_PIN                  GPIO_Pin_8
#define RELAY_ZM 									    PAout(8)

#define DOOR_GPIO_CLK                 RCC_APB2Periph_GPIOB			
#define DOOR_PORT                     GPIOB
#define DOOR_PIN                      GPIO_Pin_15
#define DOOR 													PBin(15)

#define NH_GPIO_CLK                   RCC_APB2Periph_GPIOB			
#define NH_PORT                       GPIOB
#define NH_PIN                        GPIO_Pin_14
#define NH 													  PBin(14)


/**********************************
函数声明
**********************************/
void Gpio_Init(void);													//GPIO初始化


#endif
