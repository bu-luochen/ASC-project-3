#ifndef __MENU_H
#define __MENU_H
void Menu_Init(void);
void OLED_ShowMenu(void);
void up(void);
void down(void);
void back(void);
void confirm(void);
void Menu_Order(int16_t menuIndex ,int16_t itemIndex,int16_t value);
void All_Start(void);
void All_Stop(void);

#endif
