//decode.h
#include <math.h>
#include <iostream.h>
#include <stdlib>
#define length 10
#define M 3
#define N 3
#define statenum (int)pow(2,M-1)
struct fencenode{
	int currentstate;//左移寄存器当前状态(...000,...001,...010,...)
	int incode[2];//输入数据后先后到达此点的两个点分别生成的卷积码
	int outcode[2];//outcode[0]:输入0后生成的卷积码;outcode[1]:输入1后生成的卷积码
	fencenode* in[2];//输入数据后到达此点的两个点
	fencenode* out[2];//out[0]:输入0后从此点到达的点;out[1]:输入1后从此点到达的点
};
struct survive{
	int hamingdist;//累积汉明距离
	int sufferstate[length+1];//历经状态
	int outcode[length];//对应的原信号
};
fencenode* initstatediag(int* generate);
int dist(int dist1,int* dist2);
int distcomp(int dist1,int dist2,int* dist0,int ham1,int ham2);
void copysur(survive* mysurvivenew,survive* survive,int i);
void copysur1(survive* surv1,int m,survive* surv2,int n,int i);
survive* findmin(survive* surv,int i);
void demodulate(int demodule[length][N],double receive[length][N])//解调2PSK信号
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
survive* viterbidecoder(int demodule[length][N],int* recover,int* generate)
{
	int i,j;
	survive* mysurvive=new survive[statenum];//最多有状态数(statenum)个幸存路径
	survive* mysurvivenew=new survive[statenum];//最多有状态数(statenum)个幸存路径
	fencenode* myfence=initstatediag(generate);
	for(i=0;i<statenum;i++)//对幸存路径的初始化
	{
		mysurvive[i].hamingdist=0;//设置汉明初值
		mysurvive[i].sufferstate[0]=0;//初始状态为0
		mysurvivenew[i].hamingdist=0;
		mysurvivenew[i].sufferstate[0]=0;
		for(j=0;j<length;j++)
		{
			mysurvive[i].sufferstate[j+1]=0;
			mysurvivenew[i].outcode[j]=0;
			mysurvive[i].sufferstate[j+1]=0;
			mysurvivenew[i].outcode[j]=0;
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
				mysurvive[j].hamingdist+=dist(myfence[temp1].outcode[temp2],demodule[i-1]);
				mysurvive[j].outcode[i-1]=temp2;
			}
		}
		else
		{//此篱笆图区域内每个状态必由2条路径到达,取汉明距离较小的为此时间段内此状态的幸存路径
			copysur(mysurvivenew,mysurvive,i);
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
					mysurvive[j].hamingdist+=dist(code1,demodule[i-1]);
					mysurvive[j].outcode[i-1]=j&1;
				}
				else if(1==k)
				{//node2的距离小
					copysur1(mysurvive,j,mysurvivenew,node2->currentstate,i);//赋予新路径
					mysurvive[j].sufferstate[i]=j;
					mysurvive[j].hamingdist+=dist(code2,demodule[i-1]);
					mysurvive[j].outcode[i-1]=j&1;
				}
				else
				{
					if(rand()<RAND_MAX/2)
					{
						copysur1(mysurvive,j,mysurvivenew,node1->currentstate,i);//赋予新路径
						mysurvive[j].sufferstate[i]=j;
						mysurvive[j].hamingdist+=dist(code1,demodule[i-1]);
						mysurvive[j].outcode[i-1]=j&1;
					}
					else
					{
						copysur1(mysurvive,j,mysurvivenew,node2->currentstate,i);//赋予新路径
						mysurvive[j].sufferstate[i]=j;
						mysurvive[j].hamingdist+=dist(code2,demodule[i-1]);
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
	delete []mysurvive;
	delete []mysurvivenew;
	delete []myfence;
	return mysurvive;
}
fencenode* initstatediag(int* generate)
{
	fencenode* trellisdiagram=new fencenode[statenum];//网格图雏形
	for(int i=0;i<statenum;i++) trellisdiagram[i].currentstate=i;
	trellisdiagram[0].in[0]=&trellisdiagram[0];trellisdiagram[0].incode[0]=0;
	trellisdiagram[0].in[1]=&trellisdiagram[2];trellisdiagram[0].incode[1]=3;
	trellisdiagram[0].out[0]=&trellisdiagram[0];trellisdiagram[0].outcode[0]=0;
	trellisdiagram[0].out[1]=&trellisdiagram[1];trellisdiagram[0].outcode[1]=7;
	trellisdiagram[1].in[0]=&trellisdiagram[0];trellisdiagram[1].incode[0]=7;
	trellisdiagram[1].in[1]=&trellisdiagram[2];trellisdiagram[1].incode[1]=4;
	trellisdiagram[1].out[0]=&trellisdiagram[2];trellisdiagram[1].outcode[0]=1;
	trellisdiagram[1].out[1]=&trellisdiagram[3];trellisdiagram[1].outcode[1]=6;
	trellisdiagram[2].in[0]=&trellisdiagram[1];trellisdiagram[2].incode[0]=1;
	trellisdiagram[2].in[1]=&trellisdiagram[3];trellisdiagram[2].incode[1]=2;
	trellisdiagram[2].out[0]=&trellisdiagram[0];trellisdiagram[2].outcode[0]=3;
	trellisdiagram[2].out[1]=&trellisdiagram[1];trellisdiagram[2].outcode[1]=4;
	trellisdiagram[3].in[0]=&trellisdiagram[1];trellisdiagram[3].incode[0]=6;
	trellisdiagram[3].in[1]=&trellisdiagram[3];trellisdiagram[3].incode[1]=5;
	trellisdiagram[3].out[0]=&trellisdiagram[2];trellisdiagram[3].outcode[0]=2;
	trellisdiagram[3].out[1]=&trellisdiagram[3];trellisdiagram[3].outcode[1]=5;
	return trellisdiagram;
}
int dist(int dist1,int* dist2)//dist1:二进制数,dist2:十进制数组,宽均为N
{
	int sum=0;
	switch(4*dist2[0]+2*dist2[1]+dist2[2])
	{
		case 0:
			switch(dist1)
			{
				case 0:sum=0;break;
				case 1: case 2: case 4:sum=1;break;
				case 3: case 5: case 6:sum=2;break;
				case 7:sum=3;break;
			}
			break;
		case 1:
			switch(dist1)
			{
				case 1:sum=0;break;
				case 0: case 3: case 5:sum=1;break;
				case 2: case 4: case 7:sum=2;break;
				case 6:sum=3;break;
			}
			break;
		case 2:
			switch(dist1)
			{
				case 2:sum=0;break;
				case 0: case 3: case 6:sum=1;break;
				case 1: case 4: case 7:sum=2;break;
				case 5:sum=3;break;
			}
			break;
		case 3:
			switch(dist1)
			{
				case 3:sum=0;break;
				case 1: case 2: case 7:sum=1;break;
				case 0: case 5: case 6:sum=2;break;
				case 4:sum=3;break;
			}
			break;
		case 4:
			switch(dist1)
			{
				case 4:sum=0;break;
				case 0: case 5: case 6:sum=1;break;
				case 1: case 2: case 7:sum=2;break;
				case 3:sum=3;break;
			}
			break;
		case 5:
			switch(dist1)
			{
				case 5:sum=0;break;
				case 1: case 4: case 7:sum=1;break;
				case 0: case 3: case 6:sum=2;break;
				case 2:sum=3;break;
			}
			break;
		case 6:
			switch(dist1)
			{
				case 6:sum=0;break;
				case 2: case 4: case 7:sum=1;break;
				case 0: case 3: case 5:sum=2;break;
				case 1:sum=3;break;
			}
			break;
		case 7:
			switch(dist1)
			{
				case 7:sum=0;break;
				case 3: case 5: case 6:sum=1;break;
				case 1: case 2: case 4:sum=2;break;
				case 0:sum=3;break;
			}
			break;
		default:break;
	}
/*
	int temp,sum=0,i;//按位异或
	for(i=0;i<N;i++)
	{
		temp=((dist1>>i)&1)^dist2[N-1-i];
		sum=sum+temp;
	}//求汉明距离*/
	return sum;
}
int distcomp(int dist1,int dist2,int* dist0,int ham1,int ham2)
{
	if((dist(dist1,dist0)+ham1)<(dist(dist2,dist0)+ham2)) return 0;
	else if((dist(dist1,dist0)+ham1)>(dist(dist2,dist0)+ham2)) return 1;
	else return 2;
}
void copysur(survive* mysurvivenew,survive* mysurvive,int i)
{//将路径数组mysurvive中每个路径的前i个数据赋给mysurvivene中的每个路径
	for(int j=0;j<statenum;j++)
	{
		for(int l=1;l<i;l++)//找到不同路径的源头
		{
			if(mysurvivenew[j].sufferstate[l]!=mysurvive[j].sufferstate[l]) break;
		}
		mysurvivenew[j].hamingdist=mysurvive[j].hamingdist;
		for(int k=l;k<i;k++)
		{
			mysurvivenew[j].sufferstate[k]=mysurvive[j].sufferstate[k];
			mysurvivenew[j].outcode[k-1]=mysurvive[j].outcode[k-1];
		}
	}
}
void copysur1(survive* surv1,int m,survive* surv2,int n,int i)
{//将路径surv2的前i个数据赋给surv1,变成新的surv1
	for(int l=1;l<i;l++)//找到不同路径的源头
	{
		if(surv1[m].sufferstate[l]!=surv2[n].sufferstate[l]) break;
	}
	surv1[m].hamingdist=surv2[n].hamingdist;
	for(int k=l;k<i;k++)
	{
		surv1[m].sufferstate[k]=surv2[n].sufferstate[k];
		surv1[m].outcode[k-1]=surv2[n].outcode[k-1];
	}
}
survive* findmin(survive* surv,int i)//在i个路径中找汉明距离最小的
{
	int j,k=0,best=0,smaller=surv[0].hamingdist;
	for(j=0;j<i;j++)
	{
		if(surv[j].hamingdist<smaller) {smaller=surv[j].hamingdist;best=j;}
/*
		else if(surv[j].hamingdist=smaller)
		{
			if(rand()&1) best=j;
		}*/
	}
	return &surv[best];
}
int pe(int* origin,int* recover)
{
	int i,sum=0;
	for(i=0;i<length;i++)
		if(origin[i]!=recover[i]) return 1;
	return 0;
}