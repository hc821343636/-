#include<bits/stdc++.h> 
using namespace std;
#define N 200

//�����������㺯��
int Pow(int a, int b)
{
    int i = 0, result = 1;
    for(i = 0; i < b; ++i)
    {
        result *= a;
    }
    return result;
}

int n;
void add(){
	char stra[N], strb[N];      //�ַ������飬���ַ���ʽ��������������
    int i = 0, step = 4, carry = 0; //step��ʾ�鳤��carryΪ��λλ��
    int lengtha, lengthb, maxlength, resultsize;    //maxlength��ʾstra��strb���߳��Ƚϴ���Ǹ���
    int numa[N], numb[N],numc[N];   //���δ��汻�������������ͣ�
    memset(numa, 0, sizeof(numa));
    memset(numb, 0, sizeof(numb));
    memset(numc, 0, sizeof(numc));         //��ʼ��Ϊ�㣻
    scanf("%s%s", stra, strb);
    lengtha = strlen(stra);
    lengthb = strlen(strb);     //�������������ĳ���
    //�ַ�����תΪ��λһ�����������
    for(i = lengtha-1; i >= 0; --i)
    {
        numa[(lengtha-1-i)/step] += (stra[i]-'0')*Pow(10,(lengtha-1-i)%step);
    }
    for(i = lengthb-1; i >= 0; --i)
    {
        numb[(lengthb-1-i)/step] += (strb[i]-'0')*Pow(10,(lengthb-1-i)%step);
    }
    maxlength = lengtha > lengthb ? lengtha : lengthb;

    //�����ӣ�����λ
    for(i = 0; i <= maxlength/step; ++i)
    {
        numc[i] = (numa[i] + numb[i])%Pow(10, step) + carry;    //�����
        carry = (numa[i] + numb[i])/Pow(10, step);  //�����λ
    }

    //�������͵Ŀ������
    resultsize = numc[maxlength/step] > 0 ? maxlength/step : maxlength/step - 1;
    printf("%d", numc[resultsize]);
    for(i = resultsize-1; i >= 0; --i)
    {
        printf("%04d", numc[i]);    //�Ҷ��룬���������
    }
    printf("\n");
}
void subtract(){
	char stra[N], strb[N];     //�ַ������飬���ַ���ʽ��������������
    int i = 0, step = 4, borrow = 0, mark = 0; //step��ʾ�鳤��borrowΪ��λλ, markΪ�������λ��
    int lengtha, lengthb, maxlength, resultsize;    //maxlength��ʾstra��strb���߳��Ƚϴ���Ǹ���
    int numa[N], numb[N],numc[N],  *maxnum, *minnum;   //���δ��汻�������������ͣ�
    memset(stra, 0, sizeof(stra));
    memset(strb, 0, sizeof(strb));
    memset(numa, 0, sizeof(numa));
    memset(numb, 0, sizeof(numb));
    memset(numc, 0, sizeof(numc));         //��ʼ��Ϊ�㣻
    scanf("%s%s", stra, strb);
    lengtha = strlen(stra);
    lengthb = strlen(strb);     //�������������ĳ���
    maxlength = lengtha >= lengthb ? lengtha : lengthb;

    //�ַ�����תΪ��λһ�����������
    for(i = lengtha-1; i >= 0; --i)
    {
        numa[(lengtha-1-i)/step] += (stra[i]-'0')*Pow(10,(lengtha-1-i)%step);
    }
    for(i = lengthb-1; i >= 0; --i)
    {
        numb[(lengthb-1-i)/step] += (strb[i]-'0')*Pow(10,(lengthb-1-i)%step);
    }

    //�ҳ��ϴ����
    maxnum = numa;
    minnum = numb;
    mark = 1;
    for(i = (maxlength-1)/step; i >= 0; --i)
    {
        if(numa[i] > numb[i])
        {
            maxnum = numa;
            minnum = numb;
            mark = 1;
            break;
        }
        else if(numa[i] < numb[i])
        {
            maxnum = numb;
            minnum = numa;
            mark = -1;
            break;
        }
    }

    //������������λ
    for(i = 0; i <= maxlength/step; ++i)
    {
        numc[i] = (maxnum[i] - minnum[i] + Pow(10, step) + borrow)%Pow(10,step);    //�����
        borrow = (maxnum[i] - minnum[i] + Pow(10, step) + borrow)/Pow(10, step) - 1;  //�����λ
    }

    //�������͵Ŀ������
    resultsize = maxlength/step;
    while(!numc[resultsize])    --resultsize;
    printf("%d", mark*numc[resultsize]);
    for(i = resultsize-1; i >= 0; --i)
    {
        printf("%04d", numc[i]);    //�Ҷ��룬���������
    }
    printf("\n");
	
}
int main(){
	printf("�ӷ���1��������2\n");
	cin>>n;
	if(n==1) add();
	if(n==2) subtract();
return 0;
}

