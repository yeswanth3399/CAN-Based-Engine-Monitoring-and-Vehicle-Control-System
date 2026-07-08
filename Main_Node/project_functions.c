#include <LPC21xx.h>

#include "LCD.h"
#include "DS18B20.h"
#include "delays.h"
#include "types.h"
#include "pro_funcs.h"
#include "CAN.h"
#include "CAN_defines.h"

/*=========================================================
                ENGINE TEMPERATURE LIMITS
=========================================================*/

#define TEMP_NORMAL_MAX      40
#define TEMP_WARM_MAX        70
#define TEMP_CRITICAL_MAX    90

/*=========================================================
                GLOBAL VARIABLES
=========================================================*/

/*=========================================================
                DISPLAY STATES
=========================================================*/

typedef enum
{
    DISPLAY_STARTUP = 0,
    DISPLAY_DASHBOARD,
    DISPLAY_WINDOW_UP,
    DISPLAY_WINDOW_DOWN,
    DISPLAY_REVERSE,
    DISPLAY_WARNING

}DISPLAY_STATE_T;


static unsigned char WindowLevel = 0;

static int EngineTemp;
static DISPLAY_STATE_T DisplayState = DISPLAY_STARTUP;

f32 temp;
s32 tVal;
u16 intPart,fracPart;

/*=========================================================
                DISPLAY TEMPERATURE
=========================================================*/

void Read_Temperature(void)
{
		s8 sign=1;

    temp = (ds18b20_read_temperature());
		tVal=temp*10000;
	
    if(tVal < 0)
    {
        sign=-1;
        tVal = -tVal;
    }

    intPart = tVal / 10000;
    fracPart = tVal % 10000;

    EngineTemp = (sign < 0) ? -intPart : intPart;
}

void Display_Temperature(void)
{
		if(EngineTemp < 0)
    {
        char_LCD('-');
		}
		u32_LCD(intPart);

    char_LCD('.');

		char_LCD((fracPart/1000) + '0');
		char_LCD(((fracPart/100) % 10) + '0');

    char_LCD(0xDF);
    char_LCD('C');
}

/*=========================================================
                ENGINE STATUS
=========================================================*/

void Display_Engine_Status(void)
{
    if(EngineTemp < TEMP_NORMAL_MAX)
    {
        str_LCD("NORMAL     ");
    }
    else if(EngineTemp < TEMP_WARM_MAX)
    {
        str_LCD("WARMING    ");
    }
    else if(EngineTemp < TEMP_CRITICAL_MAX)
    {
        str_LCD("HIGH TEMP  ");
    }
    else
    {
        str_LCD("CRITICAL   ");
    }
}

/*=========================================================
                OVERHEAT WARNING
=========================================================*/

void Overheat_Warning(void)
{
    if(DisplayState == DISPLAY_WARNING)
        return;

    cmd_LCD(0x01);

    cmd_LCD(0x80);
    str_LCD("!!!!! WARNING !!!!!!");

    cmd_LCD(0xC0);
    str_LCD("ENGINE OVERHEAT     ");

    cmd_LCD(0x94);
    str_LCD("STOP VEHICLE NOW    ");

    cmd_LCD(0xD4);
    str_LCD("CHECK COOLING SYS   ");

    DisplayState = DISPLAY_WARNING;
}

/*=========================================================
                STARTUP SCREEN
=========================================================*/

void Startup_Screen(void)
{
    cmd_LCD(0x01);

    cmd_LCD(0x80);
    str_LCD("..CAN BASED ENGINE..");

    cmd_LCD(0xC0);
    str_LCD("MONITORING SYSTEM   ");

    cmd_LCD(0x94);
    str_LCD("VEHICLE CONTROL     ");

    cmd_LCD(0xD4);
    str_LCD("INITIALIZING........");

    delay_s(2);

    cmd_LCD(0x01);
}

/*=========================================================
                DASHBOARD
=========================================================*/

void Vehicle_Dashboard(void)
{
    if(DisplayState != DISPLAY_DASHBOARD)
		{
			cmd_LCD(0x01);

			cmd_LCD(0x80);
			str_LCD("*****ENGINE SYS*****");

			cmd_LCD(0xC0);
			str_LCD("ENG TEMP:  ");

			cmd_LCD(0x94);
			str_LCD("MODE : FORWARD ");

			cmd_LCD(0xD4);
			str_LCD("STATUS : ");

			DisplayState = DISPLAY_DASHBOARD;
		}
}

void Dashboard_Update(void)
{
    /* Always read latest temperature */
    Read_Temperature();

    /* Warning screen currently active */
    if(DisplayState == DISPLAY_WARNING)
    {
        if(EngineTemp < TEMP_CRITICAL_MAX)
        {
            Return_To_Dashboard();
        }
        else
        {
            return;
        }
    }

    /* Temperature became critical */
    if(EngineTemp >= TEMP_CRITICAL_MAX)
    {
        Overheat_Warning();
        return;
    }


		/* Update only temperature always */
		cmd_LCD(0xC9);
		Display_Temperature();
		str_LCD("   ");

		/* Update engine status only in Dashboard */
		if(DisplayState == DISPLAY_DASHBOARD)
		{
			cmd_LCD(0xDD);
			str_LCD("          ");

			cmd_LCD(0xDD);
			Display_Engine_Status();
		}
}
/*=========================================================
                WINDOW UP ANIMATION
=========================================================*/

void Window_Up_Animation(void)
{
    unsigned int i;

    if(WindowLevel < 8)
        WindowLevel++;

    cmd_LCD(0xD4);

    str_LCD("LEVEL : [");

    for(i=0;i<WindowLevel;i++)
    {
        char_LCD(0xFF);
    }

    for(i=WindowLevel;i<8;i++)
    {
        char_LCD(' ');
    }

    char_LCD(']');
}

/*=========================================================
                WINDOW DOWN ANIMATION
=========================================================*/

void Window_Down_Animation(void)
{
    unsigned int i;

    if(WindowLevel > 0)
        WindowLevel--;

    cmd_LCD(0xD4);

    str_LCD("LEVEL : [");

    for(i=0;i<WindowLevel;i++)
    {
        char_LCD(0xFF);
    }

    for(i=WindowLevel;i<8;i++)
    {
        char_LCD(' ');
    }

    char_LCD(']');
}

/*=========================================================
                WINDOW OPEN
=========================================================*/

void Window_Up_Mode(void)
{
    if(DisplayState != DISPLAY_WINDOW_UP)
    {

        cmd_LCD(0x94);
        str_LCD("W MODE:OPEN  ST:MOVE");

        DisplayState = DISPLAY_WINDOW_UP;
    }
}

/*=========================================================
                WINDOW CLOSE
=========================================================*/

void Window_Down_Mode(void)
{
    if(DisplayState != DISPLAY_WINDOW_DOWN)
    {
        cmd_LCD(0x94);
         str_LCD("W MODE:CLOSE ST:MOVE");

        DisplayState = DISPLAY_WINDOW_DOWN;
    }
}

/*=========================================================
                START VEHICLE
=========================================================*/

void vehicle(void)
{
    Startup_Screen();
	  DisplayState = DISPLAY_STARTUP;
}

/*=========================================================
                REVERSE SCREEN
=========================================================*/

void Reverse_Mode_Screen(void)
{
	  if(DisplayState == DISPLAY_REVERSE)
			return;
    cmd_LCD(0x01);

    cmd_LCD(0x80);
    str_LCD("****REVERSE MODE****");

    cmd_LCD(0xC0);
    str_LCD("MODE : ACTIVE       ");

    cmd_LCD(0x94);
    str_LCD("DIST : WAIT...      ");

    cmd_LCD(0xD4);
    str_LCD("STATUS : READY      ");
		
		DisplayState = DISPLAY_REVERSE;
}

/*=========================================================
                ACTIVE HIGH BUZZER
=========================================================*/

void Buzzer_Beep(void)
{
    IOSET0 = (1<<21);

    delay_ms(300);

    IOCLR0 = (1<<21);
}
/*=========================================================
                RETURN TO DASHBOARD
=========================================================*/

void Return_To_Dashboard(void)
{
    DisplayState = DISPLAY_STARTUP;
}


//Window Node error
void show_window_error(void)
{
	cmd_LCD(0x01);

	cmd_LCD(0x80);
	str_LCD("*****  ERROR   *****");

	cmd_LCD(0xc0);
	str_LCD("..WINDOW NODE FAIL..");

	cmd_LCD(0x94);
	str_LCD("..CHECK CAN WIRING..");

	cmd_LCD(0xD4);
	str_LCD("..NODE  NOT  FOUND..");

	delay_ms(1500);

	Return_To_Dashboard();
}

//Reverse Node error
void show_reverse_error(void)
{
	cmd_LCD(0x01);

	cmd_LCD(0x80);
	str_LCD("*****  ERROR   *****");

	cmd_LCD(0xc0);
	str_LCD(".REVERSE NODE  FAIL.");

	cmd_LCD(0x94);
	str_LCD("..CHECK CAN WIRING..");

	cmd_LCD(0xD4);
	str_LCD("..NODE  NOT  FOUND..");

	delay_ms(1500);

	Return_To_Dashboard();
}

