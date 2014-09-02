length=1000;M=3;N=3;statenum=2^(M-1);error=zeros(1,101);
for xyz=1:1:101
    DX=(xyz-1)/10;
    origin=randi([0,1],1,length);generate=[0,1,1;0,0,1;1,1,1];
    encodeori=zeros(length,N);PSKmaker=zeros(length,N);
    send=zeros(length,N);demodule=zeros(length,N);recover=zeros(1,length);
    shift=zeros(length,M);
    shift(1,M)=origin(1);
    for i=2:1:length
        shift(i,:)=[shift(i-1,2:end),origin(i)];
    end
    encodeori=mod(shift*generate,2);
    PSKmaker=ones(size(encodeori))-encodeori*2;
    send=PSKmaker+DX*randn(size(PSKmaker));
    demodule=send<0;
    shift=zeros(statenum,1,2*M-3);
    for i=1:1:statenum
        j=i-1;k=0;
        while j~=0
            shift(i,1,end-k)=mod(j,2);
            j=bitshift(j,-1);
            k=k+1;
        end
    end
    arraytonum=ones(M-1,1);
    for i=M-1:-1:2
        arraytonum(i-1)=2*arraytonum(i);
    end
    shift1=squeeze(shift(:,:,end-M+2:end));
    myfence.incode(1).value=mod([zeros(statenum,1),shift1]*generate,2);
    myfence.incode(2).value=mod([ones(statenum,1),shift1]*generate,2);
    mysurvive.hamingdist=zeros(statenum,length);
    mysurvive.sufferstate=zeros(statenum,length+1,M-1);
    mysurvive.outcode=zeros(statenum,length);
    for i=2:1:M
        mysurvive.sufferstate(:,i,:)=shift(:,1,(i-1):(i+M-3));
    end
    mysurvive.outcode(:,1:M-1)=shift1;
    mysurvive.sufferstate1=cat(3,zeros(statenum,M-1,1),mysurvive.sufferstate(:,2:M,:));
    mysurvive.hamingdist(:,1)=sum(mod(squeeze(mysurvive.sufferstate1(:,1,:))*generate,2)~=(ones(statenum,1)*demodule(1,:)),2)';
    for i=2:1:M-1
        mysurvive.hamingdist(:,i)=sum(mod(squeeze(mysurvive.sufferstate1(:,i,:))*generate,2)~=(ones(statenum,1)*demodule(i,:)),2)+mysurvive.hamingdist(:,i-1);
    end
    for i=M:1:length
        mysurvivenew=mysurvive;
        for j=1:1:statenum
            mysurvive.outcode(j,i)=mod(j-1,2);
            mysurvive.sufferstate(j,i+1,:)=shift1(j,:);
            state1=[0,shift1(j,1:end-1)]*arraytonum+1;state2=[1,shift1(j,1:end-1)]*arraytonum+1;
            dist1=sum(myfence.incode(1).value(j,:)~=demodule(i,:))+mysurvivenew.hamingdist(state1,i-1);
            dist2=sum(myfence.incode(2).value(j,:)~=demodule(i,:))+mysurvivenew.hamingdist(state2,i-1);
            if (dist1<dist2)||((dist1==dist2)&&(randi([0,1])==0))
                mysurvive.outcode(j,1:i-1)=mysurvivenew.outcode(state1,1:i-1);
                mysurvive.sufferstate(j,2:i,:)=mysurvivenew.sufferstate(state1,2:i,:);
                mysurvive.hamingdist(j,1:i-1)=mysurvivenew.hamingdist(state1,1:i-1);
                mysurvive.hamingdist(j,i)=dist1;
            else
                mysurvive.outcode(j,1:i-1)=mysurvivenew.outcode(state2,1:i-1);
                mysurvive.sufferstate(j,2:i,:)=mysurvivenew.sufferstate(state2,2:i,:);
                mysurvive.hamingdist(j,1:i-1)=mysurvivenew.hamingdist(state2,1:i-1);
                mysurvive.hamingdist(j,i)=dist2;            
            end
        end
    end
    thebest=min(find(mysurvive.hamingdist(:,end)==min(mysurvive.hamingdist(:,end))));
    recover=mysurvive.outcode(thebest,:);
    error(xyz)=sum(recover~=origin)/length;
end
plot(linspace(0,10,101),error)