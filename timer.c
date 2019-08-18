#include "timer.h"
#include "led.h"

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //Ê±ÖÓÊ¹ÄÜ
	
	//TIM3³õÊ¼»¯
	TIM_TimeBaseStructure.TIM_Period = arr; //×Ô¶¯ÖØ×°¼Ä´æÆ÷ÖÜÆÚµÄÖµ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ô¤·ÖÆµÖÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM3ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //??????????TIMx???????
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //Ê¹ÄÜÖ¸¶¨µÄTIM3ÖĞ¶Ï£¬ÔÊĞí¸üĞÂÖĞ¶Ï

	//ÖĞ¶ÏÓÅÏÈ¼¶NVICÉèÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕ¼ÓÅÏÈ¼¶0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //´ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //³õÊ¼»¯NVIC¼Ä´æÆ÷


	TIM_Cmd(TIM3, ENABLE);  //ÖĞ¶ÏTIM3					 
}
//TIM3ÖĞ¶Ï·şÎñ
void TIM3_IRQHandler(void)   //TIM3??
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //ÊÇ·ñ·¢Éú¸üĞÂÖĞ¶Ï
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //Çå³ı¸üĞÂÖĞ¶Ï±êÖ¾ 
		LED1=!LED1;
		}
}

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStrue;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//Ê¹ÄÜTIM3
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //Ê¹ÄÜGPIOÍâÉèºÍAFIO¸´ÓÃ¹¦ÄÜÄ£¿éÊ±ÖÓ
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //TIM3²¿·ÖÖØÓ³Éä  TIM3->PB5
	//ÉèÖÃ¸ÃÒı½ÅÎª¸´ÓÃÊä³ö£¬TIM_CH2µÄPWMÊä³ö²¨ĞÎÎªGPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯GPIO
	 //TIM3³õÊ¼»¯
	TIM_TimeBaseStructure.TIM_Period = arr; //×Ô¶¯ÖØ×°¼Ä´æÆ÷ÖÜÆÚµÄÖµ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ô¤·ÖÆµÖÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM3ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//???TIM3 Channel2 PWM??	 
	TIM_OCInitStrue.TIM_OCMode = TIM_OCMode_PWM2; //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½2
 	TIM_OCInitStrue.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStrue.TIM_OCPolarity = TIM_OCPolarity_High; //Êä³ö¼«ĞÔÎª¸ß 
	

	TIM_OC2Init(TIM3, &TIM_OCInitStrue);  
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //Ê¹ÄÜTIM3ÔÚCCR2ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
	
	
	TIM_Cmd(TIM3, ENABLE);  //Ê¹ÄÜ TIM3
	
}
