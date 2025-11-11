#include "stm32f10x.h"                  // Device header

#include "PID.h"

void PID_Update(PID_TypeDef *p)
{
	p->error1 = p->error0;
	p->error0 = p->Target - p->Actual;
	
	if(p->Ki != 0){
		p->errorInt += p->error0;
	} else {
		p->errorInt = 0;
	}
	
	p->Out = p->Kp * p->error0 
		   + p->Ki * p->errorInt 
		   + p->Kd * (p->error0 - p->error1);
	if(p->Out > p->OutMax){p->Out = p->OutMax;}
	if(p->Out < p->OutMin){p->Out = p->OutMin;}
}	

