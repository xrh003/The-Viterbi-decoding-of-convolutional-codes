//main.cpp
//卷积码(N,1,M)的viterbi译码
#include <iostream.h>
#include "encode.h"
#include "decode.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip.h> 
#define length 5000
#define M 3
#define N 3
#define statenum (int)pow(2,M-1)
int generate[N]={1,5,7};//001,101,111
int origin[length];
//encodeori[length][N]由信息位生成卷积码,码率为1/N,一个码元有N比特
int encodeori[length][N];
//PSKmaker[length][N]将卷积码调制成PSK信号
int PSKmaker[length][N];
//send[length][N]加入高斯白噪声后的信号
double send[length][N];
//从信道里接收的信号
int demodule[length][N];
//解码后的信号;由statenum个幸存路径筛选得出
	int recover[length];
int viterbikey(double DX,int mode)
{
	gensignal(origin,mode);
	encode(encodeori,origin,generate);//产生信号
	PSK(encodeori,PSKmaker);//PSK调制
	addgaosi(send,PSKmaker,DX);//加高斯白噪声
	demodulate(demodule,send);//解调
	survive* sur=viterbidecoder(demodule,recover,generate);
	int PE=pe(origin,recover);
	return PE;
}
double viterbi(double DX,int mode)
{
	int sum=0;
	sum=viterbikey(DX,mode);
	return 1.0*sum/length;
}