#include "stm32f10x.h"                  // Device header
#include "Infrared.h"

void Infared_Init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}

void Infrared_GetDir(uint8_t Direct[])
{
	
	if(X1 == 1){*(Direct + 1) = 1;} else {*(Direct + 1) = 0;}
	if(X2 == 1){*(Direct + 2) = 1;} else {*(Direct + 2) = 0;}
	if(X3 == 1){*(Direct + 3) = 1;} else {*(Direct + 3) = 0;}
	if(X4 == 1){*(Direct + 4) = 1;} else {*(Direct + 4) = 0;}
	if(X5 == 1){*(Direct + 5) = 1;} else {*(Direct + 5) = 0;}
	
}

uint16_t Time_Infrared;
void Infrared_Tick(void)
{
	
	Time_Infrared ++;

}


