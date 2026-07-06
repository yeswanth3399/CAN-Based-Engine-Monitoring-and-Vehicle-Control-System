#ifndef __DISTANCE_SENSOR_H__
#define __DISTANCE_SENSOR_H__

#include "types.h"

/*--------------------------------------------------------
            GP2D12 Distance Sensor
--------------------------------------------------------*/

/* Initialize ADC */
void DistanceSensor_Init(void);

/* Read Distance (cm) */
float Distance_Read(void);

#endif
