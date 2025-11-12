#include "stm32f10x.h"                  // Device header
#include <string.h>
#include "OLED.h"
typedef enum {
    submenu,    
    data,      
    function,
	dec
} ItemType;

typedef struct {
    char name[20];
    ItemType type;
    int16_t value;           
    int16_t subIndex;    
    int16_t (*function)(void);  
} MenuItem;

typedef struct {
    char title[20];
    MenuItem item[4];  
    int16_t count;
    int16_t parIndex;     
} Menu;




Menu menu[5];

void Menu_Init(void)
{
    strcpy(menu[0].title, "Main Menu");
    menu[0].count = 3;
    menu[0].parIndex = -1;
    strcpy(menu[0].item[0].name, "PID");
    menu[0].item[0].type = submenu;
    menu[0].item[0].value = 0;
    menu[0].item[0].subIndex = 1;
    menu[0].item[0].function = NULL;
	strcpy(menu[0].item[1].name, "Start");
    menu[0].item[1].type = submenu;
    menu[0].item[1].value = 0;
    menu[0].item[1].subIndex = 2;
    menu[0].item[1].function = NULL;
    strcpy(menu[0].item[2].name, "Stop");
    menu[0].item[2].type = submenu;
    menu[0].item[2].value = 0;
    menu[0].item[2].subIndex = 3;
    menu[0].item[2].function = NULL;

	
    
    strcpy(menu[1].title, "PID");
    menu[1].count = 3;
    menu[1].parIndex = 0;
    strcpy(menu[1].item[0].name,"KP");
    menu[1].item[0].type = dec;
    menu[1].item[0].value = 0;
    menu[1].item[0].subIndex = -1;
    menu[1].item[0].function = NULL;
    strcpy(menu[1].item[1].name, "KI");
    menu[1].item[1].type = dec;
    menu[1].item[1].value = 0;
    menu[1].item[1].subIndex = -1;
    menu[1].item[1].function = NULL;
    strcpy(menu[1].item[2].name, "KD");
    menu[1].item[2].type = dec;
    menu[1].item[2].value = 0;
    menu[1].item[2].subIndex = -1;
    menu[1].item[2].function = NULL;
    
    strcpy(menu[2].title, "Image");
    menu[2].count = 1;
    menu[2].parIndex = 0;
    strcpy(menu[2].item[0].name,"Image");
    menu[2].item[0].type = function;
    menu[2].item[0].value = 0;
    menu[2].item[0].subIndex = -1;
    menu[2].item[0].function = NULL;
    
   
}

int16_t menuIndex = 0,itemIndex = 0,mode = 0;

void OLED_ShowPointNum(int16_t line, int16_t column, int16_t num, int16_t intlen, int16_t declen, int16_t place);

void OLED_ShowMenu(void)
{
	if(menu[menuIndex].count < 4){
		OLED_ShowString(1,1,menu[menuIndex].title,OLED_8X16);
		for(int16_t i = 0;i < menu[menuIndex].count;i++)
		{
			if(i == itemIndex){
				OLED_ShowString(i + 2,1,">",OLED_8X16);
			} else {
				OLED_ShowString(i + 2,1," ",OLED_8X16);
			}
			OLED_ShowString(i + 2,2,menu[menuIndex].item[i].name,OLED_8X16);
			if(menu[menuIndex].item[i].type == data )
			{
				OLED_ShowNum(i + 2,12,menu[menuIndex].item[i].value ,1,OLED_8X16);
			}
			if(menu[menuIndex].item[i].type == dec )
			{
				OLED_ShowPointNum(i + 2,12,menu[menuIndex].item[i].value,1,1,1);
			}
		}
	
	} else {
		for(int8_t i = 0;i < menu[menuIndex].count;i++)
		{
			if(i==itemIndex){
				OLED_ShowString(i + 1,1,">",OLED_8X16);
			} else {
				OLED_ShowString(i + 1,1," ",OLED_8X16);
			}
			
			OLED_ShowString(i + 1,2,menu[menuIndex].item[i].name,OLED_8X16);
			
		}
	}
	
}

void OLED_ShowPointNum(int16_t line, int16_t column, int16_t num, int16_t intlen, int16_t declen, int16_t place)
{
    int16_t divisor = 1;
    for(int i = 0; i < place; i++){
        divisor *= 10;
    }
    int16_t front = num / divisor,behind = num % divisor;  
    OLED_ShowNum(line, column, front, intlen,OLED_8X16);
    OLED_ShowString(line, column + intlen * 2 - 1, ".",OLED_8X16); 
    OLED_ShowNum(line, column + intlen * 2 , behind, declen,OLED_8X16);
}


void up(void)
{
	itemIndex--;
	if(itemIndex < 0)
	{
	itemIndex = menu[menuIndex].count-1;
	}
}


void down(void)
{
	itemIndex++;
	if(itemIndex == menu[menuIndex].count)
	{
	itemIndex = 0;
	}
}

void back(void)
{
	if(mode == 1)
	{
		mode = 0;
	}else if(menu[menuIndex].parIndex != -1 && mode == 0){
		itemIndex = menuIndex - 1;
		menuIndex = menu[menuIndex].parIndex;
	}
}

void confirm(void)
{
	if(menu[menuIndex].item[itemIndex].subIndex != -1){
		menuIndex = menu[menuIndex].item[itemIndex].subIndex;
		itemIndex = 0;
	} else if(mode == 0)
	{
		mode = 1;
	} //else if( mode == 1)
//	{
//		mode = 0;
//	}
	
}

void Menu_Order(int16_t menuIndex ,int16_t itemIndex,int16_t value)
{
	menu[menuIndex].item[itemIndex].value+=value;
}







