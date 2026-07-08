//i2c_1wire_ds18b20.c
#include <LPC21xx.h>
#include "types.h"
#include "delays.h"

#define DS18B20_PIN		22//p0.22
/*Standard command sequence for ds18b20
1. Reset Pulse Initialize 1-Wire Bus
2. SKIP ROM (0xCC)
3. CONVERT TEMP (0x44)
4. WAIT for 750ms
5. SKIP ROM (0xCC)
7. READ Scratch pad (0xBE)
*/

//cmds fro ds18b20
#define SKIP_ROM 			0xCC
#define CONVERT_TEMP		0x44
#define READ_SCRATCH_PAD	0xBE

//=====================================================
// LOW LEVEL GPIO CONTROL
//=====================================================

void DQ_OUT(void)
{
	IODIR0 |= (1<<DS18B20_PIN);
}

void DQ_IN(void)
{
	IODIR0 &= ~(1<<DS18B20_PIN);
}

void DQ_LOW(void)
{
	IOCLR0 = (1<<DS18B20_PIN);
}

u8 DQ_READ(void)
{
	return ((IOPIN0>>DS18B20_PIN)&1);
}


u8 ds18b20_reset(void)
{
	u8 presence;

	DQ_OUT();

	DQ_LOW();

	delay_us(500);

	DQ_IN();

	delay_us(70);

	presence=DQ_READ();

	delay_us(430);

	return presence;
}

void ds18b20_write_bit(u8 bit)
{
	DQ_OUT();

	DQ_LOW();

	if(bit)
	{
		delay_us(6);

		DQ_IN();

		delay_us(64);
	}
	else
	{
		delay_us(60);

		DQ_IN();

		delay_us(10);
	}
}
u8 ds18b20_read_bit(void)
{
	u8 bit;

	DQ_OUT();

	DQ_LOW();

	delay_us(3);

	DQ_IN();

	delay_us(10);

	bit=DQ_READ();

	delay_us(53);

	return bit;
}
void ds18b20_write_byte(u8 data)
{
	u8 i;

	for(i=0;i<8;i++)
	{
		ds18b20_write_bit((data>>i)&1);
	}
}

u8 ds18b20_read_byte(void)
{
	u8 i,data=0;

	for(i=0;i<8;i++)
	{
		if(ds18b20_read_bit())
		{
			data |= (1<<i);
		}
	}

	return data;
}

f32 ds18b20_read_temperature(void)
{
	u8 lsb,msb;

	s16 temp;

	f32 temperature;

	ds18b20_reset();

	ds18b20_write_byte(SKIP_ROM);

	ds18b20_write_byte(CONVERT_TEMP);

	/* Wait for conversion to complete */
	delay_ms(100);

	ds18b20_reset();

	ds18b20_write_byte(SKIP_ROM);

	ds18b20_write_byte(READ_SCRATCH_PAD);

	lsb=ds18b20_read_byte();

	msb=ds18b20_read_byte();

	temp=((msb<<8)|lsb);

	temperature=((float)temp/16);

	return temperature;
}
