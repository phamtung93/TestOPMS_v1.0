/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   13/08/2016
 ******************************************************************************
**/

/* kernel include */
#include "ak.h"
#include "message.h"
#include "timer.h"
#include "fsm.h"

/* driver include */
#include "led.h"
#include "button.h"
#include "flash.h"
#include "hs1101.h"

/* app include */
#include "app.h"
#include "app_dbg.h"
#include "app_bsp.h"
#include "app_flash.h"
#include "app_non_clear_ram.h"

#include "task_list.h"
#include "task_shell.h"
#include "task_life.h"
#include "task_if.h"
#include "task_rf24_if.h"
#include "task_uart_if.h"
#include "task_display.h"
#include "task_testting.h"

/* sys include */
#include "sys_boot.h"
#include "sys_irq.h"
#include "sys_io.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "io_cfg.h"

/* arduino include */
#include "SPI.h"
#include "WString.h"

/* common include */
#include "screen_manager.h"

/* net link include */
#include "link.h"
#include "link_sig.h"

const app_info_t app_info { \
	.magic_number = APP_MAGIC_NUMBER, \
			.version = APP_VER, \
};

static boot_app_share_data_t boot_app_share_data;

static void app_start_timer();
static void app_init_state_machine();
static void app_task_init();

/*****************************************************************************/
/* app main function.
 */
/*****************************************************************************/
int main_app() {
	APP_PRINT("app version: %d.%d.%d.%d\n", app_info.version[0] \
			, app_info.version[1] \
			, app_info.version[2] \
			, app_info.version[3]);

	sys_soft_reboot_counter++;

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
	/*********************
	* hardware configure *
	**********************/
	/* init watch dog timer */
	sys_ctrl_independent_watchdog_init();	/* 32s */
	sys_ctrl_soft_watchdog_init(200);		/* 20s */

	/*********************
	* software configure *
	**********************/
	/* initial boot object */
	sys_boot_init();

	/* life led init */
	led_init(&led_life, led_life_init, led_life_on, led_life_off);

	/* button init */
	button_init (&button_cli,		1, 1, io_button_init , io_button_3_read, (pf_button_callback)button_press_cb );
	button_init (&button_rs485,		1, 2, io_button_init , io_button_2_read, (pf_button_callback)button_press_cb );
	button_init (&button_all_test,	1, 3, io_button_init , io_button_1_read, (pf_button_callback)button_press_cb );

	button_enable (&button_cli);
	button_enable (&button_rs485);
	button_enable (&button_all_test);

	// timer for button polling
	timer_50us_init();
	timer_50us_enable ();

	EXIT_CRITICAL();

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

	sys_ctrl_shell_sw_to_nonblock();

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
	timer_set(AC_TASK_LIFE_ID, AC_LIFE_SYSTEM_CHECK, AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL, TIMER_PERIODIC);
	//timer_set(AC_TASK_TESTTING_ID, TESTTING_GPO_CHECK_REQ, 1000, TIMER_PERIODIC);
	;
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
	task_post_pure_msg(AC_TASK_TESTTING_ID, TESTTING_INIT);
}

/*****************************************************************************/
/* app common function
 */
/*****************************************************************************/

/* hardware timer interrupt 10ms
 * used for led, button polling
 */
void sys_irq_timer_10ms() {
		//button_timer_polling(&btn_mode);
		//button_timer_polling(&btn_up);
		//button_timer_polling(&btn_down);
}

/* hardware timer interrupt 50ms
 * used for encode and decode ir
 */
void sys_irq_timer_50us() {
	button_timer_polling (&button_all_test);
	button_timer_polling (&button_rs485);
	button_timer_polling (&button_cli);
}

/* hardware rtc interrupt alarm
 * used for internal rtc alarm
 */
void sys_irq_timer_hs1101() {
}

/* hardware io interrupt at rev ir pin
 * used for decode ir
 */
void sys_irq_ir_io_rev() {
}

void sys_irq_usb_recv(uint8_t* data, uint32_t len) {
	(void)data;
	(void)len;
}

void* app_get_boot_share_data() {
	return (void*)&boot_app_share_data;
}


void assert_failed(uint8_t* file, uint32_t line) {
	xprintf("Assert fail at File %s Line %d", file, (int)line);
}

