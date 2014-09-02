//encode.h
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PI 3.14
int shifter;//模拟左移移位寄存器当前状态[b(M-1),b(M-2),b(M-3),...,b(1)],最大表示16位
void gensignal(int* origin,int length,int mode)//产生信号
{//生成随机二进制数作为编码前的信息序列
	if(mode==0) for(int i=0;i<length;i++) origin[i]=0;//生成一位信息位
	if(mode==1) for(int i=0;i<length;i++) origin[i]=1;//生成一位信息位
	if(mode==2) for(int i=0;i<length;i++) origin[i]=i&1;//生成一位信息位
	if(mode==3)
	{
		unsigned long seed;
		time((long*)&seed);
		srand(seed);
		for(int i=0;i<length;i++) origin[i]=rand()&1;//生成一位信息位
	}
}
void encode(int** encodeori,int* origin,int* generate,int length,int M,int N)//编码器
{//生成卷积码,码率为1/N,一个码元有N比特
	int i,j,k,temp=shifter;
	for(k=0;k<length;k++)
	{//由此信息位生成N bit码元
		temp=(temp<<1)^origin[k];//模拟移位寄存器左移
		for(i=0;i<N;i++)
		{//生成此卷积码码元的1bit
			int sum=0,temp1=temp&generate[i];
			encodeori[k][i]=0;
			for(j=0;j<M;j++) sum^=(temp1>>j)&1;
			//实现卷积码编码电路输入输出关系,乘矩阵的列后自模2加
			encodeori[k][i]=(encodeori[k][i]<<1)^sum;//组合成卷积码码元
		}
		shifter=temp;
	}
}
void initialmatrix(int* generate,int M,int N)//十进制相关矩阵变为二进制相关矩阵
{
	for(int i=0;i<N;i++)
	{
		int sum=0,temp=generate[i];
		for(int j=0;j<M;j++)
		{
			sum+=((temp%10)<<j);
			temp=temp/10;
		}
		generate[i]=sum;
	}
}
void PSK(int** encodeori,int** PSKmaker,int length,int N)//PSK调制(1-->-1;0-->1)
{
	for(int i=0;i<length;i++) 
		for(int j=0;j<N;j++)
			{
				if(encodeori[i][j]) PSKmaker[i][j]=-1;
				else PSKmaker[i][j]=1;
			}
}
double gaosimaker(double DX)
{
	double rand1,rand2,result;
	rand1=(double)(rand())/RAND_MAX;//均匀分布
	rand2=(double)(rand())/RAND_MAX;//均匀分布
	result=sqrt((-2)*log(rand1))*cos(2*PI*rand2);
	//变换抽样,均匀分布变为正态分布
	return DX*result;
}
void addgaosi(double** send,int** PSKmaker,int length,int N,double DX)
{
	for(int i=0;i<length;i++) 
		for(int j=0;j<N;j++) 
			send[i][j]=PSKmaker[i][j]+gaosimaker(DX);
}
int result(int x)
{
	if(x=1) return 1;
	else if(x>1)
	{
		return result(x-1)+(int)pow(10,x-1);
	}
	else return 65535;
}