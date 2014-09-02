//main.cpp
//卷积码(N,1,M)的viterbi译码
#include <iostream.h>
#include "encode.h"
#include "decode.h"
#include <stdlib.h>
#include <math.h>
extern int result(int x);
double viterbi(double DX,int mode)
{
	int length=1000,M=3,N=3;//double DX;
	int* generate=new int[N];generate[0]=001;generate[1]=101;generate[2]=111;
	//generate[N]={001,101,111}电路输入输出相关矩阵M行N列,此时N=3
/*-----------------------初始化各个动态数组-----------------------------*/
	int* origin=new int[length];//origin[length]初始信息位
	//encodeori[length][N]由信息位生成卷积码,码率为1/N,一个码元有N比特
	int** encodeori=new int*[length];
	for(int temp=0;temp<length;temp++) encodeori[temp]=new int[N];
	//PSKmaker[length][N]将卷积码调制成PSK信号
	int** PSKmaker=new int*[length];
	for(temp=0;temp<length;temp++) PSKmaker[temp]=new int[N];
	//send[length][N]加入高斯白噪声后的信号
	double** send=new double*[length];
	for(temp=0;temp<length;temp++) send[temp]=new double[N];
	//从信道里接收的信号
/*
	double** receive=new double*[length];
	for(temp=0;temp<length;temp++) receive[temp]=new double[N];*/
	//经2PSK解调后的信号
	int** demodule=new int*[length];
	for(temp=0;temp<length;temp++) demodule[temp]=new int[N];
	//解码后的信号;由statenum个幸存路径筛选得出
	int* recover=new int[length];
/*-----------------------动态数组初始化结束-----------------------------*/
	int i,statenum=(int)pow(2,M-1);
	initialmatrix(generate,M,N);//初始化相关矩阵
	gensignal(origin,length,mode);
	encode(encodeori,origin,generate,length,M,N);//产生信号
	PSK(encodeori,PSKmaker,length,N);//PSK调制
	addgaosi(send,PSKmaker,length,N,DX);//加高斯白噪声
//	transmit(receive,send,length,N);//进入信道,传至接收端
	demodulate(demodule,send,length,N);//解调
	survive* sur=viterbidecoder(demodule,recover,generate,M,N,statenum,length);
	double PE=pe(origin,recover,length);
/*-----------------------释放动态数组所占空间-----------------------------*/
	for(i=0;i<length;i++) delete []encodeori[i];delete []encodeori;
	for(i=0;i<length;i++) delete []PSKmaker[i];delete []PSKmaker;
	for(i=0;i<length;i++) delete []send[i];delete []send;
//	for(i=0;i<length;i++) delete []receive[i];delete []receive;
	for(i=0;i<length;i++) delete []demodule[i];delete []demodule;
	delete []generate;
	delete []origin;
	delete []recover;
/*------------------------------释放结束----------------------------------*/
	return PE;
}
