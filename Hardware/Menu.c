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




Menu menu[4];

void All_Start(void);
void All_Stop(void);

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
    menu[0].item[1].type = function;
    menu[0].item[1].value = 0;
    menu[0].item[1].subIndex = -1;
    menu[0].item[1].function = All_Start;
    strcpy(menu[0].item[2].name, "Stop");
    menu[0].item[2].type = function;
    menu[0].item[2].value = 0;
    menu[0].item[2].subIndex = -1;
    menu[0].item[2].function = All_Stop;

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
    
   
}

int16_t menuIndex = 0,itemIndex = 0,mode = 0;



void OLED_ShowMenu(void)
{

		OLED_ShowString(0,0,menu[menuIndex].title,OLED_8X16);
		for(int16_t i = 0;i < menu[menuIndex].count;i++)
		{
			if(i == itemIndex){
				OLED_ShowString(0,(i + 1) * 16,">",OLED_8X16);
			}
			OLED_ShowString(8,(i + 1) * 16,menu[menuIndex].item[i].name,OLED_8X16);

			if(menu[menuIndex].item[i].type == dec )
			{
				OLED_ShowFloatNum(88,(i + 1) * 16,menu[menuIndex].item[i].value,1,2,OLED_8X16);
			}
		}

	OLED_Update();
	
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
	if(mode == 1 && menu[menuIndex].item[itemIndex].type != function)
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
	} else if(mode == 0 && menu[menuIndex].item[itemIndex].type != function)
	{
		mode = 1;
	} else if( mode == 1 && menu[menuIndex].item[itemIndex].type != function)
	{
		mode = 0;
	}
	
}

void Menu_Order(int16_t menuIndex ,int16_t itemIndex,int16_t value)
{
	menu[menuIndex].item[itemIndex].value+=value;
}

void All_Start(void)
{
	mode = 2;
}



void All_Stop(void)
{
	mode = 0;
}

