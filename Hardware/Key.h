#ifndef __KEY_H
#define __KEY_H

#define PRESSED                    1
#define UNPRESSED                  0

#define KEY_HOLD                   0x01
#define KEY_DOWN                   0x02
#define KEY_UP                     0x04
#define KEY_SINGLE                 0x08
#define KEY_DOUBLE                 0x10
#define KEY_LONG                   0x20
#define KEY_REPEAT                 0x40

#define KEY_TIME_DOUBLE            200
#define KEY_TIME_LONG              1000
#define KEY_TIME_REPEAT            100

#define KEY_COUNT                  4

#define KEY_14                     0
#define KEY_0					   1
#define KEY_2                      2
#define KEY_4                      3

void Key_Init(void);
void Key_Tick(void);
uint16_t Key_GetState(uint8_t n);
uint8_t Key_Check(uint8_t n,uint8_t Flag);
#endif
