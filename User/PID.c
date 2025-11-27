#include "stm32f10x.h"                  // Device header

#include "PID.h"
#include "math.h"


void PID_Update_Add(PID_TypeDef *p)
{
	p->error2 = p->error1;
	p->error1 = p->error0;
	p->error0 = p->Target - p->Actual;
	
	p->Out += p->Kp * (p->error0 - p->error1)
		   + p->Ki * p->error0 
		   + p->Kd * (p->error0 - 2 * p->error1 + p->error2);
	
	if(p->Out > p->OutMax){p->Out = p->OutMax;}
	if(p->Out < p->OutMin){p->Out = p->OutMin;}
}	

void PID_Update_Pos(PID_TypeDef *p)
{
	
	p->error1 = p->error0;
	p->error0 = p->Target - p->Actual;
	
	p->errorInt += p->error0;
	
//	if(p->errorInt >= 30){p->errorInt = 30;}
//	if(p->errorInt <= -30){p->errorInt = -30;}
	
	p->Out = p->Kp * p->error0
		   + p->Ki * p->errorInt
		   + p->Kd * (p->error0 - p->error1);
	
	if(p->Out > p->OutMax){p->Out = p->OutMax;}
	if(p->Out < p->OutMin){p->Out = p->OutMin;}
}	

