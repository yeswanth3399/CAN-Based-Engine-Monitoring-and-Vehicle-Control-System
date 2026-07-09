#include <LPC21xx.h>

#include "CAN.h"
#include "CAN_defines.h"
#include "window_control.h"
#include "delays.h"

/*--------------------------------------------------------
                CAN Receive Frame
--------------------------------------------------------*/
struct CAN_Frame RxFrame;
struct CAN_Frame StatusFrame;
/*--------------------------------------------------------
                Window State
--------------------------------------------------------*/
typedef enum
{
    WINDOW_STOP = 0,
    WINDOW_UP,
    WINDOW_DOWN

}WINDOW_STATE_T;

WINDOW_STATE_T WindowState = WINDOW_STOP;

/*--------------------------------------------------------
                    MAIN
--------------------------------------------------------*/
int main(void)
{
    /* Initialize CAN */
    Init_CAN1();

    /* Initialize Window LEDs */
    Window_Init();
	
	StatusFrame.ID = CAN_ID_WINDOW_STATUS;
	StatusFrame.vbf.RTR =0;
	StatusFrame.vbf.DLC=1;
	StatusFrame.Data1=0;
	StatusFrame.Data2=0;


    while(1)
    {
        /*------------------------------------------
          Receive CAN Command
        ------------------------------------------*/
        if(CAN1_Rx(&RxFrame))
        {
            if(RxFrame.ID == CAN_ID_WINDOW_CTRL)
            {
                switch(RxFrame.Data1)
                {
                    case WINDOW_UP_START_CMD:

                        WindowState = WINDOW_UP;

                        break;

                    case WINDOW_DOWN_START_CMD:

                        WindowState = WINDOW_DOWN;

                        break;

                    case WINDOW_STOP_CMD:

                        WindowState = WINDOW_STOP;
                        break;

                    default:

                        break;
                }
            }
        }
		StatusFrame.Data1=WindowState;
		CAN1_Tx(StatusFrame);
		delay_ms(100);

        /*------------------------------------------
          Execute Window Movement
        ------------------------------------------*/

        switch(WindowState)
        {
            case WINDOW_UP:

                Window_Up();

                delay_ms(250);

                break;

            case WINDOW_DOWN:

                Window_Down();

                delay_ms(250);

                break;

            case WINDOW_STOP:

            default:

                break;
        }
    }
}
