/*====================================================================
 *
 *      GP2D12 DISTANCE SENSOR DRIVER
 *
 *====================================================================*/

#include <LPC21xx.h>

#include "distance_sensor.h"
#include "adc.h"
#include "delays.h"

/*====================================================================
 *                  GP2D12 CONFIGURATION
 *====================================================================*/

/* ADC Channel */
#define GP2D12_ADC_CHANNEL        (0)

/* Number of ADC samples for averaging */
#define GP2D12_SAMPLE_COUNT       (5)

/* Sensor update interval */
#define GP2D12_UPDATE_TIME_MS     (40)

/* Sensor operating limits */
#define GP2D12_MIN_DISTANCE_CM    (10.0f)
#define GP2D12_MAX_DISTANCE_CM    (80.0f)

/* Voltage corresponding approximately to 80 cm */
#define GP2D12_MIN_VOLTAGE        (0.42f)

/* Distance conversion constant */
#define GP2D12_DISTANCE_CONSTANT  (27.86f)

/*====================================================================
 *                  LOCAL VARIABLES
 *====================================================================*/

static u16 DigitalValue;
static f32 AnalogVoltage;

/*====================================================================
 *                  INITIALIZE SENSOR
 *====================================================================*/

void DistanceSensor_Init(void)
{
    Init_ADC();
}

/*====================================================================
 *                  READ DISTANCE
 *====================================================================*/

float Distance_Read(void)
{
    u8 i;

    float VoltageSum = 0.0f;

    float distance;

    /*-----------------------------------------
        Take multiple ADC samples
    -----------------------------------------*/

    for(i = 0; i < GP2D12_SAMPLE_COUNT; i++)
    {
        Read_ADC(GP2D12_ADC_CHANNEL,
                 &DigitalValue,
                 &AnalogVoltage);

        VoltageSum += AnalogVoltage;

        delay_ms(2);
    }

    /*-----------------------------------------
        Average Voltage
    -----------------------------------------*/

    AnalogVoltage = VoltageSum / GP2D12_SAMPLE_COUNT;

    /*-----------------------------------------
        Convert Voltage -> Distance
    -----------------------------------------*/

    if(AnalogVoltage <= GP2D12_MIN_VOLTAGE)
    {
        distance = GP2D12_MAX_DISTANCE_CM;
    }
    else
    {
        distance =
        GP2D12_DISTANCE_CONSTANT /
        (AnalogVoltage - GP2D12_MIN_VOLTAGE);
    }

    /*-----------------------------------------
        Limit Distance
    -----------------------------------------*/

    if(distance < GP2D12_MIN_DISTANCE_CM)
    {
        distance = GP2D12_MIN_DISTANCE_CM;
    }
    else if(distance > GP2D12_MAX_DISTANCE_CM)
    {
        distance = GP2D12_MAX_DISTANCE_CM;
    }

    /*-----------------------------------------
        Sensor Update Interval
    -----------------------------------------*/

    delay_ms(GP2D12_UPDATE_TIME_MS);

    return distance;
}
	