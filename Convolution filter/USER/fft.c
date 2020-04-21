/*********************************************************************
                         ���ٸ���Ҷ�任C�����
������飺�˳������ͨ�õĿ��ٸ���Ҷ�任C���Ժ�������ֲ��ǿ�����²��ֲ���
          ��Ӳ�����˳�����������������ʽ��ʾһ������������Ϊ��Ȼ˳��ĸ�
          ��������ʵ���ǿ�����鲿Ϊ0�������Ϊ����FFT�任����Ȼ˳���
          ����.�˳�������ڳ�ʼ��ʱ����create_sin_tab()�����������Һ�������
          �Ժ�Ŀɲ��ò���������ʱ�϶��sin��cos���㣬�ӿ�ɼ����ٶ�.��
          Ver1.1����Ƚϣ�Ver1.2���ڴ������ұ�ʱֻ������1/4�����Ҳ��Ĳ���ֵ��
          ���֮�½�ʡ��FFT_N/4���洢�ռ�
ʹ��˵����ʹ�ô˺���ֻ����ĺ궨��FFT_N��ֵ����ʵ�ֵ����ĸı䣬FFT_N��
          Ӧ��Ϊ2��N�η��������������ʱӦ�ں��油0����ʹ�ò��������sinֵ��
          cosֵ��Ӧ�ڵ���FFT����ǰ����create_sin_tab()�����������ұ�
�������ã�FFT(s);
��    �ߣ���˧��
ʱ    �䣺2010-2-20
��    ����Ver1.2
�ο����ף�
**********************************************************************/
#include "fft.h"
#include "usart.h"
#include "math.h"

float *SIN_TAB;//�������ұ��Ĵ�ſռ�
int FFT_N = 64;//����������С 
/*******************************************************************
����ԭ�ͣ�struct compx EE(struct compx b1,struct compx b2)
�������ܣ��������������г˷�����
��������������������嶨��ĸ���a,b
���������a��b�ĳ˻��������������ʽ���
*******************************************************************/
struct compx EE(struct compx a,struct compx b)
{
 struct compx c;
 c.real=a.real*b.real-a.imag*b.imag;
 c.imag=a.real*b.imag+a.imag*b.real;
 return(c);
}

/******************************************************************
����ԭ�ͣ�void create_sin_tab(float *sin_t��int PointNum)
�������ܣ�����һ�����Ҳ����������������븣��Ҷ�任������ͬ
���������*sin_t������ұ�������ָ��,PointNum��������
�����������
******************************************************************/
void create_sin_tab(float *sin_t,int PointNum)
{
  int i;
  SIN_TAB=sin_t;
  FFT_N=PointNum;
  for(i=0;i<=FFT_N/4;i++)
	SIN_TAB[i]=sin(2*PI_fft*i/FFT_N);
}
/******************************************************************
����ԭ�ͣ�void sin_tab(float PI_fft)
�������ܣ����ò���ķ�������һ����������ֵ
���������PI_fft ��Ҫ��������ֵ����ֵ����Χ0--2*PI_fft��������ʱ��Ҫת��
�������������ֵPI_fft������ֵ
******************************************************************/
float sin_tab(float pi)
{
  int n=0;
  float a=0;
   n=(int)(pi*FFT_N/2/PI);

  if(n>=0&&n<=FFT_N/4)
    a=SIN_TAB[n];
  else if(n>FFT_N/4&&n<FFT_N/2)
    {
     n-=FFT_N/4;
     a=SIN_TAB[FFT_N/4-n];
    }
  else if(n>=FFT_N/2&&n<3*FFT_N/4)
    {
     n-=FFT_N/2;
     a=-SIN_TAB[n];
   }
  else if(n>=3*FFT_N/4&&n<3*FFT_N)
    {
     n=FFT_N-n;
     a=-SIN_TAB[n];
   }

  return a;
}
/******************************************************************
����ԭ�ͣ�void cos_tab(float pi)
�������ܣ����ò���ķ�������һ����������ֵ
���������pi ��Ҫ��������ֵ����ֵ����Χ0--2*PI��������ʱ��Ҫת��
�������������ֵpi������ֵ
******************************************************************/
float cos_tab(float pi)
{
   float a,pi2;
   pi2=pi+PI_fft/2;
   if(pi2>2*PI_fft)
     pi2-=2*PI_fft;
   a=sin_tab(pi2);
   return a;
}
/*****************************************************************
����ԭ�ͣ�void FFT(struct compx *xin)
�������ܣ�������ĸ�������п��ٸ���Ҷ�任��FFT��
���������*xin�����ṹ������׵�ַָ�룬struct��
�����������
*****************************************************************/
void FFT(struct compx *xin)
{
  int f,m,i,k,l,j=0;
  register int nv2,nm1;
  struct compx u,w,t;

   nv2=FFT_N/2;                  //��ַ���㣬������Ȼ˳���ɵ�λ�򣬲����׵��㷨
   nm1=FFT_N-1;
   for(i=0;i<nm1;i++)
   {
    if(i<j)                    //���i<j,�����б�ַ
     {
      t=xin[j];
      xin[j]=xin[i];
      xin[i]=t;
     }
    k=nv2;                    //��j����һ����λ��
    while(k<=j)               //���k<=j,��ʾj�����λΪ1
     {
      j=j-k;                 //�����λ���0
      k=k/2;                 //k/2���Ƚϴθ�λ���������ƣ�����Ƚϣ�ֱ��ĳ��λΪ0
     }
   j=j+k;                   //��0��Ϊ1
  }

  {
   int le,lei,ip;                            //FFT����ˣ�ʹ�õ����������FFT����
    f=FFT_N;
   for(l=1;(f=f/2)!=1;l++)                  //����l��ֵ����������μ���
           ;
  for(m=1;m<=l;m++)                         // ���Ƶ��νἶ��
   {                                        //m��ʾ��m�����Σ�lΪ���μ�����l=log��2��N
    le=2<<(m-1);                            //le���ν���룬����m�����εĵ��ν����le��
    lei=le/2;                               //ͬһ���ν��вμ����������ľ���
    u.real=1.0;                             //uΪ���ν�����ϵ������ʼֵΪ1
    u.imag=0.0;
    //w.real=cos(PI_fft/lei);                  //�����ò��������sinֵ��cosֵ
    // w.imag=-sin(PI_fft/lei);
    w.real=cos_tab(PI_fft/lei);                //wΪϵ���̣�����ǰϵ����ǰһ��ϵ������
    w.imag=-sin_tab(PI_fft/lei);
    for(j=0;j<=lei-1;j++)                  //���Ƽ��㲻ͬ�ֵ��νᣬ������ϵ����ͬ�ĵ��ν�
     {
      for(i=j;i<=FFT_N-1;i=i+le)           //����ͬһ���ν����㣬������ϵ����ͬ���ν�
       {
        ip=i+lei;                          //i��ip�ֱ��ʾ�μӵ�������������ڵ�
        t=EE(xin[ip],u);                   //�������㣬�����ʽ
        xin[ip].real=xin[i].real-t.real;
        xin[ip].imag=xin[i].imag-t.imag;
        xin[i].real=xin[i].real+t.real;
        xin[i].imag=xin[i].imag+t.imag;
       }
      u=EE(u,w);                          //�ı�ϵ����������һ����������
     }
   }
  }
}

float my_sin_tab[NPT/4+1];
extern struct compx s[NPT];
void fft_test(void)
{
	int i;
	FFT(s);	
	for(i=0;i<NPT;i++)s[i].real=sqrt(s[i].real*s[i].real+s[i].imag*s[i].imag)/(i==0?NPT:NPT/2)*ampi;
}
float fft_signel(int t)
{
	return fft_biase+fft_A1*cos(2*PI_fft*fft_F1*t/NPT-PI_fft*fft_P1/180)+
		fft_A2*cos(2*PI_fft*fft_F2*t/NPT+PI_fft*fft_P2/180);
}

#define fir_window_length  129
const double  fir_window[fir_window_length] = {
                   0,7.507115422991e-07,-3.059218300465e-06,-7.908586591987e-06,
  1.096931354564e-05,2.522716169949e-05,-2.127664475054e-05,-5.592961849957e-05,
  3.061013224155e-05,0.0001032412393925,-3.44274717931e-05,-0.0001701846072365,
  2.676012824641e-05,0.0002592472610615,-9.187592283629e-19,-0.0003719201111427,
  -5.522659702103e-05,0.0005081195787663,0.0001500541344103,-0.0006655207800343,
  -0.0002972059959999,0.0008388413743188,0.0005106311912436,-0.001019124198428,
  -0.000804941144988, 0.001193069862266, 0.001194656487726,-0.001342466607131,
   -0.00169329323717, 0.001443752572386,  0.00231233648394,-0.001467723785094,
  -0.003060165867124, 0.001379367815018, 0.003941009082929,-0.001137754912909,
  -0.004954006042168,0.0006958492780687, 0.006092466144586,-9.853414867815e-18,
  -0.007343394070567,-0.001011292636334, 0.008687345695449, 0.002410021458403,
    -0.0100986559964, -0.00428393841486,  0.01154605648053, 0.006746712842992,
   -0.01299367249703, -0.00995756658747,  0.01440236289198,  0.01416048037434,
   -0.01573133804351, -0.01976759641318,  0.01693996954403,  0.02755505727871,
   -0.01798968760357, -0.03919533964194,  0.01884585215451,  0.05906699951019,
   -0.01947948161736,  -0.1033009304268,  0.01986872968103,   0.3173683024053,
     0.4800003582091,   0.3173683024053,  0.01986872968103,  -0.1033009304268,
   -0.01947948161736,  0.05906699951019,  0.01884585215451, -0.03919533964194,
   -0.01798968760357,  0.02755505727871,  0.01693996954403, -0.01976759641318,
   -0.01573133804351,  0.01416048037434,  0.01440236289198, -0.00995756658747,
   -0.01299367249703, 0.006746712842992,  0.01154605648053, -0.00428393841486,
    -0.0100986559964, 0.002410021458403, 0.008687345695449,-0.001011292636334,
  -0.007343394070567,-9.853414867815e-18, 0.006092466144586,0.0006958492780687,
  -0.004954006042168,-0.001137754912909, 0.003941009082929, 0.001379367815018,
  -0.003060165867124,-0.001467723785094,  0.00231233648394, 0.001443752572386,
   -0.00169329323717,-0.001342466607131, 0.001194656487726, 0.001193069862266,
  -0.000804941144988,-0.001019124198428,0.0005106311912436,0.0008388413743188,
  -0.0002972059959999,-0.0006655207800343,0.0001500541344103,0.0005081195787663,
  -5.522659702103e-05,-0.0003719201111427,-9.187592283629e-19,0.0002592472610615,
  2.676012824641e-05,-0.0001701846072365,-3.44274717931e-05,0.0001032412393925,
  3.061013224155e-05,-5.592961849957e-05,-2.127664475054e-05,2.522716169949e-05,
  1.096931354564e-05,-7.908586591987e-06,-3.059218300465e-06,7.507115422991e-07,
                   0
};
float infor_buff[fir_window_length];
float fir_filter(float input)
{
	float sum=0;int j=0;
	for( j=0;j<fir_window_length-1;j++)infor_buff[j]=infor_buff[j+1];
	infor_buff[fir_window_length-1]=input;
	for( j=0;j<fir_window_length;j++)sum=sum+infor_buff[j]*fir_window[j];
	return sum;
}
struct compx fir_s[NPT];
void filter_test(void)
{
	int i,t;
	//create_sin_tab(my_sin_tab,NPT);					//�������ұ��ӿ�FFT�ٶȣ��ñ�ֻ��Ҫһ��������
	//���ɲ����źţ����ź������������źź�һ��ֱ���źźϳ�
	for(i=0;i<fir_window_length;i++)infor_buff[i]=0;
	for(t=0;t<NPT;t++)                           	//���ṹ�帳ֵ
	{
		fir_s[t].real=fir_filter(fft_signel(t)); 	//ʵ��Ϊ���Ҳ�FFT_N���������ֵΪ1
		fir_s[t].imag=0;                                //�鲿Ϊ0
	}
	FFT(fir_s);	
	for(i=0;i<NPT;i++)
		fir_s[i].real=sqrt(fir_s[i].real*fir_s[i].real+fir_s[i].imag*fir_s[i].imag)/(i==0?NPT:NPT/2)*ampi;
}