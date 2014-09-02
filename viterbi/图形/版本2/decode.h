//deoutcode.h
#include <math.h>
#include <iostream.h>
#include <stdlib>
extern void initialmatrix(int* generate,int M,int N);
struct fencenode{
	 int currentstate;//左移寄存器当前状态(...000,...001,...010,...)
	 int incode[2];//输入数据后先后到达此点的两个点分别生成的卷积码
	 int outcode[2];//outcode[0]:输入0后生成的卷积码;outcode[1]:输入1后生成的卷积码
	 fencenode* in[2];//输入数据后到达此点的两个点
	 fencenode* out[2];//out[0]:输入0后从此点到达的点;out[1]:输入1后从此点到达的点
};
struct survive{
	 int hamingdist;//累积汉明距离
	 int* sufferstate;//历经状态
	 int* outcode;//对应的原信号
};
fencenode* initstatediag(int* generate,int M,int N,int statenum);
 int dist( int dist1, int* dist2, int N);
 int distcomp( int dist1, int dist2, int* dist0, int N,int ham1,int ham2);
void copysur(survive* mysurvivenew,survive* survive,int i,int statenum);
void copysur1(survive* surv1,int m,survive* surv2,int n,int i);
 survive* findmin( survive* surv, int i);
void demodulate(int** demodule,double** receive,int length,int N)//解调2PSK信号
{//硬判决,判决门限0(小于0-->1,大于0-->0)
	for(int i=0;i<length;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(receive[i][j]<0) demodule[i][j]=1;
			else demodule[i][j]=0;
		}
	}
}
survive* viterbidecoder( int** demodule, int* recover, int* generate, int M, int N, int statenum, int length)
{
	 int i,j;
	 survive* mysurvive=new survive[statenum];//最多有状态数(statenum)个幸存路径
	 survive* mysurvivenew=new survive[statenum];//最多有状态数(statenum)个幸存路径
	for(i=0;i<statenum;i++)
	{//初始化幸存路径数组
		mysurvive[i].sufferstate=new int[length+1];
		mysurvivenew[i].sufferstate=new int[length+1];
		mysurvive[i].outcode=new int[length];
		mysurvivenew[i].outcode=new int[length];
	}
	 fencenode* myfence=initstatediag(generate,M,N,statenum);
	for(i=0;i<statenum;i++)//对幸存路径的初始化
	{
		mysurvive[i].hamingdist=0;//设置汉明初值
		mysurvive[i].sufferstate[0]=0;//初始状态为0
		for(j=0;j<length;j++)
		{
			mysurvive[i].sufferstate[j+1]=0;
			mysurvive[i].outcode[j]=0;
		}
	}
	for(i=1;i<=length;i++)
	{//生成第i个时间段内的幸存路线片段
		if(pow(2,i)<=statenum)//对应篱笆图左部下落区
		{
			for(j=0;j<statenum;j++)
			{//生成第j状态对应的幸存路线
				int temp1=(j>>(M-i))&3,temp2=(j>>(M-1-i))&1;
				mysurvive[j].sufferstate[i]=myfence[temp1].out[temp2]->currentstate;
				mysurvive[j].hamingdist+=dist(myfence[temp1].outcode[temp2],demodule[i-1],N);
				mysurvive[j].outcode[i-1]=temp2;
			}
		}
		else
		{//此篱笆图区域内每个状态必由2条路径到达,取汉明距离较小的为此时间段内此状态的幸存路径
			copysur(mysurvivenew,mysurvive,i,statenum);
			//准备由前一次结果重新排序幸存队列,将mysurvive赋给mysurvivenew
			for(j=0;j<statenum;j++)
			{
				fencenode* node1=myfence[j].in[0];fencenode* node2=myfence[j].in[1];//找到转移至状态j的两个状态的地址
				int code1=myfence[j].incode[0],code2=myfence[j].incode[1];
				int k=distcomp(code1,code2,demodule[i-1],mysurvivenew[node1->currentstate].hamingdist,mysurvivenew[node2->currentstate].hamingdist);
				if(0==k)
				{//node1的距离小
					copysur1(mysurvive,j,mysurvivenew,node1->currentstate,i);//赋予新路径
					mysurvive[j].sufferstate[i]=j;
					mysurvive[j].hamingdist+=dist(code1,demodule[i-1],N);
					mysurvive[j].outcode[i-1]=j&1;
				}
				else if(1==k) 
				{//node2的距离小
					copysur1(mysurvive,j,mysurvivenew,node2->currentstate,i);//赋予新路径
					mysurvive[j].sufferstate[i]=j;
					mysurvive[j].hamingdist+=dist(code2,demodule[i-1],N);
					mysurvive[j].outcode[i-1]=j&1;
				}
				else
				{
					if(rand()<RAND_MAX/2)
					{
						copysur1(mysurvive,j,mysurvivenew,node1->currentstate,i);//赋予新路径
						mysurvive[j].sufferstate[i]=j;
						mysurvive[j].hamingdist+=dist(code1,demodule[i-1],N);
						mysurvive[j].outcode[i-1]=j&1;
					}
					else
					{
						copysur1(mysurvive,j,mysurvivenew,node2->currentstate,i);//赋予新路径
						mysurvive[j].sufferstate[i]=j;
						mysurvive[j].hamingdist+=dist(code2,demodule[i-1],N);
						mysurvive[j].outcode[i-1]=j&1;
					}
				}
			}
		}
	}	
	 survive* thebest=findmin(mysurvive,statenum);//找出statenum条路径中汉明距离最小的
	for(i=0;i<statenum;i++)
	{
		for(j=0;j<length;j++) recover[j]=thebest->outcode[j];
	}
	for(i=0;i<statenum;i++)
	{//初始化幸存路径数组
		delete []mysurvive[i].sufferstate;
		delete []mysurvivenew[i].sufferstate;
		delete []mysurvive[i].outcode;
		delete []mysurvivenew[i].outcode;
	}
	delete []mysurvive;
	delete []mysurvivenew;
	delete []myfence;
	return mysurvive;
}
fencenode* initstatediag(int* generate,int M,int N,int statenum)
{
	int i,j,k;//计算状态数
	fencenode* trellisdiagram=new fencenode[statenum];//网格图雏形
	initialmatrix(generate,M,N);
	for(i=0;i<statenum;i++)
	{
		trellisdiagram[i].outcode[0]=0;
		trellisdiagram[i].outcode[1]=0;
		trellisdiagram[i].currentstate=i;
		trellisdiagram[i].in[0]=&trellisdiagram[(i>>1)&(statenum-1)];//第1个到达状态i的状态
		trellisdiagram[i].in[1]=&trellisdiagram[((i>>1)+statenum/2)&(statenum-1)];//第2个到达状态i的状态
		//(i>>1)+1<<(log(2,statenum)-1)=(i>>1)+statenum/2  补充左移移位丢失的1
		trellisdiagram[i].out[0]=&trellisdiagram[(i<<1)&(statenum-1)];//输入0所得状态
		trellisdiagram[i].out[1]=&trellisdiagram[((i<<1)+1)&(statenum-1)];//输入1所得状态
		for(j=0;j<N;j++)
		{//生成此卷积码码元的1bit
			int sum1=0,sum2=0;
			int temp1=((i<<1)&(2*statenum-1))&generate[j];
			int temp2=(((i<<1)+1)&(2*statenum-1))&generate[j];
			for(k=0;k<M;k++) {sum1^=(temp1>>k)&1;sum2^=(temp2>>k)&1;}
			//实现卷积码编码电路输入输出关系,乘矩阵的列后自模2加
			trellisdiagram[i].outcode[0]=(trellisdiagram[i].outcode[0]<<1)^sum1;//组合成卷积码码元
			trellisdiagram[i].outcode[1]=(trellisdiagram[i].outcode[1]<<1)^sum2;//组合成卷积码码元
		}
	}
	for(i=0;i<statenum;i++)
	{
		trellisdiagram[i].incode[0]=trellisdiagram[i].in[0]->outcode[i&1];
		trellisdiagram[i].incode[1]=trellisdiagram[i].in[1]->outcode[i&1];
	}
	return trellisdiagram;
}
 int dist( int dist1, int* dist2, int N)//dist1:二进制数,dist2:十进制数组,宽均为N
{
	 int temp,sum=0,i;//按位异或
	for(i=0;i<N;i++)
	{
		temp=((dist1>>i)&1)^dist2[N-1-i];
		sum=sum+temp;
	}//求汉明距离
	return sum;
}
 int distcomp( int dist1, int dist2, int* dist0, int N,int ham1,int ham2)
{
	if((dist(dist1,dist0)+ham1)<(dist(dist2,dist0)+ham2)) return 0;
	else if((dist(dist1,dist0)+ham1)>(dist(dist2,dist0)+ham2)) return 1;
	else return 2;
}
void copysur(survive* mysurvivenew,survive* mysurvive,int i,int statenum)
{//将路径数组mysurvive中每个路径的前i个数据赋给mysurvivene中的每个路径
	for( int j=0;j<statenum;j++)
	{
		mysurvivenew[j].hamingdist=mysurvive[j].hamingdist;
		mysurvivenew[j].sufferstate[0]=mysurvive[j].sufferstate[0];
		for( int k=0;k<i;k++)
		{
			mysurvivenew[j].sufferstate[k]=mysurvive[j].sufferstate[k];
			mysurvivenew[j].outcode[k-1]=mysurvive[j].outcode[k-1];
		}
	}
}
void copysur1( survive* surv1, int m, survive* surv2, int n, int i)
{//将路径surv2的前i个数据赋给surv1,变成新的surv1
		surv1[m].hamingdist=surv2[n].hamingdist;
		surv1[m].sufferstate[0]=surv2[n].sufferstate[0];
		for( int k=1;k<i;k++)
		{
			surv1[m].sufferstate[k]=surv2[n].sufferstate[k];
			surv1[m].outcode[k-1]=surv2[n].outcode[k-1];
		}
}
 survive* findmin( survive* surv, int i)//在i个路径中找汉明距离最小的
{
	 int j,best=0,smaller=surv[0].hamingdist;
	for(j=0;j<i;j++)
	{
		if(surv[j].hamingdist<smaller) {smaller=surv[j].hamingdist;best=j;}
	}
	return &surv[best];
}
double pe(int* origin,int* recover,int length)
{
	int i,sum=0;
	for(i=0;i<length;i++)
		if(origin[i]!=recover[i]) sum++;
	return (double)sum/length;
}