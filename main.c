#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
int main(void)  
	{
		u16 led0pwmval=0;
		u8 dir=1;
		delay_init();       //��ʱ������ʼ��
		LED_Init();     //LED�˿ڳ�ʼ��
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(115200);		
		TIM3_PWM_Init(899,0);//����Ƶ PWMƵ��=72000/(899+1)=80Khz
		while(1)
		{
				delay_ms(10);
				if(dir)led0pwmval++;
				else led0pwmval--;
				if(led0pwmval>100)dir=0;
				if(led0pwmval==0)dir=1;
				TIM_SetCompare2(TIM3,led0pwmval);
		}
  } 