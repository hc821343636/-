#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdlib.h>
#pragma pack(1) /* �����ڽṹ�嶨��֮ǰʹ��,����Ϊ���ýṹ���и���Ա��1�ֽڶ���*/
typedef struct tagBITMAPFILEHEADER
{
 unsigned short bfType;		 //����ͼƬ���͡� 'BM'(1-2�ֽ�) 
 unsigned long bfSize; 	 //λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ��3-6�ֽڣ���λ��ǰ��
 unsigned short bfReserved1;//λͼ�ļ������֣�����Ϊ0(7-8�ֽڣ�
 unsigned short bfReserved2;//λͼ�ļ������֣�����Ϊ0(9-10�ֽڣ�
 unsigned long bfOffBits;  //RGB����ƫ�Ƶ�ַ,λͼ���ݵ���ʼλ�ã��������λͼ��11-14�ֽڣ���λ��ǰ��
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
 unsigned long 	biSize; 		 //���ṹ��ռ���ֽ�����15-18�ֽڣ�
 unsigned long  biWidth; 	 //λͼ�Ŀ�ȣ�������Ϊ��λ��19-22�ֽڣ�
 unsigned long  biHeight; 	 //λͼ�ĸ߶ȣ�������Ϊ��λ��23-26�ֽڣ�
 unsigned short biPlanes; 	 //Ŀ���豸�ļ��𣬱���Ϊ1(27-28�ֽڣ�
 unsigned short biBitCount;  //ÿ�����������λ����������1��˫ɫ����29-30�ֽڣ�,4(16ɫ����8(256ɫ��16(�߲�ɫ)��24�����ɫ��֮һ

 unsigned long  biCompression;//λͼѹ�����ͣ�������0����ѹ��������31-34�ֽڣ�
 //1(BI_RLE8ѹ�����ͣ���2(BI_RLE4ѹ�����ͣ�֮һ

 unsigned long  biSizeImage;  //λͼ�Ĵ�С(���а�����Ϊ�˲���������4�ı�������ӵĿ��ֽ�)�����ֽ�Ϊ��λ��35-38�ֽڣ�

 unsigned long  biXPelsPerMeter;//λͼˮƽ�ֱ��ʣ�ÿ����������39-42�ֽڣ�
 unsigned long  biYPelsPerMeter;//λͼ��ֱ�ֱ��ʣ�ÿ����������43-46�ֽ�)
 unsigned long  biClrUsed; 		 //λͼʵ��ʹ�õ���ɫ���е���ɫ����47-50�ֽڣ�
 unsigned long  biClrImportant; //λͼ��ʾ��������Ҫ����ɫ����51-54�ֽڣ�
}BITMAPINFOHEADER;

void Bmp_Bigger_And_Smaller(BITMAPFILEHEADER head,BITMAPINFOHEADER info,double bili,char *a,char *b)
{
 	FILE *fpr1=fopen(a,"rb");
	FILE *fpw2=fopen(b,"wb");
	if(fpr1==NULL||fpw2==NULL)
	{
	printf("ͼƬ��ʧ��!\n");
	return ;
	}
	//��ȡԭ��Ƭ��ͷ��Ϣ
	fread(&head,sizeof(BITMAPFILEHEADER),1,fpr1);
	fread(&info,sizeof(BITMAPINFOHEADER),1,fpr1);

	unsigned int old_width=info.biWidth;//��ȡԭͼƬ�Ŀ�
	unsigned int old_height=info.biHeight;//��ȡԭͼƬ�ĸ�

	//��ȡԭͼƬ��λͼ����
	unsigned char *src_data=(unsigned char *)malloc(old_width*old_height*3);
	fseek(fpr1,54,SEEK_SET);			//�����λ�����ֽ���Ϊ1��ʼ��λ����54λ 
	fread(src_data,old_width*old_height*3,1,fpr1);

	printf("ԭͼƬ�Ŀ�:%d\n",old_width);
	printf("ԭͼƬ�ĸ�:%d\n",old_height);

	//�޸�ԭ��Ƭ�Ŀ��
	unsigned int new_width,new_height;
	printf("��ͼƬ�Ŀ�:%d \n",(int)bili*old_width);
	printf("��ͼƬ�ĸ�:%d \n",(int)bili*old_height);
	new_width=(int)bili*old_width;
	new_height=(int)bili*old_height;
	head.bfSize=new_width*new_height*3+54;
	info.biWidth=new_width;
	info.biHeight=new_height;

	//���޸Ĺ���ͷ��Ϣǰ54���ֽڣ�д������Ƭ
	fwrite(&head,sizeof(BITMAPFILEHEADER),1,fpw2);
	fwrite(&info,sizeof(BITMAPINFOHEADER),1,fpw2);
	//�����ǰ������ֽ����ţ����ҿ�����put_data 
	int i=0,j=0;
	unsigned long dwsrcX,dwsrcY;
	unsigned char *pucDest;
	unsigned char *pucSrc;
	unsigned char *dest_data=(unsigned char *)malloc(new_width*new_height*3);
	//��������ֽ��������ã��Ȱ��տ�ȣ��ڰ��ո߶ȡ�˫��ѭ�� 
	for(i=0;i<new_height;i++)
	{
		dwsrcY=i/bili;
		pucDest=dest_data+i*new_width*3;		//����������forѭ������������ֽ��� 
		pucSrc=src_data+dwsrcY*old_width*3;		//��С/�Ŵ��� 
		for(j=0;j<new_width;j++)
		{
			dwsrcX=j/bili;
			memcpy(pucDest+j*3,pucSrc+dwsrcX*3,3);//���ݿ���
			//��Դsourec�и���m���ֽڵ�Ŀ��destination�� ,(void* destination, void* sourec, unsigned m);
		}
	}
	fseek(fpw2,54,SEEK_SET);			//�����λ�����ֽ���Ϊ1��ʼ��λ����54λ 
	fwrite(dest_data,new_width*new_height*3,1,fpw2);
	printf("�ɹ�!\n");
	
	//�ͷŶѿռ�,�ر��ļ�
	free(dest_data);
	free(src_data);
	fclose(fpr1);
	fclose(fpw2);
}
int main(int argc, char* argv[])
{
	//����ԭ��Ƭ��Ϣ�ṹ��
	BITMAPFILEHEADER old_head;
	BITMAPINFOHEADER old_info;
	//���ṹ�����
	memset(&old_head,0,sizeof(BITMAPFILEHEADER));
	memset(&old_info,0,sizeof(BITMAPINFOHEADER));

  	double bili = atof(argv[2])/100.0;//���ַ���ת��Ϊdouble��
  	printf("%f",bili);
  
	Bmp_Bigger_And_Smaller(old_head,old_info, bili,argv[1],argv[3]);
	return 0;
}


