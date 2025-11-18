#ifndef __MENU_H
#define __MENU_H

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
