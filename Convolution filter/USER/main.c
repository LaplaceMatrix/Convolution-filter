#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "timer.h"
#include "sdram.h"
#include "math.h"
#include "fft.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验49_2
 DSP FFT实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


float fft_inputbuf[NPT*2];	//FFT输入数组
float fft_outputbuf[NPT];	//FFT输出数组

u8 timeout;


struct compx s[NPT];
extern struct compx fir_s[NPT];
//extern float fft_out_table[FFT_N];
extern float my_sin_tab[NPT/4+1];
int main(void)
{
 	u8 key=0;
	float time; 
	u8 buf[50]; 
	u16 i; 
    int t;
    HAL_Init();                     //初始化HAL库   
		Stm32_Clock_Init(160,5,2,4);  		//设置时钟,400Mhz 
		Cache_Enable();                			//打开L1-Cache
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART	
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
    LCD_Init();                     //初始化LCD
    TIM3_Init(5000-1,20000-1);          //10Khz计数频率,最大计时65ms秒超出
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"CREATED BY");	
	LCD_ShowString(30,70,200,16,16,"DSP FFT TEST");	
	LCD_ShowString(30,90,200,16,16,"ROCKET");
	LCD_ShowString(30,110,200,16,16,"2018/1/17");	
	LCD_ShowString(30,130,200,16,16,"KEY0:Run FFT");//显示提示信息 
	LCD_ShowString(30,160,200,16,16,"DSP_FFT runtime:");//显示FFT执行时间
	LCD_ShowString(30,176,200,16,16,"C_FFT runtime:");//显示FFT执行时间
 	POINT_COLOR=BLUE;		//设置字体为蓝色   
    while(1)
	{
		
			//FFT_TEST();//arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT计算（基4）
			//printf("FFT Result:\r\n");
			for(i=0;i<NPT;i++)
			{
				//printf("fft_outputbuf[%d]:%f\r\n",i,s[i].real);
			}
			//FFT_draw();
		
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			
			
	create_sin_tab(my_sin_tab,NPT);					//生成正弦表加快FFT速度，该表只需要一次性生成
	//生成测试信号，该信号由两个正弦信号和一个直流信号合成
	for(t=0;t<NPT;t++)                           	//给结构体赋值
	{
		s[t].real=fft_signel(t); 	//实部为正弦波FFT_N点采样，赋值为1
		s[t].imag=0;                                //虚部为0
	}
			__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//重设TIM3定时器的计数器值
			timeout=0;
			fft_test();
      time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)timeout*65536;//计算所用时间
			sprintf((char*)buf,"%0.3fms\r\n",time/1000);	
			LCD_ShowString(46+12*8,176,100,16,16,buf);	//显示运行时间	
			printf("\r\n%d point FFT runtime:%0.3fms\r\n",NPT,time/1000);	
			printf("C_FFT Result:\r\n");
			for(i=0;i<NPT;i++)
			{
					if((i>0)&&(i%8==0))printf("\r\n");
					printf("%8.5f ",s[i].real);
			}
			
			
			printf("\r\n");
			filter_test();
			printf("FIR Result:\r\n");
			for(i=0;i<NPT;i++)
			{
					if((i>0)&&(i%8==0))printf("\r\n");
					printf("%8.5f ",fir_s[i].real);
			}
			
			for(i=0;i<NPT;i++)
			{
				Data_Send_Senser(fft_outputbuf[i],s[i].real,fir_s[i].real);//usart1_report_imu(fft_outputbuf[i],0,0,0,0,0);
				//delay_ms(10);
			}			
		}else delay_ms(10);
		t++;
		if((t%10)==0)LED0_Toggle;                	//LED0翻转		  
		
		
		
	}
}

