#include <LPC21xx.h>

#include "CAN.h"
#include "CAN_defines.h"
#include "LCD.h"
#include "EINT.h"
#include "delays.h"
#include "pro_funcs.h"

/*=========================================================
 * Global Variables
 *=========================================================*/

volatile MODE_T CurrentMode = DASHBOARD_MODE;

volatile u8 ReverseDistance = 0;

volatile MODE_T PreviousMode;

volatile u8 WindowStopPending = 0;
volatile u8 ReverseStopPending = 0;


volatile u16 WindowTimeout=0;
volatile u16 ReverseTimeout=0;

/*=========================================================
 * CAN Frame Structures
 *=========================================================*/

struct CAN_Frame TX_frame1;
struct CAN_Frame TX_frame2;
struct CAN_Frame Rx_frame;

/*=========================================================
 * MAIN FUNCTION
 *=========================================================*/

int main(void)
{
	
    /*-----------------------------------------------------
     * LCD Initialization
     *----------------------------------------------------*/
    init_LCD();

    cmd_LCD(0x01);
    cmd_LCD(0x80);
    str_LCD("LCD OK");
    delay_ms(500);

    /*-----------------------------------------------------
     * CAN Initialization
     *----------------------------------------------------*/
    //Init_CAN1();

    cmd_LCD(0x01);
    cmd_LCD(0x80);
    str_LCD("CAN OK");
    delay_ms(500);

    /*-----------------------------------------------------
     * Interrupt Initialization
     *----------------------------------------------------*/
    Interrupt_Init();

    cmd_LCD(0x01);
    cmd_LCD(0x80);
    str_LCD("EINT OK");
    delay_ms(500);

    /*-----------------------------------------------------
     * Buzzer Output
     *----------------------------------------------------*/
    IODIR0 |= (1U << 21);

    /* Active HIGH buzzer OFF */
    IOCLR0 = (1U << 21);

    /*-----------------------------------------------------
     * Window Control CAN Frame
     *----------------------------------------------------*/
    TX_frame1.ID      = CAN_ID_WINDOW_CTRL;
    TX_frame1.vbf.RTR = 0;
    TX_frame1.vbf.DLC = 1;
		TX_frame1.Data1 = 0;
		TX_frame1.Data2 = 0;
    /*-----------------------------------------------------
     * Reverse Alert CAN Frame
     *----------------------------------------------------*/
    TX_frame2.ID      = CAN_ID_REVERSE_ENABLE;
    TX_frame2.vbf.RTR = 0;
    TX_frame2.vbf.DLC = 1;
		TX_frame2.Data1 = 0;
		TX_frame2.Data2 = 0;

    /*-----------------------------------------------------
     * Startup Screen
     *----------------------------------------------------*/
		vehicle();

		Return_To_Dashboard();

		Vehicle_Dashboard();

		Dashboard_Update();

		PreviousMode = DASHBOARD_MODE;
    /*=====================================================
     * Main Loop
     *====================================================*/

    while(1)
    {
				
        switch(CurrentMode)
        {
            /*=================================================
             * WINDOW UP MODE
             *================================================*/
							case WINDOW_UP_MODE:

										if(PreviousMode != WINDOW_UP_MODE)
										{
												Window_Up_Mode();

												TX_frame1.Data1 = WINDOW_UP_START_CMD;
												TX_frame1.Data2 = 0;

												CAN1_Tx(TX_frame1);
												WindowTimeout=0;
												//WindowNodeAlive=0;

												
												WindowStopPending = 1;

												PreviousMode = WINDOW_UP_MODE;
										}

										Window_Up_Animation();
										Dashboard_Update();
										if(CAN1_Rx(&Rx_frame))
										{		
											if(Rx_frame.ID == CAN_ID_WINDOW_STATUS)
											{
												WindowTimeout=0;
												//WindowNodeAlive=1;
											}
										}
										else
										{
											WindowTimeout++;
										}
										if(WindowTimeout>50)
										{
											show_window_error();
											
											PreviousMode=CurrentMode;
											CurrentMode=DASHBOARD_MODE;
											WindowTimeout=0;
										}
										delay_ms(30);

										break;
            /*=================================================
             * WINDOW DOWN MODE
             *================================================*/
						
							case WINDOW_DOWN_MODE:

									if(PreviousMode != WINDOW_DOWN_MODE)
									{
												Window_Down_Mode();

												TX_frame1.Data1 = WINDOW_DOWN_START_CMD;
												TX_frame1.Data2 = 0;

												CAN1_Tx(TX_frame1);
												WindowTimeout=0;
												//WindowNodeAlive=0;


												WindowStopPending = 1;

												PreviousMode = WINDOW_DOWN_MODE;
									}

									Window_Down_Animation();
									Dashboard_Update();
									if(CAN1_Rx(&Rx_frame))
									{
											if(Rx_frame.ID == CAN_ID_WINDOW_STATUS)
											{
												WindowTimeout=0;
												//WindowNodeAlive=1;
											}
									}
									else
									{
											WindowTimeout++;
									}
									if(WindowTimeout>50)
									{
											show_window_error();
											PreviousMode=CurrentMode;
											CurrentMode=DASHBOARD_MODE;
											WindowTimeout=0;
									}
									delay_ms(30);

									break;            
						/*=================================================
						* REVERSE MODE
						*================================================*/
						case REVERSE_MODE:

								if(PreviousMode != REVERSE_MODE)
								{
										Reverse_Mode_Screen();

										TX_frame2.Data1 = REVERSE_ON;
										TX_frame2.Data2 = 0;

										CAN1_Tx(TX_frame2);

										

										ReverseStopPending = 1;

										PreviousMode = REVERSE_MODE;
								}

								if(CAN1_Rx(&Rx_frame))
								{
										if(Rx_frame.ID == CAN_ID_DISTANCE)
										{
												//ReverseNodeAlive=1;
												ReverseTimeout=0;
												ReverseDistance = Rx_frame.Data1;

												cmd_LCD(0xC0);
												str_LCD("DIST : ");

												u32_LCD(ReverseDistance);

												str_LCD(" CM   ");

												if(ReverseDistance > SAFE_ZONE_LIMIT)
												{
														cmd_LCD(0x94);
														str_LCD("ZONE : SAFE      ");

														cmd_LCD(0xD4);
														str_LCD("PATH CLEAR       ");
												}
												else if(ReverseDistance > WARNING_ZONE_LIMIT)
												{
														cmd_LCD(0x94);
														str_LCD("ZONE : WARNING   ");

														cmd_LCD(0xD4);
														str_LCD("OBJECT NEARBY    ");
												}
												else if(ReverseDistance > DANGER_ZONE_LIMIT)
												{
														cmd_LCD(0x94);
														str_LCD("ZONE : DANGER    ");

														cmd_LCD(0xD4);
														str_LCD("SLOW DOWN        ");
												}
												else
												{
														Buzzer_Beep();

														cmd_LCD(0x94);
														str_LCD("ZONE : CRITICAL  ");

														cmd_LCD(0xD4);
														str_LCD("STOP VEHICLE     ");
												}
										}
								}
								else
								{
									ReverseTimeout++;
								}
								if(ReverseTimeout>50)
								{
									show_reverse_error();

									ReverseTimeout=0;
							//		ReverseNodeAlive=0;

									CurrentMode=DASHBOARD_MODE;
									PreviousMode=DASHBOARD_MODE;
									ReverseStopPending=0;
									break;
								}


								delay_ms(30);

								break;

            /*=================================================
             * DASHBOARD MODE
             *================================================*/
            case DASHBOARD_MODE:
							WindowTimeout=0;
							ReverseTimeout=0;

							if(WindowStopPending)
							{
									TX_frame1.Data1 = WINDOW_STOP_CMD;
									TX_frame1.Data2 = 0;

									CAN1_Tx(TX_frame1);

									WindowStopPending = 0;
							}

							if(ReverseStopPending)
							{
									TX_frame2.Data1 = REVERSE_OFF;
									TX_frame2.Data2 = 0;

									CAN1_Tx(TX_frame2);

									ReverseStopPending = 0;
							}
							if(CAN1_Rx(&Rx_frame))
							{
								switch(Rx_frame.ID)
								{
									case CAN_ID_WINDOW_STATUS:
										WindowTimeout=0;
										break;

									case CAN_ID_DISTANCE:
										ReverseTimeout=0;
										break;

								}
							 }

							if(PreviousMode != DASHBOARD_MODE)
							{
									Vehicle_Dashboard();

									PreviousMode = DASHBOARD_MODE;
							}

							Dashboard_Update();

							delay_ms(10);

							break;
						
					default:

						CurrentMode = DASHBOARD_MODE;

						break;
        }
    }
}
