/*******************************************************************************
ע�����
�����Լ�ʹ�õ�LCD1602�Ƿ����ת�Ӱ壬�������ת�Ӱ�ļ�Ϊ4λ������LCD.Hͷ�ļ���
����#define LCD1602_4PINS�򿪣���������ʹ�õ�LCD1602��8λ������Ĭ�Ͻ��ú�ע�͡�

����˵��������������ÿ��ʵ���½��ڵ�ʵ�������ж�Ӧ�Ľ���ͼ˵����
LCD1602���߲ο�LCD1602Һ����ʾʵ�����ͼ
DS18B20���������߲ο�DS18B20�¶ȴ�����ʵ�����ͼ



ʵ�������
LCD1602����ʾ�ɼ����¶ȣ�ͬʱ
�򿪴��ڵ������֣������ڲ�����ѡ��4800���򿪴��ڣ���ʱ�ڴ����Ͼͻ���ʾ�ɼ����¶ȣ����ڽ�ͼ�ο��ļ����¡�
*******************************************************************************/

#include<reg51.h>
#include"lcd.h"
#include"temp.h"

uchar CNCHAR[6] = "���϶�";
void LcdDisplay(int);
void UsartConfiguration();

/*******************************************************************************
* ������         : main
* ��������		   : ������
* ����           : ��
* ���         	 : ��
*******************************************************************************/

void main()
{
	UsartConfiguration();
	LcdInit();			 //��ʼ��LCD1602
	LcdWriteCom(0x88);	//д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('C'); 
	while(1)
	{
		LcdDisplay(Ds18b20ReadTemp());
//		Delay1ms(1000);//1s��ˢһ��
	}
}

/*******************************************************************************
* ������         : LcdDisplay()
* ��������		   : LCD��ʾ��ȡ�����¶�
* ����           : v
* ���         	 : ��
*******************************************************************************/

void LcdDisplay(int temp) 	 //lcd��ʾ
{
    
  	unsigned char i, datas[] = {0, 0, 0, 0, 0}; //��������
	float tp;  
	if(temp< 0)				//���¶�ֵΪ����
  	{
	  	LcdWriteCom(0x80);		//д��ַ 80��ʾ��ʼ��ַ
		SBUF='-';//�����յ������ݷ��뵽���ͼĴ���
		while(!TI);			         //�ȴ������������
		TI=0;						 //���������ɱ�־λ
	    LcdWriteData('-');  		//��ʾ��
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//����?.5��������С������档
 
  	}
 	else
  	{			
	  	LcdWriteCom(0x80);		//д��ַ 80��ʾ��ʼ��ַ
	    LcdWriteData('+'); 		//��ʾ��
		SBUF='+';//�����յ������ݷ��뵽���ͼĴ���
		while(!TI);			         //�ȴ������������
		TI=0;						 //���������ɱ�־λ
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;

	LcdWriteCom(0x82);		  //д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[0]); //��λ 
	SBUF = '0'+datas[0];//�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			         //�ȴ������������
	TI = 0;
	
	LcdWriteCom(0x83);		 //д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[1]); //ʮλ
	SBUF = '0'+datas[1];//�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			         //�ȴ������������
	TI = 0;

	LcdWriteCom(0x84);		//д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[2]); //��λ 
	SBUF = '0'+datas[2];//�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			         //�ȴ������������
	TI = 0;

	LcdWriteCom(0x85);		//д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('.'); 		//��ʾ ��.��
	SBUF = '.';//�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			         //�ȴ������������
	TI = 0;

	LcdWriteCom(0x86);		 //д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[3]); //��ʾС����  
	SBUF = '0'+datas[3];//�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			         //�ȴ������������
	TI = 0;

	LcdWriteCom(0x87);		 //д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('0'+datas[4]); //��ʾС���� 
	SBUF = '0'+datas[4];//�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			         //�ȴ������������
	TI = 0;
	for(i=0; i<6; i++)
	{
	 	SBUF = CNCHAR[i];//�����յ������ݷ��뵽���ͼĴ���
		while (!TI);			         //�ȴ������������
		TI = 0;
	}

	 
}
/*******************************************************************************
* �� �� ��         :UsartConfiguration()
* ��������		   :���ô���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void UsartConfiguration()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XF3;				//��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XF3;
//	ES=1;						//�򿪽����ж�
//	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
}