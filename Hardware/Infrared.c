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

void Infrared_GetDir(uint8_t Direct[],uint8_t Last[])
{
	if(Direct[1] == 1 || Direct[2] == 1  || Direct[3] == 1 ||  Direct[4] == 1 || Direct[5] == 1){
		Last[1] = Direct[1];
		Last[2] = Direct[2];
		Last[3] = Direct[3];
		Last[4] = Direct[4];
		Last[5] = Direct[5];
	} 
		
	
	
	
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

float BaseSpeed = 50,k = 1.5,Turn_factor = 2;
float Forward_Factor = 1,Back_Factor = 0.8; 

void Direct_Adjust(uint8_t Direct[],uint8_t Last[],float *left,float *right){
	if (Direct[1] == 0 && Direct[2] == 0 && Direct[3] == 0 && Direct[4] == 0 && Direct[5] == 0){
		if(Last[1] == 0 && Last[2] == 1 && Last[3] == 0 && Last[4] == 0 && Last[5] == 0 ||
		   Last[1] == 0 && Last[2] == 1 && Last[3] == 1 && Last[4] == 0 && Last[5] == 0){
			*left = Back_Factor * BaseSpeed * (1 - k * 0.5);
			*right = Forward_Factor * BaseSpeed * (1 + k);
		}	
			
		if(Last[1] == 0 && Last[2] == 0 && Last[3] == 0 && Last[4] == 1 && Last[5] == 0 ||
		   Last[1] == 0 && Last[2] == 0 && Last[3] == 1 && Last[4] == 1 && Last[5] == 0){
			*left =  Forward_Factor * BaseSpeed * (1 + k);
			*right = Back_Factor *  BaseSpeed * (1 - k * 0.5);
		}
			   
		if(Last[1] == 1 && Last[2] == 0 && Last[3] == 0 && Last[4] == 0 && Last[5] == 0 ||
		   Last[1] == 1 && Last[2] == 0 && Last[3] == 1 && Last[4] == 0 && Last[5] == 0){
			*left =  Back_Factor * BaseSpeed * (1 - k * Turn_factor);
			*right =  Forward_Factor * BaseSpeed * (1 + k);
		}	
			   
		if(Last[1] == 0 && Last[2] == 0 && Last[3] == 0 && Last[4] == 0 && Last[5] == 1 ||
		   Last[1] == 0 && Last[2] == 0 && Last[3] == 1 && Last[4] == 0 && Last[5] == 1){
			*left =  Forward_Factor * BaseSpeed * (1 + k);
			*right =  Back_Factor * BaseSpeed * (1 - k * Turn_factor);
		}   
		if(Last[1] == 1 && Last[2] == 0 && Last[3] == 0 && Last[4] == 0 && Last[5] == 0 ||
		   Last[1] == 1 && Last[2] == 0 && Last[3] == 1 && Last[4] == 0 && Last[5] == 0){
			*left =  Back_Factor * BaseSpeed * (1 - k);
			*right =  Forward_Factor * BaseSpeed * (1 + k);
		}	
			   
		if(Last[1] == 0 && Last[2] == 0 && Last[3] == 0 && Last[4] == 0 && Last[5] == 1 ||
			Last[1] == 0 && Last[2] == 0 && Last[3] == 1 && Last[4] == 0 && Last[5] == 1){
			*left =  Forward_Factor * BaseSpeed * (1 + k);
			*right =  Back_Factor * BaseSpeed * (1 - k);
		}   
	} else {
			if(Direct[1] == 0 && Direct[2] == 0 && Direct[3] == 1 && Direct[4] == 0 && Direct[5] == 0 ||
			   Direct[1] == 1 && Direct[2] == 1 && Direct[3] == 1 && Direct[4] == 1 && Direct[5] == 1 ||
			   Direct[1] == 1 && Direct[2] == 0 && Direct[3] == 1 && Direct[4] == 0 && Direct[5] == 1 ||
			   Direct[1] == 0 && Direct[2] == 1 && Direct[3] == 1 && Direct[4] == 1 && Direct[5] == 0 ){
				*left = BaseSpeed;
				*right = BaseSpeed;
			}
				
			if(Direct[1] == 0 && Direct[2] == 1 && Direct[3] == 0 && Direct[4] == 0 && Direct[5] == 0 ||
			   Direct[1] == 0 && Direct[2] == 1 && Direct[3] == 1 && Direct[4] == 0 && Direct[5] == 0){
				*left =  Back_Factor * BaseSpeed * (1 - k * 0.5);
				*right =  Forward_Factor * BaseSpeed * (1 + k);
			}	
			
			if(Direct[1] == 0 && Direct[2] == 0 && Direct[3] == 0 && Direct[4] == 1 && Direct[5] == 0 ||
			   Direct[1] == 0 && Direct[2] == 0 && Direct[3] == 1 && Direct[4] == 1 && Direct[5] == 0){
				*left =  Forward_Factor * BaseSpeed * (1 + k);
				*right =  Back_Factor * BaseSpeed * (1 - k * 0.5);
			}
			   
			if(Direct[1] == 1 && Direct[2] == 0 && Direct[3] == 0 && Direct[4] == 0 && Direct[5] == 0 ||
			   Direct[1] == 1 && Direct[2] == 0 && Direct[3] == 1 && Direct[4] == 0 && Direct[5] == 0){
				*left =  Back_Factor * BaseSpeed * (1 - k * 0.5);
				*right =  Forward_Factor * BaseSpeed * (1 + k);
			}	
			   
			if(Direct[1] == 0 && Direct[2] == 0 && Direct[3] == 0 && Direct[4] == 0 && Direct[5] == 1 ||
			   Direct[1] == 0 && Direct[2] == 0 && Direct[3] == 1 && Direct[4] == 0 && Direct[5] == 1){
				*left =  Forward_Factor * BaseSpeed * (1 + k);
				*right =  Back_Factor * BaseSpeed * (1 - k * 0.5);
			}   
			   
			if(Direct[1] == 1 && Direct[2] == 1 && Direct[3] == 0 && Direct[4] == 0 && Direct[5] == 0 ||
			   Direct[1] == 1 && Direct[2] == 1 && Direct[3] == 1 && Direct[4] == 0 && Direct[5] == 0){
				*left =  Back_Factor * BaseSpeed * (1 - k * Turn_factor);
				*right =  Forward_Factor * BaseSpeed * (1 + k);
			}	
			   
			if(Direct[1] == 0 && Direct[2] == 0 && Direct[3] == 0 && Direct[4] == 1 && Direct[5] == 1 ||
			   Direct[1] == 0 && Direct[2] == 0 && Direct[3] == 1 && Direct[4] == 1 && Direct[5] == 1){
				*left =  Forward_Factor * BaseSpeed * (1 + k);
				*right =  Back_Factor * BaseSpeed * (1 - k * Turn_factor);
			}   
			   
	
		}
	
}


