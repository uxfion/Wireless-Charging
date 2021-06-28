/****************************************************************************
 * @文件       menu.c
 * @作者       刘力铭
 * @功能       菜单，用于调整参数
 * @注意       该程序使用外部显示器(OLED)，五个按键(对应上下左右确认),
               外部存储器(EEPROM)和一个蜂鸣器(可关闭)，移植时请注意
 * @完成时间   2019-12-17
****************************************************************************/
#include "SELFBUILD_menu.h"

/**************** 内部结构体,枚举体定义 ****************/

typedef struct
{
	int16 AdjustValue[4];
	int16 AdjustMax;
	int16 AdjustMin;
}AdjustDef;

typedef enum
{
	ButtonFlag_Void = 0x00,
	ButtonFlag_Up = 0x01,
	ButtonFlag_Down = 0x02,
	ButtonFlag_Left = 0x04,
	ButtonFlag_Right = 0x08,
	ButtonFlag_Udlr = ButtonFlag_Up | ButtonFlag_Down | ButtonFlag_Left | ButtonFlag_Right, //有上下左右按键按下
	ButtonFlag_Confirm = 0x10,
	ButtonFlag_Pressed = ButtonFlag_Up | ButtonFlag_Down | ButtonFlag_Left | ButtonFlag_Right | ButtonFlag_Confirm, //有任何按键按下
	ButtonFlag_SerialPressed = 0x20, //连按
}ButtonFlagDef;

/**** 菜单的内部函数,依赖外部资源(GPIO,外部存储器,外部显示器).若他日需要更换硬件,可以把这些函数重写,只要保证函数功能不变,菜单功能依然不变 ****/

static void __showWarnPage(const char* str, uint8 isUpdateAll, int8 cursor);
static void __showHomePage(const Menu_Unit* menu, uint8 isUpdateAll, int8 cursor);
static void __showUintPage(const Menu_Unit* unit, int16* value, uint8 isUpdateAll, int8 cursor, uint8 confirm);
static void __changePages(void);
static void __setBuzzer(uint8 dat);
static int8 __buttonGet(void);
static void __readDataFromStorage(int32* value, int sector);
static void __writeDataToStorage(int32* value, int sector);

//------------------------------------------------------------------------------------
//                                  * 菜单显示原理 *                                  
//  将显示屏分为1~ MENU_LINE_MAX 行(第0行为大标题),每行显示 MENU_VARIABLE_PERLINE 个变量
//  那么一个显示屏可以分为0~((MENU_LINE_MAX-1）*MENU_VARIABLE_PERLINE-1)序号的位置,每个位
//  置是一个变量和光标的显示位置.为防止重叠,光标和变量会隔开MENU_GAP_CURSOR_VARIABLE个像素.
//  __showWarnPage,__showHomePage,__showUintPage都有一个cursor参数,该参数就是指光标显示
//  的位置.通过MENU_SHOW_CURSOR和MENU_SHOW_ROW可以计算出该光标位置在屏幕上实际的坐标点,
//  而各个变量名字的显示位置则通过他们在菜单结构体上的顺序决定,再通过MENU_SHOW_VARIABLE和
//  MENU_SHOW_ROW计算出该变量位置在屏幕上实际的坐标点.
//------------------------------------------------------------------------------------

#define MENU_VARIABLE_PERLINE           2 //每行的变量数
#define MENU_GAP_CURSOR_VARIABLE        8 //变量显示和光标显示之间的距离
#define MENU_SHOW_CURSOR(x)             (OLED_X_MAX * ((x) % MENU_VARIABLE_PERLINE) / MENU_VARIABLE_PERLINE) //光标显示的横坐标位置
#define MENU_SHOW_VARIABLE(x)           (OLED_X_MAX * ((x) % MENU_VARIABLE_PERLINE) / MENU_VARIABLE_PERLINE + MENU_GAP_CURSOR_VARIABLE) //变量显示的横坐标位置
#define MENU_SHOW_ROW(x)                ((x) / MENU_VARIABLE_PERLINE + 1) //菜单显示的纵坐标
#define MENU_LINE_SAVE_BUTTON           MENU_LINE_MAX - 1 //"SAVE"按键所在的行数
#define MENU_LINE_BACK_BUTTON           MENU_LINE_MAX //"BACK"按键所在的行数
#define MENU_LINE_MAX                   7

//-------------------------------------------------------------------------
//  *依赖外部的显示器的函数
//  @函数   显示警告界面
//  @参数   str:警告字符串
//          isUpdateAll:是否需要更新显示全部界面内容
//                      0 = 为防止闪屏,只更新光标.  1 = 更新显示全部界面内容
//          cursor:光标位置,详细解释见上面*菜单显示原理*
//  @示例显示效果(不一定非要这样):
//   +----------------------------+
//   |            Warn            |
//   | This is a warning example. |
//   | You have broken the rule   |
//   | that...                    |
//   |                            |
//   |  >  YES         NO         |
//   +----------------------------+
//-------------------------------------------------------------------------
static void __showWarnPage(const char* str, uint8 isUpdateAll, int8 cursor)
{
	static int8 cursorLast = 0;
	
	if(isUpdateAll)
	{
		OLED_P6x8Str((OLED_X_MAX - 1 - 6 * (sizeof("Warn") - 1)) / 2, 0, "Warn");//显示界面
		
		OLED_P6x8Str(MENU_SHOW_VARIABLE(0), 1, str);
		
		OLED_P6x8Str(MENU_SHOW_VARIABLE(0), MENU_LINE_MAX, " YES");
		OLED_P6x8Str(MENU_SHOW_VARIABLE(1), MENU_LINE_MAX, " NO");
	}
	
	OLED_P6x8Str(MENU_SHOW_CURSOR(cursorLast), MENU_SHOW_ROW(cursorLast), " "); //清除光标
	OLED_P6x8Str(MENU_SHOW_CURSOR(cursor), MENU_SHOW_ROW(cursor), ">"); //显示新光标
	cursorLast = cursor;
}

//-------------------------------------------------------------------------
//  *依赖外部的显示器的函数
//  @函数   显示菜单主界面
//  @参数   menu:菜单结构体指针
//          isUpdateAll:是否需要更新显示全部界面内容
//                      0 = 为防止闪屏,只更新光标.  1 = 更新显示全部界面内容
//          cursor:光标位置,详细解释见上面*菜单显示原理*
//  @示例显示效果(不一定非要这样):
//   +----------------------------+
//   |            Menu            |
//   |> CloseMenu     Variable_1  |
//   |  Variable_2    Variable_3  |
//   |  Variable_4    Variable_5  |
//   |  Variable_6    Variable_7  |
//   |  Variable_8    Variable_9  |
//   +----------------------------+
//-------------------------------------------------------------------------
static void __showHomePage(const Menu_Unit* menu, uint8 isUpdateAll, int8 cursor)
{
	static int8 cursorLast = 0;
	
	if(isUpdateAll)
	{
		uint8 temp;
		OLED_ClearScreen(OLED_BLACK);//清屏
		OLED_P6x8Str((OLED_X_MAX - 1 - 6 * (sizeof("Menu") - 1)) / 2, 0, "Menu");//显示界面
		OLED_P6x8Str(MENU_SHOW_VARIABLE(0), MENU_SHOW_ROW(0), "CloseMenu");
		
		for(temp = 0; temp < MENU_UNITS_NUM; ++temp)
		{
			if(menu[temp].UintTitle != 0)
			{
				OLED_P6x8Str(MENU_SHOW_VARIABLE(temp + 1), MENU_SHOW_ROW(temp + 1), (char*)menu[temp].UintTitle);
			}
		}
	}
	
	OLED_P6x8Str(MENU_SHOW_CURSOR(cursorLast), MENU_SHOW_ROW(cursorLast), " "); //清除光标
	OLED_P6x8Str(MENU_SHOW_CURSOR(cursor), MENU_SHOW_ROW(cursor), ">"); //显示新光标
	cursorLast = cursor;
}

//-------------------------------------------------------------------------
//  *依赖外部的显示器的函数
//  @函数   显示菜单单元界面
//  @参数   menu:菜单结构体指针
//          value:各个变量的值
//          isUpdateAll:是否需要更新显示全部界面内容
//                      0 = 为防止闪屏,只更新光标.  1 = 更新显示全部界面内容
//          cursor:光标位置,详细解释见上面*菜单显示原理*
//          confirm:若为1说明按下确认按键,需要更改光标的样式
//  @示例显示效果(不一定非要这样):
//   +----------------------------+
//   |            Menu            |
//   |* Variable_1       value1   |
//   |  Variable_2       value2   |
//   |  Variable_3       value3   |
//   |  SAVE                      |
//   |  BACK                      |
//   +----------------------------+
//-------------------------------------------------------------------------
static void __showUintPage(const Menu_Unit* unit, int16* value, uint8 isUpdateAll, int8 cursor, uint8 confirm)
{
	static int8 cursorLast = 0;
	
	if(isUpdateAll)
	{
		uint8 number;
		OLED_ClearScreen(OLED_BLACK);//清屏
		for(number = 0; unit->UintTitle[number] != 0; ++number);//显示界面
		OLED_P6x8Str((OLED_X_MAX - 1 - 6 * number) / 2, 0, (char*)unit->UintTitle);
		
		OLED_P6x8Str(MENU_SHOW_VARIABLE(0), MENU_LINE_SAVE_BUTTON, "SAVE");
		OLED_P6x8Str(MENU_SHOW_VARIABLE(0), MENU_LINE_BACK_BUTTON, "BACK");
	}
	
	for(int temp = 0; temp < MENU_UNIT_VARIABLES_NUM; ++temp)
	{
		if(unit->VariableType[temp] == VariableType_Void) continue;
		OLED_P6x8Str(MENU_SHOW_VARIABLE(0), temp + 1, (char*)unit->VariableName[temp]);
		
		if(unit->VariableType[temp] == VariableType_Int8 || unit->VariableType[temp] == VariableType_Int16 || unit->VariableType[temp] == VariableType_Int32)
		{
			OLED_P6x8Int(OLED_X_MAX / 2, temp + 1, value[temp], -3);
		}
		else if(unit->VariableType[temp] == VariableType_Uint8 || unit->VariableType[temp] == VariableType_Uint16 || unit->VariableType[temp] == VariableType_Uint32)
		{
			OLED_P6x8Int(OLED_X_MAX / 2, temp + 1, value[temp], 4);
		}
		else if(unit->VariableType[temp] == VariableType_Float || unit->VariableType[temp] == VariableType_Double)
		{
			if(value[temp] >= 0)
			{
				OLED_P6x8Flo(OLED_X_MAX / 2, temp + 1, (double)value[temp] / 100.0 + 0.000001, -2);
			}
			else
			{
				OLED_P6x8Flo(OLED_X_MAX / 2, temp + 1, (double)value[temp] / 100.0 - 0.000001, -2);
			}
		}
	}
	
	OLED_P6x8Str(MENU_SHOW_CURSOR(cursorLast), MENU_SHOW_ROW(cursorLast), " ");
	if(confirm == 1)
		OLED_P6x8Str(MENU_SHOW_CURSOR(cursor), MENU_SHOW_ROW(cursor), "*");
	else OLED_P6x8Str(MENU_SHOW_CURSOR(cursor), MENU_SHOW_ROW(cursor), ">");
	cursorLast = cursor;
}

//-------------------------------------------------------------------------
//  *依赖外部的显示器和GPIO的函数
//  @函数   用于界面切换时,清屏并且等待按键放开
//-------------------------------------------------------------------------
static void __changePages(void)
{
	while(gpio_get(MENU_BUTTON_CONFIRM) == 0);
	OLED_ClearScreen(OLED_BLACK);
}

//-------------------------------------------------------------------------
//  *依赖GPIO的函数
//  @函数   设置蜂鸣器
//  @参数   data:设置值
//-------------------------------------------------------------------------
static void __setBuzzer(uint8 dat)
{
#if MENU_ENABLE_BUZZER == 1 //如果不开启蜂鸣器,那么把这个函数设置为空函数
	gpio_set(MENU_BUZZER, dat);
#endif
}

//-------------------------------------------------------------------------
//  *依赖GPIO的函数
//  @函数   检测按键
//  @返回   按键值
//-------------------------------------------------------------------------
static int8 __buttonGet(void)
{
	int8 return_flag = ButtonFlag_Void;
	
	if(gpio_get(MENU_BUTTON_UP) == 0)
		return_flag |= ButtonFlag_Up;
	if(gpio_get(MENU_BUTTON_DOWN) == 0)
		return_flag |= ButtonFlag_Down;
	if(gpio_get(MENU_BUTTON_LEFT) == 0)
		return_flag |= ButtonFlag_Left;
	if(gpio_get(MENU_BUTTON_RIGHT) == 0)
		return_flag |= ButtonFlag_Right;
	if(gpio_get(MENU_BUTTON_CONFIRM) == 0)
		return_flag |= ButtonFlag_Confirm;
	
	return return_flag;
}

//-------------------------------------------------------------------------------
//                               * 菜单的储存原理 *                               
//  了解一下：
//  虽然菜单的可以控制所有不同类型的变量,事实上,在菜单修改数据,保存数据,读取数据的过程
//  中,都是把变量转换为int16(即short int)来操作的.因为在菜单调试的过程中,整形变的量调
//  整范围为0~9999或-999~999,浮点数的调整范围为-99.99~99.99;我们把浮点数乘100后变为
//  整形变量.那么所有变量的最大调整范围为-9999~9999该调整范围可以用一个15位的二级制存
//  储.在存储时,菜单将int16的变量取出有用的15位,再整合到int32变量中用于保存.
//
//  了解一下：
//  菜单需要存储两种数据,分别储存在不同的地方,且都需要占用MENU_STORAGE_SIZE空间.
//  1.一种是菜单在初始化的时候,会将所有的变量值全部整合并且压缩随后存到存储器中,这些数
//  据为"Raw数据",意思是原数据.每一次系统重新启动后,菜单会把该保存的数据和当前程序中菜
//  单结构体的数据做对比,由此来判断用户是否在程序上对菜单中的变量做出了修改.
//  2.另一种数据是在菜单在工作的时候,使用菜单的用户利用菜单修改了变量的值并SAVE,菜单会
//  把这些变量值同样压缩保存到存储器中,这些数据为"Pro数据",意思是处理后的数据.下一次系
//  统重新启动后,若用户没有对菜单中的变量做出修改,那么默认使用该数据.
//
//  如果要移植菜单的程序：要做的只是如何把这32位的变量存储到存储器中
//  在__writeDataToStorage函数和__readDataFromStorage函数都有sector参数,根据该参数
//  为0或者1时,函数需要实现从存储器中对应读出/写入"Raw数据"或者"Pro数据".
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------
//  *依赖外部存储器的函数
//  @函数   把变量压缩后保存到储存器中
//  @参数   value:一个外部声明的int32[MENU_STORE_NUM]数组,待保存数据
//          sector:不同的数据存储在不同的存储器 1 = Pro数据, 0 = Raw数据,详细
//          解释见上面*菜单的储存原理*
//-------------------------------------------------------------------------
static void __writeDataToStorage(int32* value, int sector)
{
	int save = (sector == 0 ? MENU_STORAGE_RAWDATA : MENU_STORAGE_PRODATA);
	
	for(int temp = 0; temp < MENU_STORE_NUM; ++temp)
	{
		eeprom_write_word(save, temp, (uint32)value[temp]);
	}
}

//-------------------------------------------------------------------------
//  *依赖外部存储器的函数
//  @函数   从储存器中读出保存的压缩变量
//  @参数   value:一个外部声明的int32[MENU_STORE_NUM]数组,用来保存数据
//          sector:不同的数据存储在不同的存储器 1 = Pro数据, 0 = Raw数据,详细
//          解释见上面*菜单的储存原理*
//-------------------------------------------------------------------------
static void __readDataFromStorage(int32* value, int sector)
{
	int save = (sector == 0 ? MENU_STORAGE_RAWDATA : MENU_STORAGE_PRODATA);
	
	for(int temp = 0; temp < MENU_STORE_NUM; ++temp)
	{ 
		value[temp] = eeprom_read_word(save, temp, int32);
	}
}

/**************** 菜单的内部算法函数,不依赖外部资源,若不更改算法，无需更改 ****************/

static int menuAbs(int temp) { return temp > 0 ? temp : -temp; }
static uint8 buttonGet(void);
static void compressVariable(int16* srcVariable, int32* desVariable);
static void decompressVariable(int32* srcVariable, int16* desVariable);
static void readAllVariablesFormStorage(int16* value, int sector);
static int16 readSingleVariableFromAddress(void* addr, VariableTypeDef type);
static void writeAllVariablesToStorage(int16* value, int sector);
static void writeAllVariablesToAddress(Menu_Unit* menu, int16* value);
static void writeSingleVariableToAddress(void* addr, VariableTypeDef type, int16 value);
static void collectVariablesValue(Menu_Unit* menu, int16* value);
static uint8 checkIfVariablesChanged(Menu_Unit* menu);
static void coverVariablesToAddressOrStorage(Menu_Unit* menu, uint8 direction);
static uint8 homePageHandle(uint8 buttonFlag);
static uint8 warnPageHandle(uint8 buttonFlag);
static uint8 unitPageHandle(uint8 buttonFlag);

//-------------------------------------------------------------------------
//  @函数   检测按键并判断连按
//  @返回   按键值
//-------------------------------------------------------------------------
static uint8 buttonGet(void)
{
	static uint8 buttonFlagLast = ButtonFlag_Void;
	static uint16 serialPressed = 0;
	uint8 buttonFlag = __buttonGet();
	
	if((buttonFlag & ButtonFlag_Udlr) != ButtonFlag_Void && (buttonFlag & ButtonFlag_Udlr) == (buttonFlagLast & ButtonFlag_Udlr))
	{
		serialPressed++;
		if(serialPressed >= 5) buttonFlag |= ButtonFlag_SerialPressed;
	}
	else serialPressed = 0;
	
	buttonFlagLast = buttonFlag;
	return buttonFlag;
}

//-------------------------------------------------------------------------
//  @函数   将MENU_VARIABLE_BIT位的数据压缩为MENU_STORE_BIT位的数据
//  @参数   srcVariable:一个外部声明的int16[MENU_VARIABLES_NUM]数组,待解压数据
//          desVariable:一个外部声明的int32[MENU_STORE_NUM]数组,保存数据
//-------------------------------------------------------------------------
static void compressVariable(int16 srcVariable[MENU_VARIABLES_NUM], int32 desVariable[MENU_STORE_NUM])
{
	//第一位为符号位，后面其他为数据位
	int32 signMask = 0x01 << (MENU_VARIABLE_BIT - 1);	//signMask为符号位掩码 
	int32 valueMask = 0;	//valueMask为数据位掩码 
	int16 offset = 0, index = 0;
	
	for(int16 temp = 0; temp < MENU_VARIABLE_BIT - 1; ++temp) 
	{
		valueMask <<= 1;
		valueMask |= 0x01; 
	}
	int32 wholeMask = valueMask | signMask;
	
	for(int16 temp = 0; temp < MENU_VARIABLES_NUM; ++temp) 
	{
		int32 compressedValue = menuAbs(srcVariable[temp]) & valueMask; //取出数据位 
		if(srcVariable[temp] < 0)  compressedValue |= signMask; //取出符号位 
		
		if(offset + MENU_VARIABLE_BIT < MENU_STORE_BIT) //如果没有溢出 
		{
			desVariable[index] |= compressedValue << offset; //将 MENU_VARIABLE_BIT 位的数据压缩放到32位的变量中 
			offset += MENU_VARIABLE_BIT;
		}
		else if(offset + MENU_VARIABLE_BIT == MENU_STORE_BIT)
		{
			desVariable[index] |= compressedValue << offset; //将 MENU_VARIABLE_BIT 位的数据压缩放到32位的变量中 
			offset = 0;
			++index;
		}
		else //如果溢出，把 MENU_VARIABLE_BIT 位变量分割后分别存储在不同的32位数据中 surplus|remains
		{
			int16 surplus = offset + MENU_VARIABLE_BIT - MENU_STORE_BIT; //计算surplus的位数
			int16 remains = MENU_VARIABLE_BIT - surplus;                 //计算remains的位数
			int32 wholeRemainsMask = (uint32)wholeMask >> surplus;       //计算remains数据掩码
			int32 wholeSurplusMask = (uint32)wholeMask >> remains;       //计算surplus数据掩码
			int32 compressedRemainsValue = compressedValue & wholeRemainsMask; //计算remains部分数据
			int32 compressedSurplusValue = ((uint32)compressedValue >> remains) & wholeSurplusMask; //计算surplus部分数据
			
			desVariable[index] |= 	compressedRemainsValue << offset; //将remains部分放到32位的变量中
			desVariable[++index] |= compressedSurplusValue; //将surplus部分放到下一个32位的变量中
			offset = surplus;
		} 
	}
}

//-------------------------------------------------------------------------
//  @函数   将MENU_STORE_BIT位的数据解压为MENU_VARIABLE_BIT位的数据
//  @参数   srcVariable:一个外部声明的int32[MENU_STORE_NUM]数组,待解压数据
//          desVariable:一个外部声明的int16[MENU_VARIABLES_NUM]数组,保存数据
//-------------------------------------------------------------------------
static void decompressVariable(int32 srcVariable[MENU_STORE_NUM], int16 desVariable[MENU_VARIABLES_NUM])
{
	//第一位为符号位，后面其他为数据位
	int32 signMask = 0x01 << (MENU_VARIABLE_BIT - 1);	//signMask为符号位掩码 
	int32 valueMask = 0;	//valueMask为数据位掩码 
	int16 offset = 0, index = 0;
	
	for(int16 temp = 0; temp < MENU_VARIABLE_BIT - 1; ++temp) 
	{
		valueMask <<= 1;
		valueMask |= 0x01; 
	}
	int32 wholeMask = valueMask | signMask;
	
	for(int16 temp = 0; temp < MENU_VARIABLES_NUM; ++temp)
	{
		int32 decompressedValue = 0;
		
		if(offset + MENU_VARIABLE_BIT < MENU_STORE_BIT) //如果没有溢出 
		{
			decompressedValue = ((uint32)srcVariable[index] >> offset) & wholeMask; //将32位的变量解压放到 MENU_VARIABLE_BIT 位的数据中 
			offset += MENU_VARIABLE_BIT;
		}
		else if(offset + MENU_VARIABLE_BIT == MENU_STORE_BIT)
		{
			decompressedValue = ((uint32)srcVariable[index] >> offset) & wholeMask; //将32位的变量解压放到 MENU_VARIABLE_BIT 位的数据中
			offset = 0;
			++index;
		}
		else //如果溢出，把分别存储在不同的32位数据整合到 MENU_VARIABLE_BIT 位变量 surplus|remains
		{
			int16 surplus = offset + MENU_VARIABLE_BIT - MENU_STORE_BIT; //计算surplus的位数
			int16 remains = MENU_VARIABLE_BIT - surplus;                 //计算remains的位数
			int32 wholeRemainsMask = (uint32)wholeMask >> surplus;       //计算remains数据掩码
			int32 wholeSurplusMask = (uint32)wholeMask >> remains;       //计算surplus数据掩码
			
			int32 decompressedRemainsValue = ((uint32)srcVariable[index] >> offset) & wholeRemainsMask; //将remains部分从32位的变量取出 
			int32 decompressedSurplusValue =  srcVariable[++index] & wholeSurplusMask; //将surplus部分从下一个32位的变量取出 
			offset = surplus;
			decompressedValue = decompressedSurplusValue << remains | decompressedRemainsValue; //组合数据 
		}
		
		desVariable[temp] = decompressedValue & valueMask; 
		if(decompressedValue & signMask) desVariable[temp] *= -1;
	}
}

//-------------------------------------------------------------------------
//  @函数   从内存地址中读出一个数据
//  @参数   addr:变量的地址
//          type:变量的类型
//  @返回   该数据的值
//-------------------------------------------------------------------------
static int16 readSingleVariableFromAddress(void* addr, VariableTypeDef type)
{
	int16 value = 0;
	
	if(type == VariableType_Int8)
		value = *((int8*)addr);
	else if(type == VariableType_Uint8)
		value = *((uint8*)addr);
	else if(type == VariableType_Int16)
		value = *((int16*)addr);
	else if(type == VariableType_Uint16)
		value = *((uint16*)addr);
	else if(type == VariableType_Int32)
		value = *((int32*)addr);
	else if(type == VariableType_Uint32)
		value = *((uint32*)addr);
	else if(type == VariableType_Float)
	{
		float temp = *((float*)addr);
		if(temp >= 0) temp += 0.000001f;
		else temp -= 0.000001f;
		value = (int16)(temp * 100);
	}
	else if(type == VariableType_Double)
	{
		float temp = *((double*)addr);
		if(temp >= 0) temp += 0.000001f;
		else temp -= 0.000001f;
		value = (int16)(temp * 100);
	}
	
	return value;
}

//-------------------------------------------------------------------------
//  @函数   从存储器中读出所有被压缩的数据并解压
//  @参数   value:一个外部声明的int16[MENU_VARIABLES_NUM]数组,用来保存数据
//          sector:不同的数据存储在不同的存储器 1 = Pro数据, 0 = Raw数据,详细
//          解释见上面*菜单的储存原理*
//-------------------------------------------------------------------------
static void readAllVariablesFormStorage(int16* value, int sector)
{
	int32 compressedValue[MENU_STORE_NUM] = {0};
	__readDataFromStorage(compressedValue, sector);
	decompressVariable(compressedValue, value);
}

//-------------------------------------------------------------------------
//  @函数   将单个数据保存到对应的菜单变量内存地址中
//  @参数   addr:变量的地址
//          type:变量的类型
//          value:待存储的数据
//-------------------------------------------------------------------------
static void writeSingleVariableToAddress(void* addr, VariableTypeDef type, int16 value)
{
	if(type == VariableType_Int8)
		*((int8*)addr) = value;
	else if(type == VariableType_Uint8)
		*((uint8*)addr) = value;
	else if(type == VariableType_Int16)
		*((int16*)addr) = value;
	else if(type == VariableType_Uint16)
		*((uint16*)addr) = value;
	else if(type == VariableType_Int32)
		*((int32*)addr) = value;
	else if(type == VariableType_Uint32)
		*((uint32*)addr) = value;
	else if(type == VariableType_Float)
		*((float*)addr) = (float)value / 100.0f;
	else if(type == VariableType_Double)
		*((double*)addr) = (double)value / 100.0;
}

//-------------------------------------------------------------------------
//  @函数		将数组中的全部数据保存到对应的菜单变量内存地址中
//  @参数		menu:菜单结构体指针
//          value:一个外部声明的int16[MENU_VARIABLES_NUM]数组,待存储的数据
//-------------------------------------------------------------------------
static void writeAllVariablesToAddress(Menu_Unit* menu, int16* value)
{
	int index = 0;
	for(int alpha = 0; alpha < MENU_UNITS_NUM; ++alpha)
	{
		if(menu[alpha].UintTitle == 0) continue;
		for(int beta = 0; (beta < MENU_UNIT_VARIABLES_NUM); ++beta)
		{
			if(menu[alpha].VariableType[beta] != VariableType_Void)
			{
				writeSingleVariableToAddress(menu[alpha].VariableAddr[beta], menu[alpha].VariableType[beta], value[index]);
				index++;
			}
		}
	}
}

//-------------------------------------------------------------------------
//  @函数		将数组中的全部数据经过压缩后存储到存储器中
//  @参数		value:一个外部声明的int16[MENU_VARIABLES_NUM]数组,待存储的数据
//          sector:不同的数据存储在不同的存储器 1 = Pro数据, 0 = Raw数据,详细
//          解释见上面*菜单的储存原理*
//-------------------------------------------------------------------------
static void writeAllVariablesToStorage(int16* value, int sector)
{
	int32 compressedValue[MENU_STORE_NUM] = {0};
	compressVariable(value, compressedValue);
	__writeDataToStorage(compressedValue, sector);
}

//-------------------------------------------------------------------------
//  @函数		将菜单中所有变量的值全部整合到一个数组中
//  @参数		menu:菜单结构体指针
//          value:一个外部声明的int16[MENU_VARIABLES_NUM]数组,用来保存数据
//-------------------------------------------------------------------------
static void collectVariablesValue(Menu_Unit* menu, int16* value)
{
	int index = 0;
	for(int alpha = 0; alpha < MENU_UNITS_NUM; ++alpha)
	{
		if(menu[alpha].UintTitle == 0) continue;
		for(int beta = 0; (beta < MENU_UNIT_VARIABLES_NUM); ++beta)
		{
			if(menu[alpha].VariableType[beta] != VariableType_Void)
			{
				value[index] = readSingleVariableFromAddress(menu[alpha].VariableAddr[beta], menu[alpha].VariableType[beta]);
				index++;
			}
		}
	}
}

//-------------------------------------------------------------------------
//  @函数		检测菜单的各个变量的值有没有因为编程发生改变
//  @参数		menu:菜单结构体指针
//  @返回		1 = 有变化, 0 = 无变化
//-------------------------------------------------------------------------
static uint8 checkIfVariablesChanged(Menu_Unit* menu)
{
	uint8 ifChanged = 0;
	int16 copyVariableValue[MENU_VARIABLES_NUM] = {0};
	int16 storedRawValue[MENU_VARIABLES_NUM] = {0};
	
	collectVariablesValue(menu, copyVariableValue);
	readAllVariablesFormStorage(storedRawValue, 0);
	
	//对比变量的值,检测是否改变
	for(int temp = 0; temp < MENU_VARIABLES_NUM; ++temp)
	{ 
		if(copyVariableValue[temp] != storedRawValue[temp])
		{
			ifChanged = 1;
			break;
		}
	}
	
	return ifChanged;
}

//-------------------------------------------------------------------------
//  @函数   当checkIfVariablesChanged返回值为1时,将变量的值覆盖到内存地址或
//          者覆盖到储存器
//  @参数   menu:菜单结构体指针
//          direction:1 = 将变量的值覆盖到储存器. 0 = 将变量的值覆盖到内存地址
//-------------------------------------------------------------------------
static void coverVariablesToAddressOrStorage(Menu_Unit* menu, uint8 direction)
{
	int16 storedProValue[MENU_VARIABLES_NUM] = {0}, copyVariableValue[MENU_VARIABLES_NUM] = {0};
	readAllVariablesFormStorage(storedProValue, 1);
	collectVariablesValue(menu, copyVariableValue);
	
	if(direction == 0)
	{
		writeAllVariablesToAddress(menu, storedProValue);
	}
	else
	{
		writeAllVariablesToStorage(copyVariableValue, 0);
		writeAllVariablesToStorage(copyVariableValue, 1);
	}
}

/**************** 菜单的外部函数,全局变量以及重要的界面处理函数 ****************/

//菜单用到的全局变量
static int8 homePageCursor = 0, unitPageCursor = 0;
static uint8(*pageHandle)(uint8);
static Menu_Unit* menuPoint;
static AdjustDef Adjust[VariableType_SupportNum] = {
//可以通过调整该结构体的各个参数达到直接调整菜单每次按上下左右键后对数据的修改值

// 上   下   左   右   最大值   最小值
	 0,   0,   0,   0,       0,      0,   //void
	 5,  -5,  -1,   1,     127,   -128,		//int8
	 5,  -5,  -1,   1,     255,   -000,		//uint8
	 5,  -5,  -1,   1,     999,   -999,		//int16
	 5,  -5,  -1,   1,    9999,  -0000,		//uint16
	 5,  -5,  -1,   1,     999,   -999,		//int32
	 5,  -5,  -1,   1,    9999,  -0000,		//uint32
	10, -10,  -1,   1,    9999,  -9999,		//float(实际调整值会除100)
	10, -10,  -1,   1,    9999,  -9999,		//double(实际调整值会除100)
};

//-------------------------------------------------------------------------
//  @函数   菜单初始化函数
//  @参数   menu:菜单初始化结构体
//  @注意   在调用该函数前,请确保外部资源(GPIO,外部存储器,外部显示器)的初始化
//-------------------------------------------------------------------------
void Menu_Init(Menu menu)
{
	uint8 ifChanged = 0; 
	menuPoint = menu;
	
	ifChanged = checkIfVariablesChanged(menu);
		
#if MENU_ENABLE_WARNPAGE //如果ENABLE警告页
	if(ifChanged == 0) //如果程序没有对变量值进行修改,则默认使用保存在存储器中的值作为各变量的值
	{
		coverVariablesToAddressOrStorage(menu, ifChanged);
		pageHandle = &homePageHandle;
	}
	else
	{
		pageHandle = &warnPageHandle;
	}
#else //如果DISABLE警告页
	coverVariablesToAddressOrStorage(menu, ifChanged);
	pageHandle = &warnPageHandle; //这句话没有用,主要是为了消除warnPageHandle没用被调用的警告
	pageHandle = &homePageHandle;
#endif
}

//-------------------------------------------------------------------------
//  @函数   菜单主界面
//  @参数   buttonFlag:按键值
//-------------------------------------------------------------------------
static uint8 homePageHandle(uint8 buttonFlag)
{
	static uint8 isInited = 0;
	uint8 returnFlag = 0;
	
	if(isInited == 0) //界面初始化,只在进入该界面时执行一次,在退出界面后置零
	{
		isInited = 1;
		__showHomePage(menuPoint, 1, 0);
	}
	
	if(buttonFlag & ButtonFlag_Udlr) //在该界面下,上下左右按键控制光标移动
	{
		if(buttonFlag & ButtonFlag_Up) homePageCursor -= MENU_VARIABLE_PERLINE;
		if(buttonFlag & ButtonFlag_Down) homePageCursor += MENU_VARIABLE_PERLINE;
		if(buttonFlag & ButtonFlag_Left) homePageCursor -= 1;
		if(buttonFlag & ButtonFlag_Right) homePageCursor += 1;
		
		if(homePageCursor < 0) homePageCursor += (MENU_UNITS_NUM + 1);
		if(homePageCursor >= (MENU_UNITS_NUM + 1)) homePageCursor -= (MENU_UNITS_NUM + 1);
	}
	
	if(buttonFlag & ButtonFlag_Confirm)
	{
		if(homePageCursor != 0)
		{
			if(menuPoint[homePageCursor - 1].UintTitle != 0) //进入菜单单元界面
			{
				isInited = 0;
				pageHandle = &unitPageHandle;
			}
		}
		else if(homePageCursor == 0) //如果是CloseMenu选项,则退出菜单
		{
			returnFlag = 1;
		}
	}
	
	__showHomePage(menuPoint, 0, homePageCursor); //更新光标
	if(returnFlag) __changePages();
	return returnFlag;
}

//-------------------------------------------------------------------------
//  @函数   菜单单元界面
//  @参数   buttonFlag:按键值
//  @注意   比较复杂,如果看的话,不需要每句话都看懂
//-------------------------------------------------------------------------
static uint8 unitPageHandle(uint8 buttonFlag)
{
	static uint8 confirmFlag = 0;
	static uint8 isInited = 0;
	static int16 tempVariablesValue[MENU_UNIT_VARIABLES_NUM] = {0};
	static Menu_Unit* currentMenuUint;
	
	if(isInited == 0) //界面初始化,只在进入该界面时执行一次,在退出界面后置零
	{
		isInited = 1;
		unitPageCursor = -1;
		currentMenuUint = &menuPoint[homePageCursor - 1]; //当前的菜单单元的指针
		
		for(int temp = 0; temp < MENU_UNIT_VARIABLES_NUM; ++temp)
		{
			tempVariablesValue[temp] = readSingleVariableFromAddress(currentMenuUint->VariableAddr[temp], currentMenuUint->VariableType[temp]);
			if(currentMenuUint->VariableType[temp] != VariableType_Void && unitPageCursor == -1) //让光标指向第一个变量
				unitPageCursor = temp * MENU_VARIABLE_PERLINE;
		}
		if(unitPageCursor == -1)
		{
			unitPageCursor = ((MENU_LINE_SAVE_BUTTON > MENU_LINE_BACK_BUTTON ? MENU_LINE_BACK_BUTTON : MENU_LINE_SAVE_BUTTON) - 1) * MENU_VARIABLE_PERLINE; //如果找不到变量,则指向"BACK"或者"SAVE"
		}
		__showUintPage(currentMenuUint, tempVariablesValue, 1, 0, 0); //显示菜单单元界面
	}
	
	if(buttonFlag & ButtonFlag_Udlr) //如果有上下左右被按下
	{
		if(confirmFlag == 0)
		{ //如果没有进入调整参数状态,则控制光标移动,由于该界面光标可能不会连续移动,所以这个if句子比较复杂,看不懂没关系
			if(buttonFlag & ButtonFlag_Up || buttonFlag & ButtonFlag_Left)
			{
				unitPageCursor -= MENU_VARIABLE_PERLINE;
				if(unitPageCursor < 0) unitPageCursor += MENU_LINE_MAX * MENU_VARIABLE_PERLINE;
				if(unitPageCursor >= MENU_LINE_MAX * MENU_VARIABLE_PERLINE) unitPageCursor -= MENU_LINE_MAX * MENU_VARIABLE_PERLINE;
				
				while(unitPageCursor / MENU_VARIABLE_PERLINE >= MENU_UNIT_VARIABLES_NUM
					&& unitPageCursor / MENU_VARIABLE_PERLINE < (MENU_LINE_BACK_BUTTON > MENU_LINE_SAVE_BUTTON ? MENU_LINE_SAVE_BUTTON : MENU_LINE_BACK_BUTTON) - 1)
					unitPageCursor -= MENU_VARIABLE_PERLINE;
				while(unitPageCursor / MENU_VARIABLE_PERLINE < MENU_UNIT_VARIABLES_NUM
					&& unitPageCursor / MENU_VARIABLE_PERLINE >= 0
					&& currentMenuUint->VariableType[unitPageCursor / 2] == VariableType_Void)
					unitPageCursor -= MENU_VARIABLE_PERLINE;
			}
			if(buttonFlag & ButtonFlag_Down || buttonFlag & ButtonFlag_Right)
			{
				unitPageCursor += MENU_VARIABLE_PERLINE;
				if(unitPageCursor < 0) unitPageCursor += MENU_LINE_MAX * MENU_VARIABLE_PERLINE;
				if(unitPageCursor >= MENU_LINE_MAX * MENU_VARIABLE_PERLINE) unitPageCursor -= MENU_LINE_MAX * MENU_VARIABLE_PERLINE;
				
				while(unitPageCursor / MENU_VARIABLE_PERLINE < MENU_UNIT_VARIABLES_NUM
					&& unitPageCursor / MENU_VARIABLE_PERLINE >= 0
					&& currentMenuUint->VariableType[unitPageCursor / 2] == VariableType_Void)
					unitPageCursor += MENU_VARIABLE_PERLINE;
				while(unitPageCursor / MENU_VARIABLE_PERLINE >= MENU_UNIT_VARIABLES_NUM
					&& unitPageCursor / MENU_VARIABLE_PERLINE < (MENU_LINE_BACK_BUTTON > MENU_LINE_SAVE_BUTTON ? MENU_LINE_SAVE_BUTTON : MENU_LINE_BACK_BUTTON) - 1)
					unitPageCursor += MENU_VARIABLE_PERLINE;
			}
			if(unitPageCursor < 0) unitPageCursor += MENU_LINE_MAX * MENU_VARIABLE_PERLINE;
			if(unitPageCursor >= MENU_LINE_MAX * MENU_VARIABLE_PERLINE) unitPageCursor -= MENU_LINE_MAX * MENU_VARIABLE_PERLINE;
		}
		else //否则,调整数值
		{
			for(int index = 0, temp = buttonFlag; index < 4; ++index) //上下左右按键处理,用了一个for循环来写,比较特殊
			{
				if(temp & 0x01) //调整数值到tempVariablesValue中
				{
					int serialPressed = 1;
					if(buttonFlag & ButtonFlag_SerialPressed) serialPressed = 4; //如果判断为连按,则单次调整的数值翻倍
					VariableTypeDef type = currentMenuUint->VariableType[unitPageCursor / MENU_VARIABLE_PERLINE];
					int16 adjust = Adjust[type].AdjustValue[index]; //根据当前变量的类型,在Adjust结构体中获取单次调整的数值
					tempVariablesValue[unitPageCursor / MENU_VARIABLE_PERLINE] += adjust * serialPressed;
					if(tempVariablesValue[unitPageCursor / MENU_VARIABLE_PERLINE] >= Adjust[type].AdjustMax) tempVariablesValue[unitPageCursor / MENU_VARIABLE_PERLINE] = Adjust[type].AdjustMax;
					else if(tempVariablesValue[unitPageCursor / MENU_VARIABLE_PERLINE] <= Adjust[type].AdjustMin) tempVariablesValue[unitPageCursor / MENU_VARIABLE_PERLINE] = Adjust[type].AdjustMin;
				}
				temp >>= 1;
			}
		}
	}
	
	if(buttonFlag & ButtonFlag_Confirm) //如果按下确定按键
	{
		if(unitPageCursor == (MENU_LINE_BACK_BUTTON - 1) * MENU_VARIABLE_PERLINE || unitPageCursor == (MENU_LINE_SAVE_BUTTON - 1) * MENU_VARIABLE_PERLINE) //如果按下的是SAVE或者BACK的时候
		{
			if(unitPageCursor == (MENU_LINE_SAVE_BUTTON - 1) * MENU_VARIABLE_PERLINE) //如果光标指向SAVE
			{
				for(int temp = 0; temp < MENU_UNIT_VARIABLES_NUM; ++temp) //将tempVariablesValue保存到变量地址中
				{
					writeSingleVariableToAddress(currentMenuUint->VariableAddr[temp], currentMenuUint->VariableType[temp], tempVariablesValue[temp]);
				}
				int16 copyVariableValue[MENU_VARIABLES_NUM] = {0};
				collectVariablesValue(menuPoint, copyVariableValue);
				writeAllVariablesToStorage(copyVariableValue, 1); //将变量值保存到储存器中
			}
			isInited = 0;
			pageHandle = &homePageHandle; //退出该界面,转到菜单主界面
		}
		else if(unitPageCursor < MENU_UNIT_VARIABLES_NUM * MENU_VARIABLE_PERLINE) //如果光标指向变量
		{
			confirmFlag ^= 1;
		}
	}
	
	__showUintPage(currentMenuUint, tempVariablesValue, 0, unitPageCursor, confirmFlag); //更新光标
	return 0;
}

//-------------------------------------------------------------------------
//  @函数   警告界面
//  @参数   buttonFlag:按键值
//-------------------------------------------------------------------------
static uint8 warnPageHandle(uint8 buttonFlag)
{
	static uint8 isInited = 0;
	static int8 warmPageCursor = (MENU_LINE_MAX - 1) * 2;
	
	if(isInited == 0) //界面初始化,只在进入该界面时执行一次,在退出界面后置零
	{
		isInited = 1;
		warmPageCursor = (MENU_LINE_MAX - 1) * 2;
		__showWarnPage("Some variables have been changed. Would  you like to use the  new value?", 1, warmPageCursor); //显示该界面
	}
	
	if(buttonFlag & ButtonFlag_Udlr) //在该界面下,上下左右按键控制光标移动
	{
		if(buttonFlag & ButtonFlag_Up) warmPageCursor -= 1;
		if(buttonFlag & ButtonFlag_Down) warmPageCursor += 1;
		if(buttonFlag & ButtonFlag_Left) warmPageCursor -= 1;
		if(buttonFlag & ButtonFlag_Right) warmPageCursor += 1;
		
		if(warmPageCursor < (MENU_LINE_MAX - 1) * 2) warmPageCursor += 2;
		if(warmPageCursor >= MENU_LINE_MAX * 2) warmPageCursor -= 2;
	}
	
	if(buttonFlag & ButtonFlag_Confirm) //如果按下"YES"或者"NO"
	{
		coverVariablesToAddressOrStorage(menuPoint, (warmPageCursor + 1) % 2);
		pageHandle = &homePageHandle;
		__changePages();
	}
	
	__showWarnPage("", 0, warmPageCursor); //更新光标
	return 0;
}

//-------------------------------------------------------------------------
//  @函数   菜单工作函数
//  @返回   0:菜单没有按下“CloseMenu”选项 1:菜单按下“CloseMenu”选项
//  @注意   建议每隔200ms执行一次该函数,如:while(!Menu_Work()) systick_delay_ms(200);
//-------------------------------------------------------------------------
uint8 Menu_Work(void)
{
	int8 buttonFlag = 0, returnFlag = 0;
	
	buttonFlag = buttonGet(); //检测按键
	
	//pageHandle为函数指针,它会指向warnPageHandle,homePageHandle,unitPageHandle,指向不同的函数,会显示不同的界面
	returnFlag = pageHandle(buttonFlag);

	if(buttonFlag != ButtonFlag_Void && !returnFlag) __setBuzzer(1);
	else __setBuzzer(0);
	return returnFlag;
}
