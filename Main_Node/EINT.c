/*====================================================================
 *
 *      CAN BASED ENGINE MONITORING & VEHICLE CONTROL SYSTEM
 *
 *      EXTERNAL INTERRUPT MODULE
 *
 *====================================================================*/

#include <LPC21xx.h>
#include "EINT.h"
#include "pro_funcs.h"

#define EINT0_PIN_FUNC	0x15400001	//p0.16
#define EINT1_PIN_FUNC  0x20000000  //p0.14
#define EINT2_PIN_FUNC  0x80000000	//p0.15
/*====================================================================
 *                  EINT0 ISR (WINDOW UP)
 *====================================================================*/

void EINT0_ISR(void) __irq
{
    EXTINT = (1<<0);

    /* Falling edge -> Switch Pressed */
    if((EXTPOLAR & (1<<0)) == 0)
    {
        CurrentMode = WINDOW_UP_MODE;

        /* Next interrupt on release */
        EXTPOLAR|= (1<<0);
    }
    /* Rising edge -> Switch Released */
    else
    {
			  Return_To_Dashboard();
        CurrentMode = DASHBOARD_MODE;

        /* Next interrupt on press */
        EXTPOLAR &= ~(1<<0);
    }

    VICVectAddr = 0;
}


/*====================================================================
 *                  EINT1 ISR (WINDOW DOWN)
 *====================================================================*/

void EINT1_ISR(void) __irq
{
    EXTINT = (1<<1);

    /* Falling edge -> Switch Pressed */
    if((EXTPOLAR & (1<<1)) == 0)
    {
        CurrentMode = WINDOW_DOWN_MODE;

        EXTPOLAR|= (1<<1);
    }
    /* Rising edge -> Switch Released */
    else
    {
			  Return_To_Dashboard();
        CurrentMode = DASHBOARD_MODE;

        EXTPOLAR &= ~(1<<1);
    }

    VICVectAddr = 0;
}


/*====================================================================
 *                  EINT2 ISR (REVERSE MODE)
 *====================================================================*/

void EINT2_ISR(void) __irq
{
    EXTINT = (1<<2);

    /* Falling edge -> Reverse Switch Pressed */
    if(CurrentMode!= REVERSE_MODE)
    {
        CurrentMode = REVERSE_MODE;
    }
    /* Rising edge -> Reverse Switch Released */
    else
    {
			  Return_To_Dashboard();
        CurrentMode = DASHBOARD_MODE;
		      EXTPOLAR&=~(1<<2);
    }

    VICVectAddr = 0;
}


/*====================================================================
 *                  INTERRUPT INITIALIZATION
 *====================================================================*/

void Interrupt_Init(void)
{
    /*----------------------------------------------------
        Configure External Interrupt Pins
    ----------------------------------------------------*/

    /* P0.16 -> EINT0 */
    PINSEL1 &= ~(3U << ((16*2)-16));
    PINSEL1 |= EINT0_PIN_FUNC ;

    /* P0.14 -> EINT1 */
    PINSEL0 &= ~(3U << (14*2));
    PINSEL0 |=  EINT1_PIN_FUNC;

    /* P0.15 -> EINT2 */
    PINSEL0 &= ~(3U << (15*2));
    PINSEL0 |=  EINT2_PIN_FUNC;

    /*----------------------------------------------------
        Edge Triggered Interrupts
    ----------------------------------------------------*/

    EXTMODE |= (1U << 0) |
               (1U << 1) |
               (1U << 2);

    /* Falling Edge Trigger */

    EXTPOLAR &= ~((1U << 0) |
                  (1U << 1) |
                  (1U << 2));

    /*----------------------------------------------------
        Clear Pending Interrupts
    ----------------------------------------------------*/

    EXTINT = 0x07;

    /*----------------------------------------------------
        Enable Interrupts
    ----------------------------------------------------*/

    VICIntEnable |= (1U << 14) |
                    (1U << 15) |
                    (1U << 16);

    /*----------------------------------------------------
        Vector Slot 0 -> EINT0
    ----------------------------------------------------*/

    VICVectCntl0 = (1U << 5) | 14;

    VICVectAddr0 = (unsigned int)EINT0_ISR;

    /*----------------------------------------------------
        Vector Slot 1 -> EINT1
    ----------------------------------------------------*/

    VICVectCntl1 = (1U << 5) | 15;

    VICVectAddr1 = (unsigned int)EINT1_ISR;

    /*----------------------------------------------------
        Vector Slot 2 -> EINT2
    ----------------------------------------------------*/

    VICVectCntl2 = (1U << 5) | 16;

    VICVectAddr2 = (unsigned int)EINT2_ISR;
}
