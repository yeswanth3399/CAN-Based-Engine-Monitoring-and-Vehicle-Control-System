#include <LPC21xx.h>

#include "CAN.h"
#include "CAN_defines.h"

/*====================================================================
 *                  CAN1 INITIALIZATION
 *====================================================================*/

void Init_CAN1(void)
{
    /*----------------------------------------------------
        Configure CAN1 Pins

        P0.25 -> CAN1 RD1
        P0.26 -> CAN1 TD1
    ----------------------------------------------------*/
		PINSEL1&=(~(3U<<((25-16)*2)));
    PINSEL1 |= (RD1_PIN);

    /*----------------------------------------------------
        Reset CAN Controller
    ----------------------------------------------------*/

    C1MOD = 1;

    /*----------------------------------------------------
        Bypass Acceptance Filter
        Accept all CAN Frames
    ----------------------------------------------------*/

    AFMR = 0x02;

    /*----------------------------------------------------
        Configure CAN Baud Rate
    ----------------------------------------------------*/

    C1BTR = BTR_LVAL;

    /*----------------------------------------------------
        Enter Normal Operating Mode
    ----------------------------------------------------*/

    C1MOD = 0;
}


/*====================================================================
 *                  CAN1 TRANSMIT
 *====================================================================*/

void CAN1_Tx(struct CAN_Frame txFrame)
{
    /* Wait for Transmit Buffer 1 Empty */

    while((C1GSR & TBS1_BIT_READ) == 0);

    /* Load CAN Identifier */

    C1TID1 = txFrame.ID;

    /* Load Frame Information */

    C1TFI1 = ((txFrame.vbf.RTR << 30) |
              (txFrame.vbf.DLC << 16));

    /* Load Data Bytes */

    if(txFrame.vbf.RTR == 0)
    {
        C1TDA1 = txFrame.Data1;

        C1TDB1 = txFrame.Data2;
    }

    /* Request Transmission */

    C1CMR = STB1_BIT_SET | TR_BIT_SET;

    /* Wait for Transmission Complete */

    while((C1GSR & TCS1_BIT_READ) == 0);
}


/*====================================================================
 *                  CAN1 RECEIVE
 *====================================================================*/

void CAN1_Rx(struct CAN_Frame *rxFrame)
{
    /* Wait for Receive Buffer Full */

    while((C1GSR & RBS_BIT_READ) == 0);

    /* Read CAN Identifier */

    rxFrame->ID = C1RID;

    /* Read RTR Bit */

    rxFrame->vbf.RTR = ((C1RFS >> 30) & 1);

    /* Read DLC */

    rxFrame->vbf.DLC = ((C1RFS >> 16) & 0x0F);

    /* Read Data Bytes */

    if(rxFrame->vbf.RTR == 0)
    {
        rxFrame->Data1 = C1RDA;

        rxFrame->Data2 = C1RDB;
    }

    /* Release Receive Buffer */

    C1CMR = RRB_BIT_SET;
}
