#include "stm32f10x.h"                  // Device header
#include "Infrared.h"

void Infrared_Init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_1|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void Infrared_GetDir(uint8_t Direct[])
{
	
	if(X1 == 1){Direct[1] = 0;} else {Direct[1] = 1;}
	if(X2 == 1){Direct[2] = 0;} else {Direct[2] = 1;}
	if(X3 == 1){Direct[3] = 0;} else {Direct[3] = 1;}
	if(X4 == 1){Direct[4] = 0;} else {Direct[4] = 1;}
	if(X5 == 1){Direct[5] = 0;} else {Direct[5] = 1;}
	
}

uint16_t Time_Infrared = 0;
void Infrared_Tick(void)
{
	Time_Infrared ++;
}




int16_t Direct_GetError(uint8_t Direct[])
{
	int16_t Error;
	Error = -5 * Direct[1] + -3 * Direct[2] + 0 * Direct[3] + 3 * Direct[4] + 5 * Direct[5];
	return Error;
		
	
}


