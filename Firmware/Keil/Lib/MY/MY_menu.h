/****************************************************************************
 * @文件       menu.h
 * @作者       刘力铭
 * @功能       菜单，用于调整参数
 * @注意       该程序使用外部显示器(OLED)，五个按键(对应上下左右确认),
               外部存储器(EEPROM)和一个蜂鸣器(可关闭)，移植时请注意
 * @时间       2019-12-17
****************************************************************************/
#ifndef __SELFBUILD_MENU_H
#define __SELFBUILD_MENU_H
 
#include "headfile.h"
#include "SELFBUILD_oled.h"

/***************** 主要宏定义 *****************/
#define MENU_ENABLE_WARNPAGE    1           //这个宏 写0关闭警告界面 写1开启
#define MENU_ENABLE_BUZZER      0           //这个宏 写0关闭蜂鸣器 写1开启蜂鸣器
#define MENU_UNITS_NUM          13          //菜单单元的最大个数
#define MENU_UNIT_VARIABLES_NUM 4           //每个菜单单元中能控制的变量的最大个数
#define MENU_VARIABLE_BIT       15          //每个变量在存储器中存储的位数(可修改)
#define MENU_VARIABLES_NUM      (MENU_STORAGE_SIZE/MENU_VARIABLE_BIT) //菜单最大可以存储的变量数

/***************** 次要宏定义 *****************/
#define MENU_STORAGE_PRODATA    (EEPROM_PAGE_COUNT - 10)                 //菜单保存数据在存储器中存储的位置(可修改)
#define MENU_STORAGE_RAWDATA    (EEPROM_PAGE_COUNT - 11)                 //菜单原始数据在存储器中存储的位置(可修改)
#define MENU_STORAGE_SIZE       (EEPROM_PAGE_SIZE * 8)                   //存储器的存储总位数(可修改)
#define MENU_STORE_BIT          32                                       //菜单的变量压缩后在存储器存储所占的位数
#define MENU_STORE_NUM          (MENU_STORAGE_SIZE/MENU_STORE_BIT)       //菜单的变量压缩后在存储器中存储的数量

/****************** 引脚选择 ******************/
#define MENU_BUTTON_UP          B13         //上按键
#define MENU_BUTTON_DOWN        B26         //下按键
#define MENU_BUTTON_LEFT        B12         //左按键
#define MENU_BUTTON_RIGHT       B28         //右按键
#define MENU_BUTTON_CONFIRM     B27         //确认按键
#if MENU_ENABLE_BUZZER
#define MENU_BUZZER             D0          //蜂鸣器,若MENU_ENABLE_BUZZER为0,可以不设置
#endif

//------------------------------------------------------------------------------------
//                              * 菜单保存变量值的问题 *                              
//  菜单使用外部存储器保存数据,所以当用户改变变量值并保存后,该修改的数据在单片机重启后依然保
//  持修改的值.但是如果用户通过编程改变了菜单中某几个变量的值或者增加/删除菜单中的某几个变量
//  菜单会察觉到这一修改并且做出反应,该反应根据MENU_ENABLE_WARNPAGE不同而不同,若为1,则菜单
//  会跳出警告界面询问用户,根据用户的不同选择使用新的变量值或者使用保存在FLASH中的变量值; 若
//  为0,则菜单默认直接用新的变量值覆盖原值并且覆盖存储器,不再询问.更多详细信息请见menu.c中的
//  * 菜单的储存原理 *
//------------------------------------------------------------------------------------

typedef enum
{
	VariableType_Void,       //无效类型
	VariableType_Int8,       //有符号 整型 8位
	VariableType_Uint8,      //无符号 整型 8位
	VariableType_Int16,      //有符号 整型 16位
	VariableType_Uint16,     //无符号 整型 16位
	VariableType_Int32,      //有符号 整型 32位
	VariableType_Uint32,     //无符号 整型 32位
	VariableType_Float,      //浮点数
	VariableType_Double,     //双精度浮点数
	VariableType_SupportNum, //不允许使用
}VariableTypeDef;

typedef struct
{
	const char*       UintTitle;                             //显示的名字
	void*             VariableAddr[MENU_UNIT_VARIABLES_NUM]; //变量数据地址
	VariableTypeDef   VariableType[MENU_UNIT_VARIABLES_NUM]; //变量数据类型
	const char*       VariableName[MENU_UNIT_VARIABLES_NUM]; //变量显示名字
}Menu_Unit;

typedef Menu_Unit Menu[MENU_UNITS_NUM]; //用户用Menu定义自己的菜单结构体,语法: Menu myMenu; //等价于Menu_Unit[MENU_UNIT_VARIABLES_NUM] myMenu;

void Menu_Init(Menu menu); //在调用该函数前,请确保外部资源(GPIO,外部存储器,外部显示器)的初始化
uint8 Menu_Work(void); //建议每隔200ms执行一次该函数,如:while(!Menu_Work()) systick_delay_ms(200);

/***************************************
//菜单函数调用示范

//假定需要菜单控制的变量
int16 int16_1 = 0, int16_2 = 0;
uint8 uint8_1 = 0;
float float_1 = 0;

void Menu(void)
{
	Menu menu = {0}; //声明菜单结构体变量
	
	//设置菜单结构体
	menu[0].UintTitle = "UintName";               //菜单的名字
	menu[0].VariableAddr[0] = &int16_1;           //变量的地址
	menu[0].VariableName[0] = "int16_1";          //变量的显示名字
	menu[0].VariableType[0] = VariableType_Int16; //变量的类型,注意,如果类型选择错误会死机
	menu[0].VariableAddr[1] = &uint8_1;
	menu[0].VariableName[1] = "uint8_1";
	menu[0].VariableType[1] = VariableType_Uint8;
	menu[0].VariableAddr[2] = &int16_2;
	menu[0].VariableName[2] = "int16_2";
	menu[0].VariableType[2] = VariableType_Int16;
	menu[0].VariableAddr[3] = &float_1;
	menu[0].VariableName[3] = "float_1";
	menu[0].VariableType[3] = VariableType_Float;
	//menu数组范围是0~(MENU_UNITS_NUM-1);menu[].VariableXxxx数组的选择范围是0~(MENU_UNIT_VARIABLES_NUM-1)
	//菜单最多可以控制的变量数见宏定义MENU_VARIABLES_NUM
	
	Menu_Init(menu);                              //初始化菜单,在调用该函数前,请确保外部资源(GPIO,外部存储器,外部显示器)的初始化
	while(!Menu_Work()) systick_delay_ms(200);    //菜单每200ms工作一次，并根据是否按下“关闭菜单”选项后（函数返回0）结束死循环
}
***************************************/

#endif
