#ifndef __EINT_H__
#define __EINT_H__

/*====================================================================
 *
 *      CAN BASED ENGINE MONITORING & VEHICLE CONTROL SYSTEM
 *
 *      EXTERNAL INTERRUPT HEADER FILE
 *
 *====================================================================*/


/*====================================================================
 *                  SYSTEM OPERATING MODES
 *====================================================================*/

typedef enum
{
    DASHBOARD_MODE = 0,

    WINDOW_UP_MODE,

    WINDOW_DOWN_MODE,

    REVERSE_MODE

} MODE_T;


/*====================================================================
 *                  CURRENT SYSTEM MODE
 *====================================================================*/

extern volatile MODE_T CurrentMode;


/*====================================================================
 *                  ISR PROTOTYPES
 *====================================================================*/

/* SW1 -> Window Up */
void EINT0_ISR(void) __irq;

/* SW2 -> Window Down */
void EINT1_ISR(void) __irq;

/* SW3 -> Reverse Mode */
void EINT2_ISR(void) __irq;


/*====================================================================
 *                  INTERRUPT INITIALIZATION
 *====================================================================*/

void Interrupt_Init(void);

#endif
