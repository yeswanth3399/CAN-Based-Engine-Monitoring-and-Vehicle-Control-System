/*====================================================================
 *                  LCD HEADER & DEFINITIONS
 *====================================================================*/

#include <LPC21xx.h>          // Include LPC21xx register definitions
#include "delays.h"            // Include delay functions

#define DATA (0xFF)        // Data lines connected to P0.0-P0.7
#define EN   (1<<8)           // Enable pin (P0.8)
#define RS   (1<<9)           // Register Select pin (P0.9)
#define RW   (1<<10)           // Read/Write pin (P0.10)



/*====================================================================
 *                  LCD COMMAND FUNCTION
 *====================================================================*/
void cmd_LCD(unsigned char cmd)      // Function to send command to LCD
{
    IOCLR0 = DATA;                   // Clear previous data on bus
    IOCLR0 = RS;                     // RS = 0 ? Command mode
    IOCLR0 = RW;                     // RW = 0 ? Write operation
    
    IOSET0 = (cmd) & DATA;        // Place command on data pins (shift to P0.0 to P0.7)
    
    IOSET0 = EN;                     // Enable = HIGH (start command latch)
    delay_ms(2);                     // Small delay
    IOCLR0 = EN;                     // Enable = LOW (latch command)
    delay_ms(2);                     // Wait for LCD to process
}


/*====================================================================
 *                  LCD CHARACTER FUNCTION
 *====================================================================*/
void char_LCD(unsigned char ch)      // Function to send character to LCD
{
    IOCLR0 = DATA;                   // Clear previous data
    IOSET0 = RS;                     // RS = 1 ? Data mode
    IOCLR0 = RW;                     // RW = 0 ? Write operation
    
    IOSET0 = (ch) & DATA;         // Put character on data pins
    
    IOSET0 = EN;                     // Enable HIGH
    delay_ms(2);                     // Delay
    IOCLR0 = EN;                     // Enable LOW
    delay_ms(2);                     // Wait for execution
}


/*====================================================================
 *                  LCD INITIALIZATION (8-BIT MODE)
 *====================================================================*/
void init_LCD(void)                  // Initialize LCD
{
    IODIR0 |= DATA | RS | RW | EN;   // Set all LCD pins as output
    
    delay_ms(20);                    // Wait after power ON
		cmd_LCD(0x38);                   // Function set: 8-bit, 2-line
    cmd_LCD(0x0C);                   // Display ON, cursor OFF
    cmd_LCD(0x06);                   // Entry mode (cursor increment)
    cmd_LCD(0x01);                   // Clear display
}


/*====================================================================
 *                  STRING DISPLAY FUNCTION
 *====================================================================*/
void str_LCD(const char *str)              // Function to display string
{
    while(*str)                      // Loop until null character
    {
        char_LCD(*str++);            // Send each character and move pointer
    }
}


/*=========================================================
 * Display Unsigned 32-bit Integer
 *=========================================================*/

void u32_LCD(u32 n)
{
    u8 ch[10];
    s32 i = 0;

    if(n == 0)
    {
        char_LCD('0');
        return;
    }

    while(n)
    {
        ch[i++] = (n % 10) + '0';
        n /= 10;
    }

    while(i)
    {
        char_LCD(ch[--i]);
    }
}

/*=========================================================
 * Display Signed 32-bit Integer
 *=========================================================*/

void s32_LCD(s32 n)
{
    if(n < 0)
    {
        char_LCD('-');
        n = -n;
    }

    u32_LCD((u32)n);
}

/*=========================================================
 * Display Float (2 Decimal Places)
 *=========================================================*/

void f32_LCD(f32 num)
{
    s32 intPart;
    u32 fracPart;

    if(num < 0)
    {
        char_LCD('-');
        num = -num;
    }

    intPart = (s32)num;

    fracPart = (u32)((num - intPart) * 100.0f);

    s32_LCD(intPart);

    char_LCD('.');

    if(fracPart < 10)
    {
        char_LCD('0');
    }

    u32_LCD(fracPart);
}
