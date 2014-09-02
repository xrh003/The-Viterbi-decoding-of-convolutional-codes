//main.cpp
//卷积码(N,1,M)的viterbi译码
#include <iostream.h>
#include "decode.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip.h> 
#define length 10
#define M 3
#define N 3
#define statenum (int)pow(2,M-1)
#include <time.h>
int generate[N]={1,5,7};//001,101,111
int origin[length]={1,0,1,1,0,0,1,1,1,0};
//encodeori[length][N]由信息位生成卷积码,码率为1/N,一个码元有N比特
//int encodeori[length][N];
//PSKmaker[length][N]将卷积码调制成PSK信号
//int PSKmaker[length][N];
//send[length][N]加入高斯白噪声后的信号
double send[length][N]={{-1.1,-0.7,-0.8},{1.3,0.8,-0.8},{-1.2,0.4,1.6},{-1.1,-0.5,2.4},{-0.1,-1.7,0.96},{1.8,-1.6,-1.4},{-0.7,-0.7,-1.7},{-2.6,0.2,0.8},{-0.8,0.3,-0.4},{1.2,-0.6,1}};
//从信道里接收的信号
int demodule[length][N];
//解码后的信号;由statenum个幸存路径筛选得出
int recover[length];
int viterbikey(double DX,int mode)
{
	demodulate(demodule,send);//解调
	survive* sur=viterbidecoder(demodule,recover,generate);
	int PE=pe(origin,recover);
	return PE;
}
double viterbi()
{
	int mode=3,sum=0,i,j,k;
	double DX=1;
	for(i=0;i<10;i++)
	{
		sum=sum+viterbikey(DX,mode);
		cout<<"origin: ";
		for(j=0;j<length;j++) cout<<origin[j];
		cout<<endl<<"demodule: ";
		for(j=0;j<length;j++)
		{
			for(k=0;k<N;k++) cout<<demodule[j][k];
			cout<<" ";
		}
		cout<<endl<<"recover: ";
		for(j=0;j<length;j++) cout<<recover[j];
		cout<<endl<<endl;
	}
	return sum;
}
void main()
{
	unsigned long seed;
	time((long*)&seed);
	srand(seed);
	cout<<"误码率为： "<<viterbi()<<endl;
	cout<<statenum;
}