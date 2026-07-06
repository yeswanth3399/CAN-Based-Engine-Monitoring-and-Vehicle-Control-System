/*====================================================================
 *                  LCD FUNCTION DECLARATIONS
 *====================================================================*/
#include "types.h"

void cmd_LCD(u8 cmd);      // Send command to LCD
void char_LCD(u8 ch);      // Send single character to LCD
void init_LCD(void);                  // Initialize LCD (8-bit mode)
void str_LCD(cs8 *str);              // Display string on LCD
void init_LCD_4(void);                // Alternate LCD initialization
void u32_LCD(u32 n);                  // Display integer number
void s32_LCD(s32 n);
void f32_LCD(f32 num);           // Display Float number
