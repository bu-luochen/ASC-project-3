#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "OLED.h"


uint8_t Key_Flag[KEY_COUNT];

extern int16_t menucount,menuIndex,itemIndex,mode;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

}



uint16_t Key_GetState(uint8_t n)
{
	switch(n){
		case KEY_14:
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14) == 0){
				return PRESSED;
			}
			break;
		case KEY_0:
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0){
				return PRESSED;
			}
			break;
		case KEY_2:
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == 0){
				return PRESSED;
			}
			break;
		case KEY_4:
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0){
				return PRESSED;
			}
			break;
	}
	
	
	return UNPRESSED;
}

uint8_t Key_Check(uint8_t n,uint8_t Flag)
{
	if(Key_Flag[n] & Flag)
	{
		if(Flag != KEY_HOLD){
			Key_Flag[n] &= ~Flag;
		}
		return 1;
	}
	return 0;
}


void Key_Tick(void)
{
	static int16_t count,i;
	static int16_t current[KEY_COUNT],previous[KEY_COUNT];
	static int16_t S[KEY_COUNT];
	static int16_t Time[KEY_COUNT];
	
	for(i = 0;i < KEY_COUNT;i++){
		if(Time[i] > 0){
			Time[i]--;
		}
	}
	
	
	count++;
	if( count>= 20)
	{
		count = 0;
		for(i = 0;i < KEY_COUNT;i++){
			previous[i] = current[i];
			current[i] = Key_GetState(i);
			if(current[i] == PRESSED){
				Key_Flag[i] |= KEY_HOLD;
			} else {
				Key_Flag[i] &= ~KEY_HOLD;
			}
			
			if(current[i] == PRESSED && previous[i] == UNPRESSED){
				Key_Flag[i] |= KEY_DOWN;
			} 
			
			if(current[i] == UNPRESSED && previous[i] == PRESSED){
				Key_Flag[i] |= KEY_UP;
			} 
			
			switch(S[i]){
				case 0:
					if(current[i] == PRESSED){
						Time[i] = KEY_TIME_LONG;
						S[i] = 1;
					}
					break;
				case 1:
					if(current[i] == UNPRESSED){
						Time[i] = KEY_TIME_DOUBLE;
						S[i] = 2;
					} else if(Time[i] == 0){
						Key_Flag[i] |= KEY_LONG;
						Time[i] = KEY_TIME_REPEAT;
						S[i] = 4;
					}
					break;
				case 2:
					if(current[i] == PRESSED){
						Key_Flag[i] |= KEY_DOUBLE;
						S[i] = 3;
					} else if(Time[i] == 0){
						Key_Flag[i] |= KEY_SINGLE;
						S[i] = 0;
					}
					break;
				case 3:
					if(current[i] == UNPRESSED){
						S[i] = 0;
					}
					
					break;
				case 4:
					if(current[i] == UNPRESSED){
						S[i] = 0;
					}else if(Time[i] == 0){
						Key_Flag[i] |= KEY_REPEAT;
						Time[i] = KEY_TIME_REPEAT;
						
					}
					
					break;
				
			}
		}
		
	}
	
	
	
}

