#ifndef PARTION_H
#define PARTION_H
#include<math.h>
#include<string>
#include<iostream>
#include<memory.h>
#include<queue>
#include <fstream>
#include <bitset>
#include <unordered_set>
#include <unordered_map>
//#include <unordered_set>
#include "querysupportstruct.h"
using namespace std;


class partion
{
private:
    int **widths,**logwidths;
    int partdepth,partwidth,start;
    unsigned int hashnum;
    cell **value;
    BOBHash64 * bobhash[MAX_HASH_NUM];
public:
    double outcou,incou;
    partion(unsigned int w, int h_num);
    ~partion()
    {
        for(int i=0;i<hashnum;i++)delete [] value[i];
        delete [] value;
    }
    void insert(string s1, string s2,maxtype weight);
    maxtype edgeQuery(string s1, string s2,int& h);
    maxtype edgequerypart(unsigned int pos,unsigned int fp,int num,unsigned int& nextpos);
    maxtype nodeQuery(string s1, int& h,bool type);//outval=1,inval=0
    void insertpart(const unsigned int pos,int fp,maxtype weight,int num);
    void showfp();
    bool reachbility(string s1,string s2);
    void calper();
};

void partion::calper(){
    for(int i=0;i<hashnum;i++){
        int cou[17];
        memset(cou,0,sizeof(int)*16);
        double used=0;
        for(unsigned int j=0;j<widths[i][0]*widths[i][1];j++){
            unsigned int left=0;
            while(left<PARTION){
                unsigned int last=left;
                if(edgequerypart(j,last,i,left)){
                    used++;
                    cou[left-last]+=1;
                }
            }
        }
        for(int i=1;i<17;i++)cout<<cou[i]/used<<endl;
        cout<<"----------------------------------\n";
    }
}

//广度优先搜索探寻两点之间是否可达
bool partion::reachbility(string s1,string s2){
    if(s1==s2)return true;
    unsigned int fp1,fp2,hash1,hash2,helper=(1<<20)-1,useless;
    unsigned long long h1,h2;
    unsigned int hashval1[16],hashval2[16];
    int i=0;
    while(true){
        h1=(bobhash[i]->run(s1.c_str(), strlen(s1.c_str())));
        h2=(bobhash[i]->run(s2.c_str(), strlen(s2.c_str())));
        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        h1>>=20;h2>>=20;i++;

        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        h1>>=20;h2>>=20;i++;

        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        i++;
        if(i>=hashnum)break;
    }

    for(int i=start;i<hashnum;i++){
        unordered_set<unsigned int> visited;
        queue<unsigned int> nei;
        maxtype res;
        bool reach=false;

        if(widths[i][0]<widths[i][1]){
            h1=hashval1[i]%logwidths[i][1],h2=hashval2[i]%logwidths[i][1];
            visited.insert(h1);
            nei.push(h1);
            while(!nei.empty()&&!reach){
                unsigned int now=nei.front()%logwidths[i][0];
                nei.pop();
                for(int j=0;j<logwidths[i][1];j++){
                    unsigned int fpos=(now*logwidths[i][1]+j);
                    unsigned int x=fpos/(partdepth*logwidths[i][1]),y=(fpos%logwidths[i][1])/partwidth,fp=((fpos/logwidths[i][1])%partdepth)*partwidth+(fpos%logwidths[i][1])%partwidth;
                    if(edgequerypart(x*widths[i][1]+y,fp,i,useless)!=0){
                        if(j==h2){
                            reach=true;
                            break;
                        }
                        else if(!visited.count(j)){
                            visited.insert(j);
                            nei.push(j);
                        }
                    }

                }
            }
		}
		else{
		    unsigned int mag=widths[i][0]/widths[i][1];
            unsigned int h1=hashval1[i]%logwidths[i][0],h2=hashval2[i]%logwidths[i][1];
            //if(s1=="1"&&s2=="2"&&i==2)cout<<hashval1[i]<<" "<<hashval2[i]<<endl;
            visited.insert(h1);
            nei.push(h1);
            while(!nei.empty()&&!reach){
                unsigned int now=nei.front();
                nei.pop();
                for(int j=0;j<logwidths[i][1];j++){
                    unsigned int fpos=(now*logwidths[i][1]+j);
                    unsigned int x=fpos/(partdepth*logwidths[i][1]),y=(fpos%logwidths[i][1])/partwidth,fp=((fpos/logwidths[i][1])%partdepth)*partwidth+(fpos%logwidths[i][1])%partwidth;
                    if(edgequerypart(x*widths[i][1]+y,fp,i,useless)!=0){
                        if(j==h2){
                            reach=true;
                            break;
                        }
                        else if(!visited.count(j)){
                            for(int k=0;k<mag;k++){
                                visited.insert(k*logwidths[i][1]+j);
                                nei.push(k*logwidths[i][1]+j);
                            }
                        }
                    }

                }
            }
		}

		if(!reach)return false;
    }
    return true;
}




maxtype partion::nodeQuery(string s2, int& h,bool type){
    maxtype m=UNMAX;
    unsigned int fp2,hash2,helper=(1<<20)-1,useless;
    unsigned long long h2;
    unsigned int hashval2[16];
    int i=0;
    while(true){
        h2=(bobhash[i]->run(s2.c_str(), strlen(s2.c_str())));
        hashval2[i]=h2&helper;
        h2>>=20;i++;

        hashval2[i]=h2&helper;
        h2>>=20;i++;

        hashval2[i]=h2&helper;
        i++;
        //cout<<h1<<" "<<h2<<endl;
        if(i>=hashnum)break;
    }
    for(int i=start;i<hashnum;i++){
		maxtype inner=0;
		//double cou=0;
        if(type){
            unsigned int fpos= (hashval2[i]%logwidths[i][0])*logwidths[i][1];
            unsigned int x=fpos/(partdepth*logwidths[i][1]),fp=((fpos/logwidths[i][1])%partdepth)*partwidth+(fpos%logwidths[i][1])%partwidth;

		    for(int j=0;j<widths[i][1];j++){
                unsigned int pos=x*widths[i][1]+j;
                unsigned int left=fp;
                while(left<fp+partwidth){
                    //cou++;
                    unsigned int last=left;
                    inner+=edgequerypart(pos,last,i,left);
                }
		    }
		    //outcou+=cou/widths[i][1];
        }
        else{
            unsigned int fpos= hashval2[i]%logwidths[i][1];
            unsigned int y=fpos/partwidth,fp=fpos%partwidth;
		    for(int j=0;j<widths[i][0];j++){
                unsigned int pos=j*widths[i][1]+y;
                unsigned int left=fp;
                while(left<PARTION){
                    //cou++;
                    unsigned int last=left;
                    inner+=edgequerypart(pos,last,i,left);
                    while(last<left)last+=partwidth;
                    left=last;
                }
		    }
		    //incou+=cou/widths[i][0];
        }
		if(inner<m){
            h=i;
            m=inner;
            if(m==0)return 0;
		}
    }
    if(m==UNMAX)return 0;
    return m;
}

maxtype partion::edgequerypart(unsigned int pos,unsigned int fp,int num,unsigned int& nextpos){
    int mid=fp,left=mid-1,right=mid+1;
    maxtype w;
    //左右隔板均存在表示没有越界
    if((left<0||value[num][pos].part[left])&&(mid+1==PARTION||value[num][pos].part[mid]))
        w=((value[num][pos].w>>(mid*PER))&((1<<PER)-1));
    else{
        while(left>=0&&!value[num][pos].part[left])left--;
        while(right<PARTION&&!value[num][pos].part[right-1])right++;
        if(left<0&&right>=PARTION)w=value[num][pos].w;
        else w=((value[num][pos].w>>((left+1)*PER))&((1<<(PER*(right-left-1)))-1));
    }
    nextpos=right;
    return w;
}

void partion::insertpart(const unsigned int pos,int fp,maxtype weight,int num){

    int mid=fp,left=mid-1,right=mid+1;
    maxtype w=weight;
    //左右隔板均存在表示没有越界
    if((left<0||value[num][pos].part[left])&&(mid+1==PARTION||value[num][pos].part[mid])){
        w+=((value[num][pos].w>>(mid*PER))&((1<<PER)-1));
    }

    else{
        while(left>=0&&!value[num][pos].part[left])left--;
        while(right<PARTION&&!value[num][pos].part[right-1])right++;
        if(left<0&&right>=PARTION)w+=value[num][pos].w;
        else{
            maxtype helper=1;helper<<=(PER*(right-left-1));
            w+=((value[num][pos].w>>((left+1)*PER))&(helper-1));
        }
    }
    //判断是否需要合并
    if(right-left-1==PARTION||w<(1<<((right-left-1)*PER))){
        int gap=right-left-1;
        maxtype helper=1;helper<<=(PER*gap);
        value[num][pos].w&=(~((helper-1) << (PER*(left+1))));
        //覆盖
        value[num][pos].w+=(w<<((left+1)*PER));
        return;
    }

    while(true){
        maxtype overflow=0;
        int l=left,r=right;
        //合并低位为最佳方案
        if(right>=PARTION||left==0||value[num][pos].part[left-1]){
            if(left==0||value[num][pos].part[left-1])left--;
            else{
                left--;
                while(left>=0&&!value[num][pos].part[left])left--;
            }
            maxtype helper=1;helper<<=(PER*(l-left));
            w+=((value[num][pos].w>>((left+1)*PER))&(helper-1));
        }

        //合并高位为最佳方案
        else if(left<0||right+1==PARTION||value[num][pos].part[right]){
            if(right+1==PARTION||value[num][pos].part[right])right++;
            else{
                right++;
                while(right<PARTION&&!value[num][pos].part[right-1])right++;
            }
            maxtype helper=1;helper<<=(PER*(right-r));
            w+=((value[num][pos].w>>(r*PER))&(helper-1));
        }

        //取表示区间小的进行合并
        else{
            left--;
            while(left>=0&&!value[num][pos].part[left])left--;

            right++;
            while(right<PARTION&&!value[num][pos].part[right-1])right++;

            if(l-left<=right-r){
                maxtype helper=1;helper<<=(PER*(l-left));
                w+=((value[num][pos].w>>((left+1)*PER))&(helper-1));
                right=r;
            }
            else{
                maxtype helper=1;helper<<=(PER*(right-r));
                w+=((value[num][pos].w>>(r*PER))&(helper-1));
                left=l;
            }

        }


        //cout<<w<<" "<<left<<" "<<right<<endl;
        int gap=right-left-1;
        if(gap==PARTION){
            value[num][pos].w=w;
        }
        else{
            //计算是否需要二次合并
            maxtype helper=1;helper<<=(PER*gap);
            if(w>=(helper)){
                overflow=w-(helper-1);
                w=(helper-1);
            }
            //区间合并
            value[num][pos].w&=(~((helper-1) << (PER*(left+1))));
            //覆盖
            value[num][pos].w+=(w<<((left+1)*PER));
        }

        //设置标记位
        //cout<<left<<" "<<right-1<<endl;
        if(left>=0)value[num][pos].part.set(left);
        if(right-1<PARTION-1)value[num][pos].part.set(right-1);
        for(int i=left+1;i<=right-2;i++)value[num][pos].part.reset(i);

        if(overflow==0){
            break;
        }
    }
}

partion::partion(unsigned int w, int h_num){

    //partdepth=sqrt(PARTION);
    outcou=0;
    incou=0;
    partdepth=4;
    partwidth=PARTION/partdepth;

    hashnum=h_num;
    widths=new int*[h_num];logwidths=new int*[h_num];
    for(int i=0;i<h_num;i++){widths[i]=new int[2];logwidths[i]=new int[2];}
    widths[0][0]=sqrt(w);
    widths[0][1]=widths[0][0];
    logwidths[0][0]=widths[0][0]*partdepth;
    logwidths[0][1]=widths[0][1]*partwidth;

    for(int i=1;i<=(h_num>>1);i++){
        int divd=1<<i;start=0;

//        int divd=16;start=1;
        widths[2*i-1][0]=sqrt(w)/divd;
        widths[2*i-1][1]=w/widths[2*i-1][0];
        widths[2*i-1][1]+=(widths[2*i-1][0]-(widths[2*i-1][1]%widths[2*i-1][0]));
        logwidths[2*i-1][0]=widths[2*i-1][0]*partdepth;
        logwidths[2*i-1][1]=widths[2*i-1][1]*partwidth;
        widths[2*i][0]=sqrt(w)*divd;
        widths[2*i][1]=w/widths[2*i][0];
        widths[2*i][0]+=(widths[2*i][1]-(widths[2*i][0]%widths[2*i][1]));
        logwidths[2*i][0]=widths[2*i][0]*partdepth;
        logwidths[2*i][1]=widths[2*i][1]*partwidth;
    }

    value=new cell*[h_num];
    for(int i=start;i<h_num;i++){
        int counternum=widths[i][0]*widths[i][1];
        value[i]=new cell[counternum];
    }
    for(int i = 0; i < MAX_HASH_NUM; i++){
		bobhash[i] = new BOBHash64(i + 1000);
	}
}

maxtype partion::edgeQuery(string s1, string s2,int& h){
    maxtype m=UNMAX;
    unsigned int fp1,fp2,hash1,hash2,helper=(1<<20)-1,useless;
    unsigned long long h1,h2;
    unsigned int hashval1[16],hashval2[16];
    int i=0;
    while(true){
        h1=(bobhash[i]->run(s1.c_str(), strlen(s1.c_str())));
        h2=(bobhash[i]->run(s2.c_str(), strlen(s2.c_str())));
        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        h1>>=20;h2>>=20;i++;

        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        h1>>=20;h2>>=20;i++;

        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        i++;
        if(i>=hashnum)break;
    }
    for(int i=start;i<hashnum;i++){
        h1=hashval1[i];h2=hashval2[i];
        maxtype res;
        unsigned int hash1= h1%logwidths[i][0];
        unsigned int hash2= h2%logwidths[i][1];
		unsigned int fpos=(hash1*logwidths[i][1]+hash2);
		unsigned int x=fpos/(partdepth*logwidths[i][1]),y=(fpos%logwidths[i][1])/partwidth,fp=((fpos/logwidths[i][1])%partdepth)*partwidth+(fpos%logwidths[i][1])%partwidth;
		res=edgequerypart(x*widths[i][1]+y,fp,i,useless);
		if(res<m){
            h=i;
            m=res;
            if(m==0)return m;
		}
    }
    if(m==UNMAX)return 0;
    return m;

}

void partion::insert(string s1, string s2,maxtype weight){
    unsigned int fp1,fp2,hash1,hash2,helper=(1<<20)-1,useless;
    unsigned long long h1,h2;
    unsigned int hashval1[16],hashval2[16];
    int i=0;
    while(true){
        h1=(bobhash[i]->run(s1.c_str(), strlen(s1.c_str())));
        h2=(bobhash[i]->run(s2.c_str(), strlen(s2.c_str())));
        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        h1>>=20;h2>>=20;i++;

        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        h1>>=20;h2>>=20;i++;

        hashval1[i]=h1&(helper);hashval2[i]=h2&helper;
        i++;
        if(i>=hashnum)break;
    }

    for(int i=start;i<hashnum;i++){
        h1=hashval1[i];h2=hashval2[i];
        unsigned int hash1= h1%logwidths[i][0];
        unsigned int hash2= h2%logwidths[i][1];


		unsigned int fpos=(hash1*logwidths[i][1]+hash2);
		unsigned int x=fpos/(partdepth*logwidths[i][1]),y=(fpos%logwidths[i][1])/partwidth,fp=((fpos/logwidths[i][1])%partdepth)*partwidth+(fpos%logwidths[i][1])%partwidth;
		//if(s1=="1"&&s2=="2"&&i==2)cout<<hashval1[i]<<" "<<hashval2[i]<<endl;
		insertpart(x*widths[i][1]+y,fp,weight,i);
    }

}
#endif // PARTION_H
