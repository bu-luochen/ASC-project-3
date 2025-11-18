#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSenser.h"
#include "OLED.h"
#include "CountSenser.h"
#include "Encoder.h"
#include "Timer.h"
#include "PWM.h"
#include "Motor.h"
#include "IC.h"
#include "EI.h"
#include "Serial.h"
#include <string.h>
#include "PID.h"
#include "Menu.h"
#include "Infrared.h"

extern uint16_t Time_Serial;
extern uint16_t Time_Infrared; 


void PID_Set(uint8_t Index);

PID_TypeDef left = {
	
	.error0 = 0,
	.error1 = 0,
	.error2 = 0,
	
	.Kp = 0.03,
	.Ki = 0.23,
	.Kd = 0,
	
	.OutMax = 100,
	.OutMin = -100,
	
	.Out = 0,
	
};

PID_TypeDef right = {
	.error0 = 0,
	.error1 = 0,
	.error2 = 0,
	
	.Kp = 0.03,
	.Ki = 0.23,
	.Kd = 0,
	
	.OutMax = 100,
	.OutMin = -100,
	
	.Out = 0,
};

uint8_t Direct[6] = {0};
int16_t Speed,Location;



extern Menu menu[4];
extern int16_t menuIndex;
extern int16_t itemIndex;
extern int16_t mode;
extern uint8_t start_flag;




int main ()
{	
	OLED_Init();
	
	Timer_Init();
	Key_Init();
	Serial_Init();
	EI_Init();
	Motor_Init();
	Menu_Init();
	
	menu[1].item[0].value = left.Kp;
	menu[1].item[1].value = left.Ki;
	menu[1].item[2].value = left.Kd;
	
	while(1)
	{	
		
		if(Serial_RxFlag == 1){
				left.Target = 0;
				right.Target = 0;
				if(Serial_RxPacket[0] == '-'){
					for(int i = 1;Serial_RxPacket[i] != '%';i++){
						left.Target = left.Target * 10 + (Serial_RxPacket[i] - '0');
					}
					if(left.Target >= 100){left.Target = 100;}
					left.Target = -left.Target;
					right.Target = left.Target;
				} else{  
					for(int i = 0;Serial_RxPacket[i] != '%';i++){
						left.Target = left.Target * 10 + (Serial_RxPacket[i] - '0');
					}
					if(left.Target >= 100){left.Target = 100;}
					right.Target = left.Target;
				}
				
			Serial_RxFlag = 0;
				
		}
		
		
		
		if(Key_Check(KEY_4,KEY_SINGLE)){
			if(mode != 1){
				up();
				} else if(mode == 1 && menuIndex == 1)
				{
					menu[1].item[itemIndex].value += 0.01;
					PID_Set(itemIndex);
				}
		}
		
		if(Key_Check(KEY_2,KEY_SINGLE)){
			if(mode != 1){
				down();
			} else if(mode == 1 && menuIndex == 1)
			{
				menu[1].item[itemIndex].value -= 0.01;
				PID_Set(itemIndex);
			}
		}
		
		if(Key_Check(KEY_14,KEY_SINGLE)){
			back();
			OLED_Clear();
		}
		if(Key_Check(KEY_0,KEY_SINGLE)){
			OLED_Clear();
			if (menu[menuIndex].item[itemIndex].function != NULL){
				menu[menuIndex].item[itemIndex].function();
			}
			confirm();
			
		}
		if(Key_Check(KEY_4,KEY_REPEAT)){
			if(mode == 1 && menuIndex == 1)
			{
				menu[1].item[itemIndex].value += 0.05;
				PID_Set(itemIndex);
			}
		}
		if(Key_Check(KEY_2,KEY_REPEAT)){
			if(mode == 1 && menuIndex == 1)
			{
				menu[1].item[itemIndex].value -= 0.05;
				PID_Set(itemIndex);				
			}
		}
		OLED_Clear();
		if(mode == 1){
			OLED_ShowString(120,0,"E",OLED_8X16);
		}
		
		if(Time_Infrared >= 20){
			Time_Infrared = 0;
			Infrared_GetDir(Direct);
			
			OLED_ShowNum(64,16,Direct[1],1,OLED_8X16);
			OLED_ShowNum(72,16,Direct[2],1,OLED_8X16);
			OLED_ShowNum(80,16,Direct[3],1,OLED_8X16);
			OLED_ShowNum(88,16,Direct[4],1,OLED_8X16);
			OLED_ShowNum(96,16,Direct[5],1,OLED_8X16);
		}
		
		
		OLED_ShowMenu();
		
		OLED_Update();
		
//		OLED_ShowNum(64,16,left.Target,3,OLED_8X16);
//		OLED_ShowNum(64,32,right.Target,3,OLED_8X16);
		
		if(Time_Serial >= 10){
			Time_Serial = 0 ;
			if(start_flag == 1){
				printf("%.2f,%.2f\n",left.Actual,left.Target);
			}
		}
		
		
		
		
		
//		OLED_Printf(0,16,OLED_8X16,"Target=%+05.0f",right.Target);
//		OLED_Printf(0,32,OLED_8X16,"Actual=%+05.0f",right.Actual);
//		OLED_Printf(0,48,OLED_8X16,"Out=%+05.0f",right.Out);
//		
		
		
		
	}
	
}

void TIM2_IRQHandler(void)
{
	static uint16_t Count1,Count2;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){

		Key_Tick();
		Serial_Tick();
		Infrared_Tick();
		if(start_flag == 1){
			Count1 ++;
			if(Count1 >= 20){
				Count1 = 0;
				Speed = EI_GetTim3() * 125.0 / 142.0;
				
				left.Actual = Speed;
				
				PID_Update(&left);
				
				Motor_SetSpeed(M1,left.Out);
					
			}
			
			Count2++;
			if(Count2 >= 20){
				Count2 = 0;
				Speed = EI_GetTim4() * 100.0 / 142.0;
				right.Actual = Speed;
				
				PID_Update(&right);
				
				Motor_SetSpeed(M2,right.Out);
				
			}
			
		} else {
			Motor_SetSpeed(M1,0);
			Motor_SetSpeed(M2,0);
		}
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	
}



void PID_Set(uint8_t Index)
{
	switch(Index){
		case 0:
				left.Kp = menu[1].item[itemIndex].value;
				right.Kp = menu[1].item[itemIndex].value;
			break;
		case 1:
				left.Ki = menu[1].item[itemIndex].value;
				right.Ki = menu[1].item[itemIndex].value;
			break;
		case 2:
				left.Kd = menu[1].item[itemIndex].value;
				right.Kd = menu[1].item[itemIndex].value;
			break;
	}
}



