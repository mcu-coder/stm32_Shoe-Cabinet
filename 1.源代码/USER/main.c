#include "sys.h"
#include "stdio.h"
#include "usart.h"
#include "stdlib.h"
#include "math.h"
#include "delay.h"
#include "gpio.h"
#include "key.h"
#include "oled.h"
#include "usart.h"
#include "dht11.h"
#include "adc.h"
#include "pm25.h"


/**********************************
变量定义
**********************************/
uint8_t key_num = 0;									//按键扫描标志位
uint8_t flag_display = 0;							//显示界面标志位
uint32_t time_num = 0;								//10ms计时
char display_buf[32];									//显示缓存区

_Bool flag_mode = 0;                  //模式标志位
int jr_flag = 0;
int xd_flag = 0;
int tf_flag = 0;
int zm_flag = 0;

u16 humi_value = 0;										//鞋柜湿度值
u16 humi_max = 70;										//鞋柜湿度最大值
u16 temp_value = 0;										//鞋柜温度值
u16 temp_min = 10;							      //最小温度变量

u16 light_value = 0;									//光照值
u16 light_min = 50;									  //光照最小值

u16 pm25_value = 0;                   //粉尘值
u16 pm25_max = 250;                   //粉尘最大值

u16 nh_value = 0;                  	 	//氨气值
u16 nh_max = 80;                  	  //氨气最大值

extern uint8_t usart1_buf[256];				//串口1接收数组


/**********************************
函数声明
**********************************/
void Key_function(void);							//按键函数
void Monitor_function(void);					//监测函数
void Display_function(void);					//显示函数
void Manage_function(void);						//处理函数


/****
*******	主函数 
*****/
int main(void)
{
	Delay_Init();	    	 								//延时初始化	  
	Gpio_Init();		  									//IO初始化
	Key_Init();		  										//按键初始化
	Oled_Init();		  									//OLED初始化
	Oled_Clear_All();										//清屏
	Usart1_Init(9600);  								//串口1初始化
	Adc_Init();													//ADC初始化
	 
	while(1)
	{
		Key_function();										//按键函数
		Monitor_function();								//监测函数
		Display_function();								//显示函数
		Manage_function();								//处理函数

		time_num++;												//计时变量+1
		Delay_ms(10);
    if(time_num %10 == 0)
      LED_SYS = ~LED_SYS;
		if(time_num >= 5000)
		{
			time_num = 0;
		}
	}
}

/****
*******按键函数
*****/
void Key_function(void)
{
	key_num = Chiclet_Keyboard_Scan(0);					//按键扫描
	if(key_num != 0)														//有按键按下
	{
		switch(key_num)
		{
			case 1:																	//按键1：切换界面
				flag_display++;																			
        if(flag_display >= 6)
          flag_display = 0;
				Oled_Clear_All();											//清屏
			break;

			 

			default:
				break;
		}
	}
}

/****
*******监测函数
*****/
void Monitor_function(void)
{
	if(flag_display == 0)																			//测量界面
	{
		if(time_num % 10 == 0)
		{
			Dht11_Get_Temp_Humi_Value(&temp_value,&humi_value);		//获取温湿度值
			light_value = 100-30*(Get_Adc_Average(0,3)*3.3/4096.0);		//获取光照强度

			pm25_value = Pm25_Get_Value(3);												//获取PM2.5
			
			if(NH == 0)
				nh_value = 90*(Get_Adc_Average(4,3)*3.3/4096.0);		//获取氨气值
			else
				nh_value = 0;

			if(time_num % 30 == 0)																//发送测量数据
			{
				UsartPrintf(USART1,"\r\n温度:%d.%dC  湿度:%d.%d%%",temp_value/10,temp_value%10,humi_value/10,humi_value%10);
				UsartPrintf(USART1,"\r\n粉尘:%d  空气质量:%d",pm25_value,nh_value);
				UsartPrintf(USART1,"\r\n光照:%d  ",light_value);
				
				if(DOOR == 1)
					UsartPrintf(USART1,"    柜门:关");
				else
					UsartPrintf(USART1,"   	柜门:开");
				
				if(flag_mode == 0)
					UsartPrintf(USART1,"\r\n模式:自动");
				else
					UsartPrintf(USART1,"\r\n模式:手动");
			}
		}
		 
	}
}

/****
*******显示函数
*****/
void Display_function(void)
{
	switch(flag_display)																		//根据不同的显示模式标志位，显示不同的界面
	{
		case 0:																								//界面0：测量界面，显示测量值及模式
			Oled_ShowCHinese(1,0,"温");
			sprintf(display_buf,":%d.%dC ",temp_value/10,temp_value%10);
			Oled_ShowString(1, 2, display_buf);
		 
		
		  Oled_ShowCHinese(3,0,"光");
			sprintf(display_buf,":%d  ",light_value);
			Oled_ShowString(3, 2, display_buf);

			Oled_ShowCHinese(3,4,"柜门");
			Oled_ShowString(3, 12, ":");
			 
			
			if(flag_mode == 1)
				Oled_ShowCHinese(4,2,"手动模式");
			else
				Oled_ShowCHinese(4,2,"自动模式");

		break;
 

		default:
		break;
	}
}

/****
*******处理函数
*****/
void Manage_function(void)
{
	if(flag_display == 0)							//测量界面
	{
		if(flag_mode == 0)
		{
			if(DOOR == 1)        					//如果鞋柜门是关闭的，关照明灯，如果氨气超高，开消毒灯
      {
        RELAY_ZM = 0;
				if(nh_max < nh_value)
				{
					RELAY_XD = 1;
				}
        else
				{
					RELAY_XD = 0;
				}
      } 
			 
		}
	}
	else															//设置界面，关闭所有继电器
	{
		RELAY_ZM = 0;
    RELAY_TF = 0; 
    RELAY_XD = 0;
	}
}

