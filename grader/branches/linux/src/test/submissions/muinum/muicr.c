/*
TASK: muinum
LANG: C
*/

#include<stdio.h>
#include<string.h>

void ltofive(long a,char b[]){
	int p=0;

	do{	b[p++]=a%5+'0';
		a=a/5;
	}while(a!=0);
	b[p]='\0';
	}

long muitol(char a[]){
	int i;
	long r=0,p=1;

	for(i=0;i<strlen(a);i++)
	{     	r=r+p*(a[i]-'0');
		p=p*5;
	}
	return r;
	}

void mystrrev(char a[]){
	int p=0,i;
	char c[20];

	for(i=strlen(a)-1;i>=0;i--)
		c[p++]=a[i];
	c[p]='\0';
	strcpy(a,c);
	}

int checkmui(char a[]){
	int i,check=0;

	for(i=0;i<strlen(a);i++)
	{	if(a[i]!='0'&&a[i]!='1'&&a[i]!='3'&&a[i]!='7')
		{	break;
		}
		else if(i==strlen(a)-1)
			check=1;
	}
	return check;
	}

void incmui(char a[]){
	int i,p=0;

	for(i=0;i<strlen(a);i++)
	{   if(i==0)
	    {	if(a[i]=='0'){a[i]='1';p=0;}
		else if(a[i]=='1'){a[i]='3';p=0;}
		else if(a[i]=='3'){a[i]='7';p=0;}
		else if(a[i]=='7'){p=1;a[i]='0';}
	    }else
	    if(p==1)
	    {	if(a[i]=='0'){a[i]='1';p=0;}
		if(a[i]=='1'){a[i]='3';p=0;}
		if(a[i]=='3'){a[i]='7';p=0;}
		if(a[i]=='7'){a[i]='0';p=1;}
	    }
	}
	if(p!=0)
	{	a[i]='1';
		a[i+1]='\0';
	}
	}

void addmui(char a[],char b[],char c[]){
	long al,bl,cl;

	al=muitol(a);
	bl=muitol(b);
	cl=al+bl;
	ltofive(cl,c);
	}

int main(){
	int n,j,i;
	long in[1050],r[1050],check=0;
	char b1[1050][20],b2[1050][20],c[20],temp[20],temp2[20];

	scanf("%d",&n);
	for(i=0;i<n;i++)
		scanf("%ld",&in[i]);
	for(i=0;i<n;i++)
	{       ltofive(in[i],c);
		if(checkmui(c)==1)
		{       mystrrev(c);
			strcpy(b1[i],c);
			strcpy(b2[i],"0");
		}else
		{       strcpy(temp,"1");
			check=0;
			do{	addmui(c,temp,temp2);
				check=checkmui(temp2);
				if(check)
				{       mystrrev(temp2);
					mystrrev(temp);
					strcpy(b1[i],temp2);
					strcpy(b2[i],temp);
					break;
				}
				incmui(temp);
			}while(!check);
		}
	}     /*
	for(i=0;i<n;i++)
	{       mystrrev(b[i]);
		puts(b[i]);
	}   */
	for(i=0;i<n;i++)
		printf("%s-%s\n",b1[i],b2[i]);
	return 0;
	}