/**
 ******************************************************************************
 * @Author: ThanNT
 * @Date:   13/08/2016
 ******************************************************************************
**/

#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "app_if.h"
#include "app_eeprom.h"

/*****************************************************************************/
/*  task OTB_LIFE define
 */
/*****************************************************************************/
/* define timer */
#define OTB_LIFE_TASK_TIMER_LED_LIFE_INTERVAL		(1000)

/* define signal */
#define OTB_LIFE_SYSTEM_CHECK						(0)

/*****************************************************************************/
/*  task OTB_SHELL define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
#define OTB_SHELL_LOGIN_CMD							(0)
#define OTB_SHELL_REMOTE_CMD							(1)

/*****************************************************************************/
/*  task OTB_RF24 define
 */
/*****************************************************************************/
/* define timer */
#define OTB_RF24_IF_TIMER_PACKET_DELAY_INTERVAL		(100)	/* 100 ms */
#define OTB_RF24_IF_POLLING_INTERVAL					(100)	/* 100ms */

/* define signal */
/* interrupt signal */
#define OTB_RF24_IF_IRQ_TX_FAIL						(1)
#define OTB_RF24_IF_IRQ_TX_SUCCESS						(2)
#define OTB_RF24_IF_IRQ_RX_READY						(3)

#define OTB_RF24_IF_INIT_NETWORK						(4)
#define OTB_RF24_IF_PURE_MSG_OUT						(5)
#define OTB_RF24_IF_COMMON_MSG_OUT						(6)
#define OTB_RF24_IF_TIMER_PACKET_DELAY					(7)

/*****************************************************************************/
/* task OTB_IF define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
#define OTB_IF_PURE_MSG_IN								(1)
#define OTB_IF_PURE_MSG_OUT							(2)
#define OTB_IF_COMMON_MSG_IN							(3)
#define OTB_IF_COMMON_MSG_OUT							(4)

/*****************************************************************************/
/* task rs485_modbus define
 */
/*****************************************************************************/
/* define timer */
#define OTB_RS485_SLAVE_UPDATE_INTERVAL				(20)
#define OTB_RS485_SLAVE_INIT_INTERVAL					(500)
enum {
	/* define signal */
	OTB_RS485_INIT = 1					,
	OTB_RS485_SLAVE_UPDATE				,
	OTB_RS485_SLAVE_FUN_1_CONTROL_REQ	,
	OTB_RS485_SLAVE_FUN_2_CONTROL_REQ	,
	OTB_RS485_SLAVE_FUN_3_CONTROL_REQ	,
	OTB_RS485_SLAVE_FUN_4_CONTROL_REQ	,
	OTB_RS485_SLAVE_FUN_5_CONTROL_REQ	,
	OTB_RS485_SLAVE_FUN_6_CONTROL_REQ	,

	OTB_RS485_SLAVE_DEVICE_1_RESET_REQ	,
	OTB_RS485_SLAVE_DEVICE_2_RESET_REQ	,
	OTB_RS485_SLAVE_DEVICE_3_RESET_REQ	,
	OTB_RS485_SLAVE_DEVICE_4_RESET_REQ	,
	OTB_RS485_SLAVE_DEVICE_5_RESET_REQ	,
	OTB_RS485_SLAVE_DEVICE_6_RESET_REQ	,
	OTB_RS485_SLAVE_RESET_REQ
};
/*****************************************************************************/
/*  app function declare
 */
/*****************************************************************************/
#define SL_AC_NUMBER_SAMPLE_CT_SENSOR				2000
#define SL_TOTAL_CT_SENSOR							(8)
#define SL_SENSOR_CHECKING_REQ						(0)
/*****************************************************************************/
/*  global define variable
 */
/*****************************************************************************/
#define APP_OK										(0x00)
#define APP_NG										(0x01)

#define APP_FLAG_OFF								(0x00)
#define APP_FLAG_ON									(0x01)

extern int  main_app();

#ifdef __cplusplus
}
#endif

#endif //APP_H
