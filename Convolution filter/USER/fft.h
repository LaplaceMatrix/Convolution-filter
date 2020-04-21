#ifndef FFT_H
#define FFT_H
                                                 //���帣��Ҷ�任�ĵ���
#define PI_fft 3.1415926535897932384626433832795028841971 
#define PI                 3.14159265358979f              //����Բ����ֵ
#define		NPT		64	
struct compx {float real,imag;};                                    //����һ�������ṹ

extern void create_sin_tab(float *sin_t,int PointNum);
extern void FFT(struct compx *xin);
void fft_test(void);
float fft_signel(int x);


#define		fft_F1		50	//�����ź�1Ƶ��
#define		fft_A1		3	//�����ź�1����
#define		fft_P1		30	//�����ź�1��λ
#define		fft_F2		100	//�����ź�1Ƶ��
#define		fft_A2		1.5	//�����ź�1����
#define		fft_P2		90	//�����ź�1��λ
#define   fft_biase 4

#define 	ampi	10
float fir_filter(float input);
void filter_test(void);
#endif // FFT_H
