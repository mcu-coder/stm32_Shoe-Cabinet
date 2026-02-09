#ifndef __PM25_H__
#define __PM25_H__


/**********************************
包含头文件
**********************************/
#include "sys.h"


/**********************************
重定义关键词
**********************************/
#define PM25_LED_GPIO_CLK_ENABLE             	RCC_APB2Periph_GPIOB				//PM2.5的LED引脚
#define PM25_LED_PORT                         GPIOB
#define PM25_LED_PIN                          GPIO_Pin_13
#define PM25_LED 															PBout(13)


/**********************************
函数声明
**********************************/
void PM25_Init(void);  															//PM2.5初始化函数
uint16_t Pm25_Get_Value(uint8_t times);							//PM2.5读取函数


#endif

