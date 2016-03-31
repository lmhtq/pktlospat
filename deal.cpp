#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
#define SIZE 14400
#define N (100000 + 1000)
using namespace std;
char Str1[]=" TCP ";
char Str2[]="Seq=";
char Str3[]="Len=";
char Str4[]="[TCP Retransmission]";
char Str5[]="[TCP Previous segment not captured]";
char Str6[]="[TCP Out-Of-Order]";
char Str7[]="[TCP Fast Retransmission]";

struct Packet
{
	int type;
	int seq;
	int len;
	int flag;
}packet[N];

int char2num(char *p)
{
	int num=0,i;
	for(i=0;;i++)
	{
		if(p[i]>='0'&&p[i]<='9')
		{
			num=num*10+(p[i]-'0');
		}else break;
	}
	return num;
}

void print(int n)
{
	//printf("1\n");
	int i,flow=0;
	int seq = 0;
	for(i=0;i<n;i++)
	{
		if (seq >= packet[i].seq) {
			continue;
		} else {
			seq = packet[i].seq;
		}
		switch(packet[i].type)
		{
			//RTO
			case 0:packet[i].flag=0;if(packet[i].seq/SIZE==flow){printf("%d ",packet[i].flag, packet[i].seq);} else {printf("\n%d ",packet[i].flag, packet[i].seq); flow++;} break;
			//Prevous not captured
			case 3:packet[i].flag=1;if(packet[i].seq/SIZE==flow){printf("1 %d ",packet[i].flag, packet[i].seq-packet[i-1].len);} else {printf("1 %d ",packet[i].flag, packet[i].seq-packet[i-1].len); flow++;} break;
			//Out-of-order
			case 2:packet[i].flag=1;if(packet[i].seq/SIZE==flow){printf("%d ",packet[i].flag, packet[i].seq);} else {printf("\n%d ",packet[i].flag, packet[i].seq); flow++;} break;
			//Fast Retrans
			case 1:packet[i].flag=0;if(packet[i].seq/SIZE==flow){printf("%d ",packet[i].flag, packet[i].seq);} else {printf("\n%d ",packet[i].flag, packet[i].seq); flow++;} break;
			//Normal
			case 4:packet[i].flag=1;if(packet[i].seq/SIZE==flow){printf("%d ",packet[i].flag, packet[i].seq);} else {printf("\n%d ",packet[i].flag, packet[i].seq); flow++;} break;
		}
	}
}

void print_with_seq(int n)
{
	//printf("1\n");
	int i,flow=0;
	int seq = 0;
	for(i=0;i<n;i++)
	{
		if (seq >= packet[i].seq) {
			continue;
		} else {
			seq = packet[i].seq;
		}
		switch(packet[i].type)
		{
			//RTO
			case 0:packet[i].flag=0;if(packet[i].seq/SIZE==flow){printf("%d:%d ",packet[i].flag, packet[i].seq);} else {printf("\n%d:%d ",packet[i].flag, packet[i].seq); flow++;} break;
			//Prevous not captured
			case 3:packet[i].flag=1;if(packet[i].seq/SIZE==flow){printf("1 %d:%d ",packet[i].flag, packet[i].seq-packet[i-1].len);} else {printf("1 %d:%d ",packet[i].flag, packet[i].seq-packet[i-1].len); flow++;} break;
			//Out-of-order
			case 2:packet[i].flag=1;if(packet[i].seq/SIZE==flow){printf("%d:%d ",packet[i].flag, packet[i].seq);} else {printf("\n%d:%d ",packet[i].flag, packet[i].seq); flow++;} break;
			//Fast Retrans
			case 1:packet[i].flag=0;if(packet[i].seq/SIZE==flow){printf("%d:%d ",packet[i].flag, packet[i].seq);} else {printf("\n%d:%d ",packet[i].flag, packet[i].seq); flow++;} break;
			//Normal
			case 4:packet[i].flag=1;if(packet[i].seq/SIZE==flow){printf("%d:%d ",packet[i].flag, packet[i].seq);} else {printf("\n%d:%d ",packet[i].flag, packet[i].seq); flow++;} break;
		}
	}
}

bool cmp(Packet A,Packet B)
{
	if(A.seq != B.seq)
	{
		return A.seq < B.seq;
	}
	else 
	{
		return A.type < B.type;
	}
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		cout << "./deal file_name";
		return 0;
	}
	int i=0,num=0,length=0,n=0;
	char buf[500];
	FILE *fp=fopen(argv[1],"r");
	while(fgets(buf,500,fp)!=NULL)//read one line
	{
		//printf("%s",buf);
		char *p=strstr(buf,Str1)+strlen(Str1);
		length=char2num(p);
		//printf("%d length:%d\n",i,num);
		//++i;
		if(length>100)//packet that have real data
		{

			if(strstr(buf,Str4)!=NULL){
				packet[n].type=0;// Re
			}else if(strstr(buf,Str5)!=NULL){
				packet[n].type=3;// need to add packet
				//add a packet
				packet[n].type = 4;
				char *tmp = strstr(buf, Str2) + strlen(Str2);
				packet[n].seq = char2num(tmp) - 10;
				packet[n].len = 1;
				n++;
				packet[n].type = 3;
			}else if(strstr(buf,Str6)!=NULL){
				packet[n].type=2;//order
			}else if(strstr(buf,Str7)!=NULL){
				packet[n].type=1;//Fast_re
			}else{
				packet[n].type=4;//normal
			}
			char *seq=strstr(buf,Str2)+strlen(Str2);
			packet[n].seq=char2num(seq);
			char *len=strstr(buf,Str3)+strlen(Str3);
			packet[n].len=char2num(len);
			n++;//packet + +
		} else {
			//some info that need to add packet, its size if only 66
			if(strstr(buf,Str5)!=NULL){
				packet[n].type=3;// need to add packet
				//add a packet
				packet[n].type = 4;
				char *tmp = strstr(buf, Str2) + strlen(Str2);
				packet[n].seq = char2num(tmp) - 10;
				packet[n].len = 1;
				n++;
				packet[n].type = 3;
				char *seq=strstr(buf,Str2)+strlen(Str2);
				packet[n].seq=char2num(seq);
				char *len=strstr(buf,Str3)+strlen(Str3);
				packet[n].len=char2num(len);
				n++;//packet + +
			}
		}
	}

	sort(packet,packet+n-1,cmp);
	//  for(i=0;i<n;i++)
	//  {
	//    printf("type:%d  seq:%d  len:%d\n",packet[i].type,packet[i].seq,packet[i].len);
	//  }
	print(n);
	return 0;
}
