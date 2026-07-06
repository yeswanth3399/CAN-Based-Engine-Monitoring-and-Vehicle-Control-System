#include <LPC21xx.h>

#include "LCD.h"
#include "DS18B20.h"
#include "delays.h"
#include "dashboard.h"
#include "types.h"

#define TEMP_NORMAL_MAX      40
#define TEMP_WARM_MAX        70
#define TEMP_CRITICAL_MAX    90

static int EngineTemp;

/*=========================================================
 * Display Temperature
 *=========================================================*/

void Display_Temperature(void)
{
    s32 tVal;
    u32 intPart;
    u32 fracPart;

    tVal = (ReadTemp() * 100) / 16;

    if(tVal < 0)
    {
        char_LCD('-');
        tVal = -tVal;
    }

    intPart  = tVal / 10000;
    fracPart = tVal % 10000;

    EngineTemp = intPart;

    num_LCD(intPart);

    char_LCD('.');

    char_LCD((fracPart/1000)+'0');
    char_LCD(((fracPart/100)%10)+'0');

    char_LCD(0xDF);
    char_LCD('C');
}

/*=========================================================
 * Display Engine Status
 *=========================================================*/

void Display_Engine_Status(void)
{
    if(EngineTemp < TEMP_NORMAL_MAX)
    {
        str_LCD("NORMAL      ");
    }
    else if(EngineTemp < TEMP_WARM_MAX)
    {
        str_LCD("WARMING     ");
    }
    else if(EngineTemp < TEMP_CRITICAL_MAX)
    {
        str_LCD("HIGH TEMP   ");
    }
    else
    {
        str_LCD("CRITICAL    ");
    }
}

/*=========================================================
 * Engine Overheat Warning
 *=========================================================*/

void Overheat_Warning(void)
{
    cmd_LCD(0x01);

    cmd_LCD(0x80);
    str_LCD(" !!! WARNING !!! ");

    cmd_LCD(0xC0);
    str_LCD("ENGINE OVERHEAT  ");

    cmd_LCD(0x94);
    str_LCD("STOP VEHICLE NOW ");

    cmd_LCD(0xD4);
    str_LCD("CHECK COOLING SYS");

    delay_s(2);

		return;
}

/*=========================================================
 * Startup Screen
 *=========================================================*/

void Startup_Screen(void)
{
    cmd_LCD(0x01);

    cmd_LCD(0x80);
    str_LCD(" CAN BASED ENGINE ");

    cmd_LCD(0xC0);
    str_LCD(" MONITORING SYSTEM");

    cmd_LCD(0x94);
    str_LCD(" VEHICLE CONTROL  ");

    cmd_LCD(0xD4);
    str_LCD(" INITIALIZING.... ");

    delay_s(2);

    cmd_LCD(0x01);
}

/*=========================================================
 * Dashboard Home Screen
 *=========================================================*/

void Dashboard_Home(void)
{
    cmd_LCD(0x01);

    /*---------------- Row 1 ----------------*/

    cmd_LCD(0x80);
    str_LCD("***VEC DASHBOARD***");

    /*---------------- Row 2 ----------------*/

    cmd_LCD(0xC0);
    str_LCD("TEMP:");

    Display_Temperature();
    cmd_LCD(0xCB);
    str_LCD(" MOD: FWD");

    /*---------------- Row 3 ----------------*/

    cmd_LCD(0x94);
    str_LCD("STATUS : ");

    Display_Engine_Status();

    /*---------------- Row 4 ----------------*/

    cmd_LCD(0xD4);
    str_LCD("CAN BUS : ACTIVE    ");

    if(EngineTemp >= TEMP_CRITICAL_MAX)
    {
        Overheat_Warning();
    }
}
