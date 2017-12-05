#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dma.h"
#include "adc.h"
#include "math.h"
#include "string.h"
#include "can.h"
#include "wdg.h"
float adc_x,adc_y,adc_z;
void yaokong_fenxi(void);
float flagX,flagY,flagZ;
u16 anjian_temp;
int main(void)
{	
	u8 i=0;	
	delay_init();	
	NVIC_Configuration();
	uart1_init(115200);
	uart2_init(115200);
	DMA_USART1_Configuration();
	DMA_USART2_Configuration();
	DMA_ADC_Configuration();
	ADC1_Configuration();
	Init_LEDpin();
	IWDG_Init(4,625);    //与分频数为64,重载值为625,溢出时间为1s
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,0);
	yaokong_fenxi();
	cansend[3] = 0;//清除复位标志
	while(1)
	{
		yaokong_fenxi();			
		i++;
		if(i==5)
		{
			IWDG_Feed();
			LED_CPU = ~LED_CPU;
			i=0;
		}
		delay_ms(100);
	}
}
//Uart1_Send[0]	方向
//Uart1_Send[1]	速度
//Uart1_Send[2]	行走/升降
//Uart1_Send[3]	直行/斜行
//Uart1_Send[4]	下降
//Uart1_Send[5]	急停
//Uart1_Send[6]	快速/慢速
//Uart1_Send[7]	起升

void yaokong_fenxi()
{
	char x[100],y[100],z[100];
	float max=145.0,min=105.0;
	adc_z = After_filter[0]*3.3/4096*100;//Z	
	adc_y = After_filter[1]*3.3/4096*100;//Y
	adc_x = After_filter[2]*3.3/4096*100;//X
	Uart1_Send[2] = XingZou_OR_ShengJiang|(ZhiXing_OR_XieXing<<1)|(XiaJiang<<2)|(JiTing<<3)
					|(KuaiSu_OR_ManSu<<4)|(QiSheng<<5)|(YaoGan_Key<<6)|(YaoKong_OR_KongZhiHe<<7);
	sprintf(x,"x:%3.f",adc_x);
	sprintf(y,"y:%3.f",adc_y);
	sprintf(z,"z:%3.f",adc_z);
	if(adc_x!=flagX||adc_y!=flagY||adc_z!=flagZ||Uart1_Send[2]!=anjian_temp)
	{
		flagX = adc_x;
		flagY = adc_y;
		flagZ = adc_z;
		anjian_temp = Uart1_Send[2];
		//左上
		if(flagX <min && flagY > max)
		{
			 Uart1_Send[0] = 5;
			 Uart1_Send[1] = (flagY-max)/(197.0-max)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		}//右上
		else if(flagX >130 && flagY > max)
		{
			 Uart1_Send[0] = 6;
			 Uart1_Send[1] = (flagY-max)/(188.0-max)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		}//左下
		else if(flagX <min && flagY < min)
		{
			 Uart1_Send[0] = 7;
			 Uart1_Send[1] = (min-flagY)/(min-55.0)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		}//右下
		else if(flagX >max && flagY < min)
		{
			 Uart1_Send[0] = 8;
			 Uart1_Send[1] = (min-flagY)/(min-55.0)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		}//左移
		else if(flagX < min )
		{
			 Uart1_Send[0] = 3;
			 Uart1_Send[1] = (min-flagX)/(min-27.0)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		 }//右移
		else if(flagX > max )
		{
			 Uart1_Send[0] = 4;
			 Uart1_Send[1] = (flagX-max)/(223-max)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;

		}//前进
		else if(flagY > max)
		{
			 Uart1_Send[0] = 1;
			 Uart1_Send[1] = (flagY-max)/(197-max)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		}//后退
		else if(flagY < min)
		{
			 Uart1_Send[0] = 2;
			 Uart1_Send[1] = (min-flagY)/(min-25)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		}//左旋
		else if(flagZ < min)
		{
			 Uart1_Send[0] = 9;
			 Uart1_Send[1] = (min-flagZ)/(min-25)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		}//右旋
		else if(flagZ > max)
		{
			 Uart1_Send[0] = 10;
			 Uart1_Send[1] = (flagZ-max)/(225-max)*100;
			 Uart1_Send[1] = (Uart1_Send[1]<=100) ? Uart1_Send[1]:100;
		}//停止
		else if((flagX >min && flagX < max)||(flagY >min && flagY < max)||(flagZ >min && flagZ < max))
		{
			 Uart1_Send[0] = 0;
			 Uart1_Send[1]  = 0;
		}
		send();		
  }
}

