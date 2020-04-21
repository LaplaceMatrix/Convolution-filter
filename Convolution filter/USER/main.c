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
 ALIENTEK ������STM32F429������ʵ��49_2
 DSP FFTʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


float fft_inputbuf[NPT*2];	//FFT��������
float fft_outputbuf[NPT];	//FFT�������

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
    HAL_Init();                     //��ʼ��HAL��   
		Stm32_Clock_Init(160,5,2,4);  		//����ʱ��,400Mhz 
		Cache_Enable();                			//��L1-Cache
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART	
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    LCD_Init();                     //��ʼ��LCD
    TIM3_Init(5000-1,20000-1);          //10Khz����Ƶ��,����ʱ65ms�볬��
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"CREATED BY");	
	LCD_ShowString(30,70,200,16,16,"DSP FFT TEST");	
	LCD_ShowString(30,90,200,16,16,"ROCKET");
	LCD_ShowString(30,110,200,16,16,"2018/1/17");	
	LCD_ShowString(30,130,200,16,16,"KEY0:Run FFT");//��ʾ��ʾ��Ϣ 
	LCD_ShowString(30,160,200,16,16,"DSP_FFT runtime:");//��ʾFFTִ��ʱ��
	LCD_ShowString(30,176,200,16,16,"C_FFT runtime:");//��ʾFFTִ��ʱ��
 	POINT_COLOR=BLUE;		//��������Ϊ��ɫ   
    while(1)
	{
		
			//FFT_TEST();//arm_cfft_radix4_f32(&scfft,fft_inputbuf);	//FFT���㣨��4��
			//printf("FFT Result:\r\n");
			for(i=0;i<NPT;i++)
			{
				//printf("fft_outputbuf[%d]:%f\r\n",i,s[i].real);
			}
			//FFT_draw();
		
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			
			
	create_sin_tab(my_sin_tab,NPT);					//�������ұ�ӿ�FFT�ٶȣ��ñ�ֻ��Ҫһ��������
	//���ɲ����źţ����ź������������źź�һ��ֱ���źźϳ�
	for(t=0;t<NPT;t++)                           	//���ṹ�帳ֵ
	{
		s[t].real=fft_signel(t); 	//ʵ��Ϊ���Ҳ�FFT_N���������ֵΪ1
		s[t].imag=0;                                //�鲿Ϊ0
	}
			__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//����TIM3��ʱ���ļ�����ֵ
			timeout=0;
			fft_test();
      time=__HAL_TIM_GET_COUNTER(&TIM3_Handler)+(u32)timeout*65536;//��������ʱ��
			sprintf((char*)buf,"%0.3fms\r\n",time/1000);	
			LCD_ShowString(46+12*8,176,100,16,16,buf);	//��ʾ����ʱ��	
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
		if((t%10)==0)LED0_Toggle;                	//LED0��ת		  
		
		
		
	}
}

