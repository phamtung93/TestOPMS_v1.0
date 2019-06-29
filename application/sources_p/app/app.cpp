/**
 ******************************************************************************
 * @Author: ThanNT
 * @Date:   13/08/2016
 ******************************************************************************
**/
#include "stm32l.h"

/* kernel include */
#include "../ak/ak.h"
#include "../ak/message.h"
#include "../ak/timer.h"
#include "../ak/fsm.h"

/* sys include */
#include "../sys/sys_irq.h"
#include "../sys/sys_io.h"
#include "../sys/sys_ctrl.h"
#include "../sys/sys_dbg.h"
#include "../sys/sys_arduino.h"

/* app include */
#include "app.h"
#include "app_dbg.h"
#include "app_flash.h"

/* driver include */
#include "../driver/led/led.h"
#include "../driver/modbus_rtu/modbus_rtu.h"

/* task include */
#include "task_list.h"
#include "task_shell.h"
#include "task_life.h"
#include "task_if.h"
#include "task_rs485_modbus.h"

/* common include */
#include "../common/utils.h"

static void app_task_init();
static void app_start_timer();
static void app_init_state_machine();

/*****************************************************************************/
/* app main function.
 */
/*****************************************************************************/
int main_app() {
	APP_PRINT("main_app() entry OK\n");

	/******************************************************************************
	* init active kernel
	*******************************************************************************/
	ENTRY_CRITICAL();
	task_init();
	task_create(app_task_table);
	EXIT_CRITICAL();

	/******************************************************************************
	* init applications
	*******************************************************************************/

	/* init watch dog timer */
	sys_ctrl_independent_watchdog_init();	/* 32s */
	sys_ctrl_soft_watchdog_init(200);		/* 20s */

	/*********************
	* software configure *
	**********************/
	/* life led init */
	led_init(&led_life, led_life_init, led_life_on, led_life_off);

	/* start timer for application */
	app_init_state_machine();
	app_start_timer();

	/******************************************************************************
	* app task initial
	*******************************************************************************/
	app_task_init();

	/******************************************************************************
	* run applications
	*******************************************************************************/
	return task_run();
}

/*****************************************************************************/
/* app initial function.
 */
/*****************************************************************************/

/* start software timer for application
 * used for app tasks
 */
void app_start_timer() {
	/* start timer to toggle life led */
	timer_set(OTB_TASK_LIFE_ID, OTB_LIFE_SYSTEM_CHECK, OTB_LIFE_TASK_TIMER_LED_LIFE_INTERVAL, TIMER_PERIODIC);
}

/* init state machine for tasks
 * used for app tasks
 */
void app_init_state_machine() {
}

/* send first message to trigger start tasks
 * used for app tasks
 */
void app_task_init() {
}

/* hardware timer interrupt 10ms
 * used for sensor polling
 */
void sys_irq_timer_10ms() {
}
