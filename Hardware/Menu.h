#ifndef __MENU_H
#define __MENU_H
void Menu_Init(void);
void OLED_ShowMenu(void);
void OLED_ShowPointNum(int16_t line, int16_t column, int16_t num, int16_t intlen, int16_t declen, int16_t place);
void up(void);
void down(void);
void back(void);
void confirm(void);
void Menu_Order(int16_t menuIndex ,int16_t itemIndex,int16_t value);


#endif
