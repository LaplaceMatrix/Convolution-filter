#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//外部中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/11/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//外部中断初始化
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOA时钟
    
    GPIO_Initure.Pin=GPIO_PIN_0;                //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;			//下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    //中断线0
	
    HAL_NVIC_SetPriority(EXTI0_IRQn,1,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0
		
		__HAL_RCC_GPIOI_CLK_ENABLE();
		GPIO_Initure.Pin=GPIO_PIN_8;                //PA0
		GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
}


//中断服务函数
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);   //调用中断处理公用函数
}

//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
u32 EXTI_CNT = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	 //消抖，此处为了方便使用了延时函数，实际代码中禁止在中断服务函数中调用任何delay之类的延时函数！！！
    //delay_ms(20);     
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(WK_UP==1)	//控制LED0,LED1互斥点亮
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


