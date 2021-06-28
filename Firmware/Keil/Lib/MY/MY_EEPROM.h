#ifndef MY_EEPROM_H
#define MY_EEPROM_H

#include "STC8.h"
#include "common.h"
#include "intrins.h"

#define BASE_ADDRESS		0x0000
#define EEPROM_SIZE			0x0600		//次处设置EEPROM大小为3页  即设置为1.5k
#define EEPROM_PAGE_SIZE 	(512)		

void eeprom_init();
u8 eeprom_read_8bit(u16 address);
void eeprom_write_8bit(u8 *dat,u16 address);
void eeprom_delete_512byte(u16 address);
u8 eeprom_read_8bit_word(u8 pagenum,u16 offset);
void eeprom_write_8bit_word(u8 pagenum,u8 *dat,u8 offset,u8 num);


#endif