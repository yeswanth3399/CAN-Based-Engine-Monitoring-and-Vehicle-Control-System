#ifndef __CAN_DEFINES_H__
#define __CAN_DEFINES_H__

/*====================================================================
 *
 *  CAN BASED ENGINE MONITORING & VEHICLE CONTROL SYSTEM
 *
 *  CAN DEFINITIONS
 *
 *====================================================================*/

/*====================================================================
 *                  CAN RX TX PIN DEFINITION
 *====================================================================*/

#define RD1_PIN    (0x15440000)

/*====================================================================
 *                  APPLICATION CAN IDENTIFIERS
 *====================================================================*/

/* Main Node -> Window Control Node */
#define CAN_ID_WINDOW_CTRL        (0x101)
#define CAN_ID_WINDOW_STATUS       0x201




/*====================================================================
 *                  WINDOW POSITION LEVELS (%)
 *====================================================================*/

#define WINDOW_CLOSE              (0)
#define WINDOW_25_PERCENT         (25)
#define WINDOW_50_PERCENT         (50)
#define WINDOW_75_PERCENT         (75)
#define WINDOW_FULL_OPEN          (100)

/*====================================================================
 *                  WINDOW CONTROL COMMANDS
 *====================================================================*/

#define WINDOW_STOP_CMD           (0x00)
#define WINDOW_UP_START_CMD       (0x01)
#define WINDOW_DOWN_START_CMD     (0x02)




/*====================================================================
 *                  CAN BAUD RATE CONFIGURATION
 *====================================================================*/

#define FOSC                     (12000000UL)
#define CCLK                     (FOSC * 5)
#define PCLK                     (CCLK / 4)

#define BIT_RATE                 (125000UL)

#define QUANTA                   (15)

#define BRP                      (PCLK / (BIT_RATE * QUANTA))

#define SAMPLE_POINT             (0.7 * QUANTA)

#define TSEG1                    ((int)SAMPLE_POINT - 1)

#define TSEG2                    (QUANTA - (1 + TSEG1))

#define SJW                      ((TSEG2 >= 5) ? 4 : (TSEG2 - 1))

#define SAM                      (0)

#define BTR_LVAL   ( (SAM << 23)         | \
                     ((TSEG2 - 1) << 20) | \
                     ((TSEG1 - 1) << 16) | \
                     ((SJW - 1) << 14)   | \
                     (BRP - 1) )

/*====================================================================
 *                  CAN COMMAND REGISTER BITS
 *====================================================================*/

#define TR_BIT_SET               (1U << 0)
#define RRB_BIT_SET              (1U << 2)
#define STB1_BIT_SET             (1U << 5)

/*====================================================================
 *                  CAN GLOBAL STATUS REGISTER BITS
 *====================================================================*/

#define RBS_BIT_READ             (1U << 0)
#define TBS1_BIT_READ            (1U << 2)
#define TCS1_BIT_READ            (1U << 3)



#endif
