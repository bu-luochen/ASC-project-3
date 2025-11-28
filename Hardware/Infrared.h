#ifndef __INFRARED_
#define __INFRARED_

#define X1			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
#define X2			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define X3			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define X4			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define X5			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

void Infrared_Init(void);
void Infrared_GetDir(uint8_t Direct[]);
void Infrared_Tick(void);
float Direct_GetError(uint8_t Direct[]);
#endif

