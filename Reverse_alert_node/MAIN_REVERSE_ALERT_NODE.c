/*====================================================================
 *
 *      CAN BASED ENGINE MONITORING & VEHICLE CONTROL SYSTEM
 *
 *      REVERSE ALERT NODE
 *
 *====================================================================*/

#include <LPC21xx.h>

#include "CAN.h"
#include "CAN_defines.h"
#include "Distance_sensor.h"
#include "delays.h"

/*=========================================================
                GLOBAL VARIABLES
=========================================================*/

float Distance;

struct CAN_Frame TxFrame;
struct CAN_Frame RxFrame;

/*=========================================================
                MAIN FUNCTION
=========================================================*/

int main(void)
{
    /*----------------------------------------------------
        Initialize CAN Controller
    ----------------------------------------------------*/
    Init_CAN1();

    /*----------------------------------------------------
        Initialize GP2D12 Sensor (ADC)
    ----------------------------------------------------*/
    DistanceSensor_Init();

    /*----------------------------------------------------
        Configure CAN Transmit Frame
    ----------------------------------------------------*/
    TxFrame.ID      = CAN_ID_DISTANCE;
    TxFrame.vbf.RTR = 0;
    TxFrame.vbf.DLC = 1;

    /*----------------------------------------------------
        Main Loop
    ----------------------------------------------------*/
    while(1)
    {
        /* Wait for CAN command from Main Node */
        if(C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&RxFrame);

            /*--------------------------------------------
              Reverse Mode ON
            --------------------------------------------*/
            if((RxFrame.ID == CAN_ID_REVERSE_ENABLE) &&
               (RxFrame.Data1 == REVERSE_ON))
            {
                /* Read Distance */
                Distance = Distance_Read();

                /* Limit to one CAN data byte */
                if(Distance > 255.0f)
                    Distance = 255.0f;

                if(Distance < 0.0f)
                    Distance = 0.0f;

                /* Send Distance */
                TxFrame.Data1 = (unsigned int)Distance;
                TxFrame.Data2 = 0;

                CAN1_Tx(TxFrame);

                delay_ms(100);
            }

            /*--------------------------------------------
              Reverse Mode OFF
            --------------------------------------------*/
            else if((RxFrame.ID == CAN_ID_REVERSE_ENABLE) &&
                    (RxFrame.Data1 == REVERSE_OFF))
            {
                /* Nothing to do.
                   Main Node handles buzzer and display. */
            }
        }
    }
}
