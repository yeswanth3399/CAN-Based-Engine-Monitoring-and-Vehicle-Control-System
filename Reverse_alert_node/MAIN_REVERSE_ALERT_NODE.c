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

static float Distance = 0.0f;

/* Reverse Mode Status */
static u8 ReverseMode = 0;

/* CAN Frames */
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
        Initialize GP2D12 Distance Sensor
    ----------------------------------------------------*/
    DistanceSensor_Init();

    /*----------------------------------------------------
        Configure CAN Transmit Frame
    ----------------------------------------------------*/
    TxFrame.ID      = CAN_ID_DISTANCE;
    TxFrame.vbf.RTR = 0;
    TxFrame.vbf.DLC = 1;

    TxFrame.Data1 = 0;
    TxFrame.Data2 = 0;

    /*====================================================
                    Main Loop
    ====================================================*/

    while(1)
    {
        /*------------------------------------------------
            Check for CAN Commands
        ------------------------------------------------*/
        if(C1GSR & RBS_BIT_READ)
				{
						CAN1_Rx(&RxFrame);

						switch(RxFrame.ID)
						{
								case CAN_ID_REVERSE_ENABLE:

										if(RxFrame.Data1 == REVERSE_ON)
										{
												ReverseMode = 1;
										}
										else if(RxFrame.Data1 == REVERSE_OFF)
										{
												ReverseMode = 0;
												Distance = 0.0f;

												TxFrame.Data1 = 0;
												TxFrame.Data2 = 0;
										}

										break;

								default:
										/* Ignore other CAN messages */
										break;
						}
				}

        /*------------------------------------------------
            Reverse Mode Active
        ------------------------------------------------*/
        if(ReverseMode)
        {
            /* Read GP2D12 Distance */
            Distance = Distance_Read();

            /* Limit distance */
            if(Distance < 0.0f)
            {
                Distance = 0.0f;
            }
            else if(Distance > 255.0f)
            {
                Distance = 255.0f;
            }

            /* Prepare CAN Frame */
            TxFrame.Data1 = (u8)(Distance + 0.5f);   // Rounded distance
            TxFrame.Data2 = 0;

            /* Send Distance to Main Node */
            CAN1_Tx(TxFrame);
			

            /* Update every 100 ms */
            delay_ms(100);
        }
    }
}
