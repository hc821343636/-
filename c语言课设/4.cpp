#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void zip(char *filename,char *outfile)
{//ѹ������ 
	FILE *in, *out;//����ָ���ļ���ָ�� 
	int filelen;//ÿһ���ַ������ظ����ֵĴ��� 
	char cur,tmp;	
	if(!(in=fopen(filename,"rb")))//�Զ����Ʒ�ʽ��ֻ���ļ���ԭ�ļ�������ڣ� 
		printf("�ļ���ʧ��\n");//���ļ��������������ʾ 
	else
	{
		out=fopen(outfile,"wb");/*�����Ʒ�ʽ��ֻд�ļ�
		�����ļ������������д�������������Ḳ��ԭ���ݣ�*/ 
		cur=fgetc(in);//��ȡ��һ���ַ������ض�ȡ�����ַ�
		tmp=cur;
		filelen=1;
		while(!feof(in))//�����ļ�ĩβʱ����ѭ�� 
		{
			cur=fgetc(in);
			if(cur==tmp){
				filelen++;
			}//����RLEѹ�� 
			else{
				fputc(filelen+'0',out);//дһ���ַ����˴��Ǽ������֣�����ʧ���򷵻�EOF 
				fputc(tmp,out);//ͬ�ϣ��˴���ԭ�ַ� 
				tmp=cur;
				filelen=1;
			}
		}
	}
	fclose(in);
	fclose(out);//�ر��ļ� 
}
void unzip(char *filename,char *outfile)
{//��ѹ���� ��ע�ͻ����������һ�� ������׸�� 
	FILE *in, *out;
	int filelen;
	char cur;
	if(!(in=fopen(filename,"rb")))
		printf("�ļ���ʧ��\n");
	else
	{
		out = fopen(outfile,"wb");
		while(!feof(in)){
			filelen = fgetc(in)-'0';
			if(feof(in)) break;
			cur = fgetc(in);
			while(filelen--)
				fputc(cur,out);
		}
	}
	fclose(in);
	fclose(out);
}
int main(int argc,char *argv[])
{//ͨ���ж������в���������Ӧ���� 
	if(!strcmp(argv[2], "-d"))
	{
		unzip(argv[1], argv[3]);
		printf("decompress finished\n");
	}
	else if(!strcmp(argv[2],"-c"))
	{
		zip(argv[1], argv[3]);
		printf("compress finished\n");
	}
	else
		printf("�����������,�����¼��,-c : compress; -d : decompress\n"); 
	return 0;
}

