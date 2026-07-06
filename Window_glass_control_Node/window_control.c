#include <LPC21xx.h>

#include "window_control.h"

#define WINDOW_LED_MASK    (0xFFU)      /* P0.0 - P0.7 */

static unsigned char WindowPosition = 0;

/*--------------------------------------------------
  Update LEDs according to position
--------------------------------------------------*/
static void Update_Window_Position(void)
{
    unsigned int i;

    /* Active LOW LEDs */

    IOSET0 = WINDOW_LED_MASK;

    for(i = 0; i < WindowPosition; i++)
    {
        IOCLR0 = (1U << i);
    }
}

/*--------------------------------------------------
  Initialize Window
--------------------------------------------------*/
void Window_Init(void)
{
    IODIR0 |= WINDOW_LED_MASK;

    WindowPosition = 0;

    Update_Window_Position();
}

/*--------------------------------------------------
  Move Window Up by One Step
--------------------------------------------------*/
void Window_Up(void)
{
    if(WindowPosition < 8)
    {
        WindowPosition++;

        Update_Window_Position();
    }
}

/*--------------------------------------------------
  Move Window Down by One Step
--------------------------------------------------*/
void Window_Down(void)
{
    if(WindowPosition > 0)
    {
        WindowPosition--;

        Update_Window_Position();
    }
}
