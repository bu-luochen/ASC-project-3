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

extern  uint16_t Time_Serial;



PID_TypeDef left = {
	.Kp = 0.3,
	.Ki = 0,
	.Kd = 0,
	.OutMax = 100,
	.OutMin = -100,
	
};

PID_TypeDef right = {
	.Kp = 0.3,
	.Ki = 0,
	.Kd = 0,
	.OutMax = 100,
	.OutMin = -100,
};


int16_t Speed,Location;


typedef enum {
    submenu,    
    data,      
    function    
} ItemType;

typedef struct {
    char name[20];
    ItemType type;
    float value;           
    int16_t subIndex;    
    void (*function)(void);  
} MenuItem;

typedef struct {
    char title[20];
    MenuItem item[4];  
    int16_t count;
    int16_t parIndex;     
} Menu;
extern Menu menu[4];
extern int16_t menuIndex;
extern int16_t itemIndex;
extern int16_t mode;





int main ()
{	
	OLED_Init();
	
	Timer_Init();
	Key_Init();
	Serial_Init();
	EI_Init();
	Motor_Init();
	Menu_Init();
	
	
	
	
	while(1)
	{	
		if(Key_Check(KEY_4,KEY_SINGLE)){
			if(mode != 1){
				up();
				} else if(mode == 1 && menuIndex == 1)
				{
					menu[1].item[itemIndex].value += 0.01;
				}
		}
		
		if(Key_Check(KEY_2,KEY_SINGLE)){
			if(mode != 1){
				down();
			} else if(mode == 1 && menuIndex == 1)
			{
				menu[1].item[itemIndex].value -= 0.01;
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
			}
		}
		if(Key_Check(KEY_2,KEY_REPEAT)){
			if(mode == 1 && menuIndex == 1)
			{
				menu[1].item[itemIndex].value -= 0.05;	
			}
		}
		OLED_Clear();
		if(mode == 1){
			OLED_ShowString(120,0,"E",OLED_8X16);
		}
		
		OLED_ShowMenu();
		
		
	}
	
}

void TIM2_IRQHandler(void)
{
	static uint16_t Count1,Count2;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){

		Key_Tick();
//		Serial_Tick();
		Count1 ++;
		if(Count1 >= 20){
			Count1 = 0;
			Speed = Encoder_Get();
			
			left.Actual = Speed;
			
			PID_Update(&left);
			
			Motor_SetSpeed(M1,left.Out);
				
		}
		
		Count2++;
		if(Count2 >= 20){
			Count2 = 0;
			right.Actual = Location;
			
			PID_Update(&right);
			
			Motor_SetSpeed(M2,right.Out);
			
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	
}

