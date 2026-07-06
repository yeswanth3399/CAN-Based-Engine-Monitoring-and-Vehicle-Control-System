#ifndef __DASHBOARD_H__
#define __DASHBOARD_H__

/*====================================================================
 *
 *      CAN BASED ENGINE MONITORING & VEHICLE CONTROL SYSTEM
 *
 *      DASHBOARD DISPLAY HEADER FILE
 *
 *====================================================================*/


/*=========================================================
 * Temperature Functions
 *=========================================================*/

/* Read DS18B20 Temperature and Display */
void Display_Temperature(void);

/* Display Engine Health Status */
void Display_Engine_Status(void);


/*=========================================================
 * Warning Functions
 *=========================================================*/

/* Engine Overheat Warning Screen */
void Overheat_Warning(void);


/*=========================================================
 * Dashboard Screens
 *=========================================================*/

/* System Startup Screen */
void Startup_Screen(void);

/* Main Dashboard Screen */
void Dashboard_Home(void);

#endif
