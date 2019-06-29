#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "../ak/ak.h"
#include "../ak/task.h"

extern task_t app_task_table[];

/*****************************************************************************/
/*  DECLARE: Internal Task ID
 *  Note: Task id MUST be increasing order.
 */
/*****************************************************************************/

enum {
	/**
	  * SYSTEM TASKS
	  **************/
	TASK_TIMER_TICK_ID				,
	/**
	  * APP TASKS
	  **************/
	OTB_TASK_SHELL_ID				,
	OTB_TASK_LIFE_ID				,
	OTB_TASK_IF_ID					,
	OTB_TASK_RS485_ID				,
	/**
	  * EOT task ID
	  **************/
	AK_TASK_EOT_ID
};
/*****************************************************************************/
/*  DECLARE: Task entry point
 */
/*****************************************************************************/
extern void task_shell(ak_msg_t*);
extern void task_life(ak_msg_t*);
extern void task_if(ak_msg_t*);
extern void task_lcd(ak_msg_t*);
extern void task_rs485(ak_msg_t*);
extern void task_sensor(ak_msg_t*);
#endif //__TASK_LIST_H__
