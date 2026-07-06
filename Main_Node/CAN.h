#ifndef __CAN_H__
#define __CAN_H__

#include "types.h"

/*====================================================================
 *
 *      CAN BASED ENGINE MONITORING & VEHICLE CONTROL SYSTEM
 *
 *      CAN HEADER FILE
 *
 *====================================================================*/


/*====================================================================
 *                  CAN FRAME STRUCTURE
 *====================================================================*/

struct CAN_Frame
{
    /* CAN Message Identifier */
    u32 ID;

    /* Frame Information */
    struct BitField
    {
        u32 RTR : 1;      /* Remote Transmission Request */
        u32 DLC : 4;      /* Data Length Code */

    }vbf;

    /* CAN Data Bytes */
    u32 Data1;
    u32 Data2;
};


/*====================================================================
 *                  FUNCTION PROTOTYPES
 *====================================================================*/

/* Initialize CAN1 Controller */
void Init_CAN1(void);

/* Transmit CAN Frame */
void CAN1_Tx(struct CAN_Frame txFrame);

/* Receive CAN Frame */
void CAN1_Rx(struct CAN_Frame *rxFrame);

#endif
