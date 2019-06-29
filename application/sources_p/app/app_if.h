/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   30/1/2017
 * @brief:  app external signal define
 ******************************************************************************
**/
#ifndef __APP_IF_H__
#define __APP_IF_H__
#include <stdint.h>

/*****************************************************************************/
/*  getway if_rf24 define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
#define GW_IF_RF24_SEND_REQ							(0x01)

/*****************************************************************************/
/*  getway task rf24 define
 */
/*****************************************************************************/
/* define timer */
#define GW_RF24_IF_TIMER_PACKET_DELAY_INTERVAL		(100)

/* define signal */
#define GW_RF24_IF_PURE_MSG_OUT						(1)
#define GW_RF24_IF_COMMON_MSG_OUT					(2)
#define GW_RF24_IF_TIMER_PACKET_DELAY				(3)

/*****************************************************************************/
/*  getway task console define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
#define GW_CONSOLE_INTERNAL_LOGIN_CMD						(0x01)
#define GW_CONSOLE_AC_LOGIN_CMD								(0x02)

/*****************************************************************************/
/*  getway task rf433 define
 */
/*****************************************************************************/
/* define timer */
#define GW_RF433_TIMER_SMOKE_WARNNING_CHECK_INTERVAL		(20 * 60000)	/* 20' */
#define GW_RF433_TIMER_WATER_WARNNING_CHECK_INTERVAL		(20 * 60000)	/* 20' */

/* define signal */
#define GW_RF433_ENTRY_RECORD_CODE_REQ						(0x01)
#define GW_RF433_EXIT_RECORD_CODE_REQ						(0x02)

#define GW_RF433_RECORD_DOOR_OPENED_CODE_REQ				(0x03)
#define GW_RF433_RECORD_DOOR_CLOSED_CODE_REQ				(0x04)
#define GW_RF433_RECORD_SMOKE_WARNNING_CODE_REQ				(0x05)
#define GW_RF433_RECORD_WATER_WARNNING_CODE_REQ				(0x06)

#define GW_RF433_RECV_NOTIFY								(0x07)
#define GW_RF433_SMOKE_TIMER_WARNNING						(0x08)
#define GW_RF433_WATER_TIMER_WARNNING						(0x09)

/*****************************************************************************/
/*  getway task mqtt define
 */
/*****************************************************************************/
/* define timer */

/* define signal */
#define GW_MQTT_SENSOR_RES								(0x01)
#define GW_MQTT_SETTING_REQ								(0x02)
#define GW_MQTT_SETTING_RES								(0x03)
#define GW_MQTT_CONTROL_REQ								(0x04)
#define GW_MQTT_CONTROL_RES_OK							(0x05)
#define GW_MQTT_AC_SENSOR_REP							(0x06)
#define GW_MQTT_WR_SENSOR_REP							(0x07)
#define GW_MQTT_CONTROL_RES_ERR							(0x08)

/*****************************************************************************/
/*  getway task ac firmware define
 */
/*****************************************************************************/
/* define timer */
#define GW_TIMER_FIRMWARE_TRANFER_PACKET_PENDING_INTERVAL	(100)		/* 100ms */
#define GW_TIMER_FIRMWARE_PACKED_TIMEOUT_INTERVAL			(5000)		/* 5s */
#define GW_TIMER_FIRMWARE_GET_FIRMWARE_INFO_TIMEOUT			(2000)		/* 2s */
#define GW_TIMER_FIRMWARE_DEV_INTERNAL_UPDATE_TIMEOUT		(180000)	/* 3' */

/* define signal */
#define GW_AC_FW_OTA_REQ							(1)
#define GW_AC_FW_CURRENT_INFO_RES					(2)
#define GW_AC_FW_UPDATE_RES_OK						(3)
#define GW_AC_FW_TRANFER_REQ						(4)
#define GW_AC_FW_TRANSFER_RES_OK					(5)
#define GW_AC_FW_TRANSFER_CHECKSUM_ERR				(6)
#define GW_AC_FW_INTERNAL_UPDATE_REQ				(7)
#define GW_AC_FW_UPDATE_BUSY						(8)
#define GW_AC_FW_PACKED_TIMEOUT						(9)
#define GW_AC_FW_UPDATE_COMPLETED					(10)
#define GW_AC_FW_SM_UPDATE_RES_OK					(11)
#define GW_AC_FW_SM_RELEASE_RES_OK					(12)
#define GW_AC_FW_GET_FIRMWARE_INFO_TIMEOUT			(13)
#define GW_AC_FW_DEV_INTERNAL_UPDATE_TIMEOUT		(14)


/*****************************************************************************/
/*  getway task wr firmware define
 */
/*****************************************************************************/
/* define signal */
#define GW_WR_FW_OTA_REQ							(1)
#define GW_WR_FW_CURRENT_INFO_RES					(2)
#define GW_WR_FW_UPDATE_RES_OK						(3)
#define GW_WR_FW_TRANFER_REQ						(4)
#define GW_WR_FW_TRANSFER_RES_OK					(5)
#define GW_WR_FW_TRANSFER_CHECKSUM_ERR				(6)
#define GW_WR_FW_INTERNAL_UPDATE_REQ				(7)
#define GW_WR_FW_UPDATE_BUSY						(8)
#define GW_WR_FW_PACKED_TIMEOUT						(9)
#define GW_WR_FW_UPDATE_COMPLETED					(10)
#define GW_WR_FW_SM_UPDATE_RES_OK					(11)
#define GW_WR_FW_SM_RELEASE_RES_OK					(12)
#define GW_WR_FW_GET_FIRMWARE_INFO_TIMEOUT			(13)
#define GW_WR_FW_DEV_INTERNAL_UPDATE_TIMEOUT		(14)

/*****************************************************************************/
/* getway if task define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
#define GW_IF_PURE_MSG_IN							(1)
#define GW_IF_PURE_MSG_OUT							(2)
#define GW_IF_COMMON_MSG_IN							(3)
#define GW_IF_COMMON_MSG_OUT						(4)

/*****************************************************************************/
/* getway sensor task define
 */
/*****************************************************************************/
/* define timer */
#define GW_SENSOR_TIMER_AC_SENSOR_REPORT_REQ_INTERVAL	(10000)		/* 10s */
#define GW_SENSOR_TIMER_WR_SENSOR_REPORT_REQ_INTERVAL	(15000)		/* 15s */

/* define signal */
#define GW_SENSOR_AC_SENSOR_REPORT_REQ				(1)
#define GW_SENSOR_AC_SENSOR_REPORT_RES				(2)
#define GW_SENSOR_WR_SENSOR_REPORT_REQ				(3)
#define GW_SENSOR_WR_SENSOR_REPORT_RES				(4)

/*****************************************************************************/
/* snmp task define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
#define GW_SNMP_AC_SENSOR_REP						(1)
#define GW_SNMP_AC_SETTINGS_REP						(2)
#define GW_SNMP_WR_SENSOR_REP						(3)

/*****************************************************************************/
/* ac_sm task define
 */
/*****************************************************************************/
/* define timer */
#define GW_AC_SM_SENSOR_REPORT_TIMEOUT_INTERVAL				2000	/* 2s */
#define GW_AC_SM_HANDSHAKE_REQ_INTERVAL						4000	/* 4s */
#define GW_AC_SM_CONTROL_MODE_RETRY_INTERVAL				10000	/* 10s */
#define GW_AC_SM_AIR_COND_MANUAL_CONTROL_BUSY_RETRY_INTERVAL		15000	/* 15s */
#define GW_AC_SM_AC_SENSOR_REQ_PENDING_INTERVAL				500		/* 0.5s */

/* define signal */
#define GW_AC_SM_SENSOR_REPORT_REQ					(1)
#define GW_AC_SM_SENSOR_REPORT_RES					(2)
#define GW_AC_SM_FIRMWARE_UPDATE_REQ				(3)
#define GW_AC_SM_FIRMWARE_UPDATE_RELEASE_REQ		(4)
#define GW_AC_SM_HANDSHAKE_REQ						(5)
#define GW_AC_SM_SETTING_INFO_RES					(6)
#define GW_AC_SM_AIR_COND_INFO_RES					(7)
#define GW_AC_SM_SENSOR_REPORT_TIMEOUT				(8)
#define GW_AC_SM_LOST_CONNECTION					(9)
#define GW_AC_SM_IF_SETTING_INFO_REQ				(10)
#define GW_AC_SM_IF_AIR_COND_INFO_REQ				(11)
#define GW_AC_SM_IF_SETTING_INFO_RES				(12)
#define GW_AC_SM_IF_AIR_COND_INFO_RES				(13)
#define GW_AC_SM_SETTING_INFO_SYNC					(14)
#define GW_AC_SM_AIR_COND_INFO_SYNC					(15)
#define GW_AC_SM_SETTING_UPDATE_REQ					(16)
#define GW_AC_SM_AIR_COND_UPDATE_REQ				(17)
#define GW_AC_SM_SETTING_UPDATE_RES_OK				(18)
#define GW_AC_SM_AIR_COND_UPDATE_RES_OK				(19)
#define GW_AC_SM_CONTROL_MODE_RES_OK				(20)
#define GW_AC_SM_CONTROL_MODE_RES_BUSY				(21)
#define GW_AC_SM_AIR_COND_MANUAL_CONTROL_RES_OK		(22)
#define GW_AC_SM_AIR_COND_MANUAL_CONTROL_RES_ERR	(23)
#define GW_AC_SM_AIR_COND_MANUAL_CONTROL_RES_BUSY	(24)
#define GW_AC_SM_CONTROL_MODE_REQ					(25)
#define GW_AC_SM_AIR_COND_MANUAL_CONTROL_REQ		(26)
#define GW_AC_SM_CONTROL_MODE_RETRY					(27)
#define GW_AC_SM_AIR_COND_MANUAL_CONTROL_RETRY		(28)

/* private define */
#define GW_AC_SM_SENSOR_REPORT_TIMEOUT_MAX			10
#define GW_AC_SM_CONTROL_MODE_REQ_MAX				10
#define GW_AC_SM_AIR_COND_MANUAL_CONTROL_REQ_BUSY_MAX	10

/*****************************************************************************/
/* wr task define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
#define GW_WR_RF433_SENSOR_REPORT					(1)
#define GW_WR_SENSOR_REPORT_REQ						(2)
#define GW_WR_SENSOR_REPORT_RES						(3)
#define GW_WR_SENSOR_MILESTONES_SET_REQ				(4)
#define GW_WR_SENSOR_MILESTONES_SET_RES				(5)
#define GW_WR_SENSOR_MILESTONES_GET_REQ				(6)
#define GW_WR_SENSOR_MILESTONES_GET_RES				(7)
#define GW_WR_SENSOR_FW_ENTRY_REQ					(8)
#define GW_WR_SENSOR_FW_RELEASE_REQ					(9)

/*****************************************************************************/
/* debug task define
 */
/*****************************************************************************/
/* define timer */
/* define signal */
#define GW_DEBUG_GET_AC_SETTING_REQ					(1)
#define GW_DEBUG_GET_AC_AIR_COND_REQ				(2)
#define GW_DEBUG_GET_AC_SETTING_RES					(3)
#define GW_DEBUG_GET_AC_AIR_COND_RES				(4)
#define GW_DEBUG_SET_AC_SETTING_REQ					(5)
#define GW_DEBUG_SET_AC_AIR_COND_REQ				(6)
#define GW_DEBUG_SET_AC_CONTROL_MODE_REQ			(7)
#define GW_DEBUG_SET_AC_MANUAL_CONTROL_REQ			(8)

#endif //__APP_IF_H__
