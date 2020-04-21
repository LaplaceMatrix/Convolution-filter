#include "usart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2017/6/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
//#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART3->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART3->TDR=(u8)ch;      
	return ch;
}
#endif 

#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART3_Handler; //UART句柄

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound)
{	
	//UART 初始化设置
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()会使能UART3
	
	HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART3)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART3
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_11;			//PA10
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PA10
		
#if EN_USART3_RX
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//使能USART3中断通道
		HAL_NVIC_SetPriority(USART3_IRQn,0,1);			//抢占优先级3，子优先级3
#endif	
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}

	}
}
 
//串口1中断服务程序
void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART3_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART3_Handler)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART3_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 
#endif	

/*下面代码我们直接把中断控制逻辑写在中断服务函数内部。*/
/*

//串口1中断服务程序
void USART3_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
        HAL_UART_Receive(&UART3_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART3_Handler);	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 
#endif	
*/

/**************************************/
//通过串口1上报结算后的姿态数据给电脑(状态帧)
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
//csb:超声波高度,单位:cm
//prs:??,单位:mm
void usart3_report_imu(int roll,int pitch,int yaw,u32 height,s8 fly_mode,u8 armed)
{
	u8 tbuf[12];   	
	tbuf[0]=(roll>>8)&0XFF;
	tbuf[1]=roll&0XFF;
	tbuf[2]=(pitch>>8)&0XFF;
	tbuf[3]=pitch&0XFF;
	tbuf[4]=(yaw>>8)&0XFF;
	tbuf[5]=yaw&0XFF;
	tbuf[6]=(height>>24)&0XFF;
	tbuf[7]=(height>>16)&0XFF;
	tbuf[8]=(height>>8)&0XFF;
	tbuf[9]=height&0XFF;
	tbuf[10]=fly_mode&0XFF;
	tbuf[11]=armed&0XFF;
	usart3_niming_report(0X01,tbuf,12);//状态帧,0X01
} 
void report_dspfft(int16_t value,int16_t buff0,int16_t buff1)
{
	u8 tbuf[6];  
	u8 sum = 0;int i=0;
	tbuf[0]=(value>>8)&0XFF;
	tbuf[1]=value&0XFF;
	tbuf[2]=(buff0>>8)&0XFF;
	tbuf[3]=buff0&0XFF;
	tbuf[4]=(buff1>>8)&0XFF;
	tbuf[5]=buff1&0XFF;
	usart3_send_char(0XAA);
	usart3_send_char(0XAA);
	usart3_send_char(0XF1);
	usart3_send_char(0X06);
	for(i=0;i<6;i++)
	{
		usart3_send_char(tbuf[i]);
		sum+=tbuf[i];
	}
	sum+=0XAA;
	sum+=0XAA;
	sum+=6;
	usart3_send_char(sum&0XFF);
}
void Data_Send_Senser(int16_t value,int16_t value2,int16_t value3)
{
	u8 data_to_send[32];
	u8 _cnt=0;
	u8 sum = 0;u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=(value>>8)&0XFF;
	data_to_send[_cnt++]=value&0XFF;
	data_to_send[_cnt++]=(value2>>8)&0XFF;//BYTE1(Moto_PWM_2);
	data_to_send[_cnt++]=value2&0XFF;
	data_to_send[_cnt++]=(value3>>8)&0XFF;
	data_to_send[_cnt++]=value3&0XFF;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	for(i=0;i<_cnt;i++)usart3_send_char(data_to_send[i]);
	//usart1_send_char(data_to_send,_cnt);
}

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0X01~0X1C
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart3_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0XAA;	//帧头
	send_buf[1]=0XAA;	//帧头
	send_buf[2]=fun;	//功能字
	send_buf[3]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//复制数据
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+5;i++)usart3_send_char(send_buf[i]);	//发送数据到串口1 
}

//串口1发送1个字符 
//c:要发送的字符
void usart3_send_char(u8 c)
{
//	while((USART3->SR&0X40)==0);	//循环发送,直到发送完毕   
//    USART3->DR=c;  
				USART3->TDR=c;
				while((USART3->ISR&0X40)==0);//等待发送结束
} 


 




