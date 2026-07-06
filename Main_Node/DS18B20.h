//==================== FUNCTION DECLARATIONS ====================

//i2c_1wire_ds18b20.h
#include "types.h"

// Reset DS18B20 Sensor
u8 ds18b20_reset(void);

// Read Single Bit from DS18B20
u8 ds18b20_read_bit(void);

// Write Single Bit to DS18B20
void ds18b20_write_bit(u8 bit);

// Read One Byte from DS18B20
u8 ds18b20_read_byte(void);

// Write One Byte to DS18B20
void ds18b20_write_byte(u8 data);

// Read Temperature from DS18B20
f32 ds18b20_read_temperature(void);
