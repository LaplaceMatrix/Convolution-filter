#ifndef FFT_H
#define FFT_H
                                                 //定义福利叶变换的点数
#define PI_fft 3.1415926535897932384626433832795028841971 
#define PI                 3.14159265358979f              //定义圆周率值
#define		NPT		64	
struct compx {float real,imag;};                                    //定义一个复数结构

extern void create_sin_tab(float *sin_t,int PointNum);
extern void FFT(struct compx *xin);
void fft_test(void);
float fft_signel(int x);


#define		fft_F1		50	//测试信号1频率
#define		fft_A1		3	//测试信号1幅度
#define		fft_P1		30	//测试信号1相位
#define		fft_F2		100	//测试信号1频率
#define		fft_A2		1.5	//测试信号1幅度
#define		fft_P2		90	//测试信号1相位
#define   fft_biase 4

#define 	ampi	10
float fir_filter(float input);
void filter_test(void);
#endif // FFT_H
