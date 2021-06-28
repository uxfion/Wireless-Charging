










uint8 Freq_Adjust(void)
{
	int8 _index = 1, _index_old = 1;
	uint8 i;
	uint8 old_freq[3] = {0};
	uint8 num;
	
	iap_read_bytes(0x0000, old_freq, 3);
	num = (old_freq[0]-'0')*100+(old_freq[1]-'0')*10+(old_freq[2]-'0');
	
	
	OLED_P6x8Str((OLED_X_MAX-1-6*(sizeof("Crystal Freq")-1))/2, 0, "Crystal Freq");
	while(1)
	{
		OLED_P6x8Str(OLED_SHOW(1), 2, "Old"); OLED_P6x8Int(OLED_SHOW(2), 2, num, 4);
//		OLED_P6x8Str(OLED_SHOW(1), 3, "Old"); OLED_P6x8Str(OLED_SHOW(2), 3, old_freq);
	}
	
}