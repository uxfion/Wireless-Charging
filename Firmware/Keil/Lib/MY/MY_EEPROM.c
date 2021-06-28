#include "MY_eeprom.h"

//内部函数
static void IAP_close()
{
	IAP_CONTR &= ~IAPEN;	//禁止eeprom操作
	IAP_CMD=0;
	IAP_TRIG=0;
	IAP_ADDRH=0x80;	//将地址设置到非IAP区域
	IAP_ADDRL=0;
}

/******eeprom初始化函数*******/
void eeprom_init()
{
	IAP_CONTR &=~SWBS;	//软件复位后从用户代码开始执行
	IAP_CONTR &=~SWRST;	//无软件复位
	IAP_CONTR |= 0x02;	//设置eeprom操作时间
	IAP_close();
}

/*eeprom写一个字节函数
@变量1	u8 *dat			要保存的数据的地址
@变量2	u16 address		eeprom地址
*/
void eeprom_write_8bit(u8 *dat,u16 address)
{
	IAP_CONTR |= IAPEN;	//使能eeprom操作
	IAP_DATA=*dat;
	IAP_ADDRH=(u8)(address>>8);	//设置地址
	IAP_ADDRL=(u8)(address);
	IAP_CMD=0x02;	//写命令
	IAP_TRIG=0x5A;	//触发
	IAP_TRIG=0xA5;
	_nop_();
	IAP_close();
}

/*eeprom读取一个字节函数
@变量1	u16 address		eeprom地址
*/
u8 eeprom_read_8bit(u16 address)
{
	u8 dat;
	IAP_CONTR |= IAPEN;	//使能eeprom操作
	IAP_ADDRH=(u8)(address>>8);	//设置地址
	IAP_ADDRL=(u8)(address);
	IAP_CMD=0x01;	//读命令
	IAP_TRIG=0x5A;	//触发
	IAP_TRIG=0xA5;
	_nop_();
	dat=IAP_DATA;
	IAP_close();
	return dat;
}

/*eeprom擦除一个扇区（512 byte）函数
@变量1	u16 address		eeprom地址
*/
void eeprom_delete_512byte(u16 address)
{
	IAP_CONTR |= IAPEN;	//使能eeprom操作
	IAP_ADDRH=(u8)(address>>8);	//设置地址
	IAP_ADDRL=(u8)(address);
	IAP_CMD=0x03;	//擦除命令
	IAP_TRIG=0x5A;	//触发
	IAP_TRIG=0xA5;
	_nop_();
	IAP_close();
}

/*eeprom写一个字节函数
@变量1	u8 pagenum		eeprom页数(0 1 2)
@变量2	u8 *dat			要保存的数据的地址
@变量3	u8 dat_size		数据的大小（b）
@变量4	u8 num			要写入的数的个数
*/
void eeprom_write_8bit_word(u8 pagenum,u8 *dat,u8 offset,u8 num)
{
	u8 i;
	eeprom_delete_512byte(pagenum*EEPROM_PAGE_SIZE);
	IAP_CONTR |= IAPEN;	//使能eeprom操作
	
	for(i=0;i<num;i++)
	{	
		IAP_DATA=dat[i];
		IAP_ADDRH=(u8)((pagenum*EEPROM_PAGE_SIZE+i*offset)>>8);	//设置地址
		IAP_ADDRL=(u8)(pagenum*EEPROM_PAGE_SIZE+i*offset);
		IAP_CMD=0x02;	//写命令
		IAP_TRIG=0x5A;	//触发
		IAP_TRIG=0xA5;
		_nop_();
	}
	IAP_close();
}

/*eeprom读一个字节函数
@变量1	u8 pagenum		eeprom页数
@变量2	u16 offset		当前页的偏移地址（0,(512)）
*/
u8 eeprom_read_8bit_word(u8 pagenum,u16 offset)
{
	return eeprom_read_8bit(pagenum*EEPROM_PAGE_SIZE+offset);
}





