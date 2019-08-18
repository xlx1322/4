#include "timer.h"
#include "led.h"

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //自动重装寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM3向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断，允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //中断TIM3					 
}
//TIM3中断服务
void TIM3_IRQHandler(void)   //TIM3
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //是否发生更新中断
		{ 
		LED1=!LED1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除更新中断标志 
		}
}

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStrue;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //TIM3部分重映射  TIM3->PB5
	//设置该引脚为复用输出，TIM_CH2的PWM输出波形为GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	 //TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //自动重装寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频种�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM3向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//???TIM3 Channel2 PWM??	 
	TIM_OCInitStrue.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式2
 	TIM_OCInitStrue.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStrue.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性为高 
	

	TIM_OC2Init(TIM3, &TIM_OCInitStrue);  
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能TIM3在CCR2上的预装载寄存器
	
	
	TIM_Cmd(TIM3, ENABLE);  //使能 TIM3
	
}
