#include "stm32f10x.h"                  // Device header

#include "PID.h"



void PID_Update(PID_TypeDef *p)
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

