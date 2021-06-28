#include "SELFBUILD_correct_sensor.h"


uint8 MCP41_EEPROM_Save[8];//用于保存MCP41返回的电阻值
ADCCH_enum MCP41_ADC[ADC_NUM] = {
	AD_CH_L__,
	AD_CH_L_1,
	AD_CH_M_1,
	AD_CH_R_1,
	AD_CH_R__
};//设定的通道

uint8 MCP41_Set[ADC_NUM] = {
	ADC_L_L_Min_8bit,
	ADC_L_Min_8bit,
	ADC_M_Min_8bit,
	ADC_R_Min_8bit,
	ADC_R_R_Min_8bit
};//各个电感的设定值(对应上面设定的通道号)

void Correct_Sensor(void)
{
	MCP41_Init();  
	gpio_init(SET_MCP41_CS0, GPO, 1,PULLUP);
	gpio_init(SET_MCP41_CS1, GPO, 1,PULLUP);
	gpio_init(SET_MCP41_CS2, GPO, 1,PULLUP);
	gpio_init(SET_MCP41_CS3, GPO, 1,PULLUP);
	gpio_init(SET_MCP41_CS4, GPO, 1,PULLUP);
	EEPROM_InitMCP41();
	
	if(gpio_get(SWITCH2_4) == 0)
		Normalized_MCP41();
}

void Normalized_MCP41(void)
{
	int8 _index = 1, _index_old = 1;
	OLED_P6x8Str((OLED_X_MAX - 1 - 6 * (sizeof("Normalized") - 1)) / 2, 0, "Normalized");
	OLED_P6x8Str(OLED_SHOW(1), 1, "L__");
	OLED_P6x8Str(OLED_SHOW(2), 1, "L_1");
	OLED_P6x8Str(OLED_SHOW(3), 1, "M_1");
	OLED_P6x8Str(OLED_SHOW(4), 1, "R_1");
	OLED_P6x8Str(OLED_SHOW(5), 1, "R__");//显示界面
        
	while(1)
	{
		OLED_P6x8Int(OLED_SHOW(1), 3, adc_convert(AD_CH_L__, ADC_8BIT), 3);
		OLED_P6x8Int(OLED_SHOW(2), 3, adc_convert(AD_CH_L_1, ADC_8BIT), 3);
		OLED_P6x8Int(OLED_SHOW(3), 3, adc_convert(AD_CH_M_1, ADC_8BIT), 3);
		OLED_P6x8Int(OLED_SHOW(4), 3, adc_convert(AD_CH_R_1, ADC_8BIT), 3);
		OLED_P6x8Int(OLED_SHOW(5), 3, adc_convert(AD_CH_R__, ADC_8BIT), 3);//显示电感读数

		if(gpio_get(BUTTON_LEFT) == 0) 
			_index--;						//光标左右移动
		if(gpio_get(BUTTON_RIGHT) == 0) 
			_index++;
		if(gpio_get(BUTTON_UP) == 0)//如果按下了上键，那么将设定的电阻值全部保存到FLASH
		{
			//flash_write(FLASH_SAVE_MCP41, 32 * temp, MCP41_Flash_Save[temp]);//写入电阻值
			for(int8 temp = 0; temp < 8; temp++)
				eeprom_write_word(FLASH_SAVE_MCP41,temp, MCP41_EEPROM_Save[temp]);		//写入电阻值
			break;
		}
		if(gpio_get(BUTTON_DOWN) == 0)//如果按了下键，那么不保存设定值，用原来保存的电阻值重新设置电位器
                {
                	EEPROM_InitMCP41();
                	break;
                }
		if(gpio_get(BUTTON_MID) == 0)//如果按确认键，那么开始调整电阻值
		{
			OLED_P6x8Str(OLED_SHOW(_index), 2, " & ");//调整过程中显示 & 
			MCP41_EEPROM_Save[_index - 1] = MCP41_SetValue(_index - 1, MCP41_Set[_index  - 1]);
			systick_delay_ms(50);
			OLED_P6x8Str(OLED_SHOW(_index), 2, " * ");//显示 * 表示调整结束
			OLED_P6x8Int(OLED_SHOW(_index), 4, MCP41_Set[_index - 1], 3);//显示调整值用于参考
		}
		
		if(_index == 0) _index = ADC_NUM;//光标控制
		if(_index == ADC_NUM + 1) _index = 1;
		
		OLED_P6x8Str(OLED_SHOW(_index), 2, " * ");
		if(_index != _index_old)
			OLED_P6x8Str(OLED_SHOW(_index_old), 2, "   ");
		
		_index_old = _index;
		systick_delay_ms(50);
		
		for(int temp = 0; temp < 3; ++temp)//增加电感读数显示的频率
		{
		OLED_P6x8Int(OLED_SHOW(1), 3, adc_convert(AD_CH_L__, ADC_8BIT), 3);
		OLED_P6x8Int(OLED_SHOW(2), 3, adc_convert(AD_CH_L_1, ADC_8BIT), 3);
		OLED_P6x8Int(OLED_SHOW(3), 3, adc_convert(AD_CH_M_1, ADC_8BIT), 3);
		OLED_P6x8Int(OLED_SHOW(4), 3, adc_convert(AD_CH_R_1, ADC_8BIT), 3);
		OLED_P6x8Int(OLED_SHOW(5), 3, adc_convert(AD_CH_R__, ADC_8BIT), 3);//显示电感读数
			systick_delay_ms(50);
		}
	}
        
	OLED_ClearScreen(OLED_BLACK);//while结束后清屏
	while(gpio_get(BUTTON_UP) == 0);//等待松手
	while(gpio_get(BUTTON_DOWN) == 0);
}

//从FLSAH中读出电阻的保存值并设置电位器
void EEPROM_InitMCP41(void)
{
  for(int8 temp = 0; temp < 8; temp++)
  {
			MCP41_EEPROM_Save[temp] =eeprom_read_word(FLASH_SAVE_MCP41,temp, uint8);
  }
  for(int8 temp = 0; temp < ADC_NUM; temp++)
  {
      MCP41_SetCmd(temp, MCP41_EEPROM_Save[temp] );
  }
}
