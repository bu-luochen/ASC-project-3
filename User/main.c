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

extern  uint16_t Time_Serial;



PID_TypeDef Inner = {
	.Kp = 0.3,
	.Ki = 0,
	.Kd = 0,
	.OutMax = 100,
	.OutMin = -100,
};

PID_TypeDef Outer = {
	.Kp = 0.3,
	.Ki = 0,
	.Kd = 0,
	.OutMax = 20,
	.OutMin = -20,
};


int16_t Speed,Location;

int main ()
{	
	OLED_Init();
	LED_Init();
	Timer_Init();
	Key_Init();
	Serial_Init();
	EI_Init();
	Motor_Init();
	
	
	while(1)
	{	
		
		
		
	}
	
}

void TIM2_IRQHandler(void)
{
	static uint16_t Count1,Count2;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		Serial_Tick();
		Key_Tick();
		Count1 ++;
		if(Count1 >= 20){
			Count1 = 0;
			Speed = Encoder_Get();
			Location += Speed;
			
			Inner.Actual = Speed;
			
			PID_Update(&Inner);
			
			Motor_SetSpeed(M1,Inner.Out);
				
		}
		
		Count2++;
		if(Count2 >= 20){
			Count2 = 0;
			Outer.Actual = Location;
			
			PID_Update(&Outer);
			
			Motor_SetSpeed(M1,Outer.Out);
			
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

