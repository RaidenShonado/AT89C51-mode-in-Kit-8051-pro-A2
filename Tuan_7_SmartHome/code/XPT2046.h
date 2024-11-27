#ifndef	  __XPT2046_H_
#define   __XPT2046_H_

#include<reg51.h>
#include<intrins.h>


#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

#ifndef ulong
#define ulong  unsigned long
#endif

sbit DOUT = P3^7;	  
sbit CLK  = P3^6;	  
sbit DIN  = P3^4;	  
sbit CS   = P3^5;

uint Read_AD_Data(uchar cmd);
uint SPI_Read(void);
void SPI_Write(uchar dat);

#endif





