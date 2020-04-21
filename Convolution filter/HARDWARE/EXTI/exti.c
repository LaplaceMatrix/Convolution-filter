#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//�ⲿ�ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//�ⲿ�жϳ�ʼ��
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //����GPIOAʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_0;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    //�ж���0
	
    HAL_NVIC_SetPriority(EXTI0_IRQn,1,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0
		
		__HAL_RCC_GPIOI_CLK_ENABLE();
		GPIO_Initure.Pin=GPIO_PIN_8;                //PA0
		GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
}


//�жϷ�����
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //�����жϴ����ú���
}

//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
u32 EXTI_CNT = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	 //�������˴�Ϊ�˷���ʹ������ʱ������ʵ�ʴ����н�ֹ���жϷ������е����κ�delay֮�����ʱ����������
    //delay_ms(20);     
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(WK_UP==1)	//����LED0,LED1�������
            {
							EXTI_CNT++;
            }
            break;
				default:;
    }
}

int32_t GET_EXTI_CNT(void)
{
	int32_t buff = EXTI_CNT;
	EXTI_CNT = 0;
	if(HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_8)==0)buff = -buff;
	return buff;
}


