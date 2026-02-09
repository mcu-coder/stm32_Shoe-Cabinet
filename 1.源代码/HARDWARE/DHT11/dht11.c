#include "dht11.h"
#include "delay.h"

/****
*******DHT11引脚输出配置
*****/
void DHT11_GPIO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK_ENABLE, ENABLE);	 	//使能端口时钟
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;				 					//引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 			//IO口速度为50MHz
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);					 			//根据设定参数初始化
}

/****
*******DHT11引脚输入配置
*****/
void DHT11_GPIO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK_ENABLE, ENABLE);	 	//使能端口时钟
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;				 					//引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 				//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 			//IO口速度为50MHz
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);					 			//根据设定参数初始化
}

/****
*******DHT11复位函数 
*****/
void DHT11_Rst(void)	   
{                 
	DHT11_GPIO_OUT(); 	//SET OUTPUT
	DHT11_OUT=0; 	//拉低DQ
	Delay_ms(20);    	//拉低至少18ms
	DHT11_OUT=1; 	//DQ=1 
	Delay_us(30);     	//主机拉高20~40us
}

/****
*******等待DHT11的回应函数
*******返回1:未检测到DHT11的存在
*******返回0:存在
*****/
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_GPIO_IN();												//SET INPUT	 
	while(DHT11_IN&&retry<100)							//DHT11会拉低40~80us
	{
		retry++;
		Delay_us(1);
	}

	if(retry>=100)
		return 1;
	else
		retry=0;

	while (!DHT11_IN&&retry<100)						//DHT11拉低后会再次拉高40~80us
	{
	retry++;
	Delay_us(1);
	}

	if(retry>=100)
		return 1;
  
	return 0;
}

/****
*******从DHT11读取一个位函数
*******返回值：1/0
*****/
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_IN&&retry<100)							//等待变为低电平
	{
		retry++;
		Delay_us(1);
	}
	retry=0;
	while(!DHT11_IN&&retry<100)							//等待变高电平
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40);														//等待40us
	if(DHT11_IN)
		return 1;
	else
		return 0;		   
}

/****
*******从DHT11读取一个字节函数
*******返回值：读到的数据
*****/
uint8_t DHT11_Read_Byte(void)    
{        
	uint8_t i,dat;
	dat=0;
	for(i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}

	return dat;
}

/****
*******从DHT11读取一次数据函数
*******参量定义：	temp:温度值(扩大10倍)
*******           humi:湿度值(扩大10倍)
*****/
void Dht11_Get_Temp_Humi_Value(uint16_t *temp,uint16_t *humi)
{
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)												//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0]*10+buf[1];								//获取湿度值
			*temp=buf[2]*10+buf[3];								//获取温度值
		}
	} 
}

/****
*******DHT11初始化函数
*******返回1:初始化失败
*******返回0:初始化成功
*****/
uint8_t DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK_ENABLE, ENABLE);	 //使能端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;				 					//引脚配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			//推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 			//IO口速度为50MHz
 	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);				 				//根据设定参数初始化
 	GPIO_SetBits(DHT11_PORT,DHT11_PIN);						 						//IO口输出高

	DHT11_Rst();  																						//复位DHT11
	return DHT11_Check();																			//等待DHT11的回应
}

