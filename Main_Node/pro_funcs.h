#ifndef __PRO_FUNCS_H__
#define __PRO_FUNCS_H__
#include "types.h"
/*=========================================================
 *
 *      CAN BASED ENGINE MONITORING & VEHICLE CONTROL SYSTEM
 *
 *      PROJECT FUNCTION PROTOTYPES
 *
 *=========================================================*/


/*=========================================================
 *                  DASHBOARD FUNCTIONS
 *=========================================================*/

/* Vehicle Dashboard */
void Vehicle_Dashboard(void);
void Dashboard_Update(void);

/* Startup Screen */
void Startup_Screen(void);

/* Display Temperature */
void Display_Temperature(void);
void Read_Temperature(void);

/* Display Engine Status */
void Display_Engine_Status(void);

/* Engine Overheat Warning */
void Overheat_Warning(void);


/*=========================================================
 *                  WINDOW CONTROL FUNCTIONS
 *=========================================================*/

/* Window Up Screen */
void Window_Up_Mode(void);

/* Window Down Screen */
void Window_Down_Mode(void);

/* Window Opening Animation */
void Window_Up_Animation(void);

/* Window Closing Animation */
void Window_Down_Animation(void);


/*=========================================================
 *                  VEHICLE FUNCTIONS
 *=========================================================*/

/* Startup Display */
void vehicle(void);

/* Reverse Assist Screen */
void Reverse_Mode_Screen(void);

/* Active HIGH Buzzer */
void Buzzer_Beep(void);

void Return_To_Dashboard(void);

void show_window_error(void);
void show_reverse_error(void);


#endif
