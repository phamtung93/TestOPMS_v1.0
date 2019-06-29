/**
 ******************************************************************************
 * @Author: ThanNT
 * @Date:   13/08/2016
 ******************************************************************************
**/

#include <stdint.h>
#include <stdlib.h>

#include "../ak/ak.h"
#include "../ak/task.h"
#include "../ak/timer.h"
#include "../ak/message.h"

#include "../common/cmd_line.h"
#include "../common/utils.h"
#include "../common/xprintf.h"
#include "../common/ring_buffer.h"

#include "../sys/sys_ctrl.h"
#include "../sys/sys_io.h"
#include "../sys/sys_dbg.h"

#include "app.h"
#include "app_if.h"
#include "app_dbg.h"
#include "app_data.h"
#include "app_flash.h"
#include "app_eeprom.h"

#include "task_shell.h"
#include "task_list.h"
#include "task_list_if.h"
#include "task_if.h"
#include "task_life.h"
#include "task_rs485_modbus.h"

#include "../driver/led/led.h"
#include "../driver/eeprom/eeprom.h"

/*****************************************************************************/
/*  command function declare
 */
/*****************************************************************************/
static int32_t shell_reset(uint8_t* argv);
static int32_t shell_ver(uint8_t* argv);
static int32_t shell_help(uint8_t* argv);
static int32_t shell_reboot(uint8_t* argv);
static int32_t shell_fatal(uint8_t* argv);
static int32_t shell_pwrs(uint8_t* argv);
static int32_t shell_set(uint8_t* argv);
static int32_t shell_reset_device(uint8_t* argv);
static int32_t shell_flash(uint8_t* argv);
/*****************************************************************************/
/*  command table
 */
/*****************************************************************************/
cmd_line_t lgn_cmd_table[] = {

	/*************************************************************************/
	/* system command */
	/*************************************************************************/
	{(const int8_t*)"reset"		,	shell_reset,		(const int8_t*)"reset terminal"},
	{(const int8_t*)"ver"		,	shell_ver,			(const int8_t*)"version info"},
	{(const int8_t*)"help"		,	shell_help,			(const int8_t*)"help command info"},
	{(const int8_t*)"reboot"	,	shell_reboot,		(const int8_t*)"reboot system"},
	{(const int8_t*)"fatal"		,	shell_fatal,		(const int8_t*)"fatal system info"},
	{(const int8_t*)"pwrs"		,	shell_pwrs,			(const int8_t*)"reset power device info"},
	{(const int8_t*)"set"		,	shell_set,			(const int8_t*)"set device"},
	{(const int8_t*)"res"		,	shell_reset_device,	(const int8_t*)"reset device"},
	{(const int8_t*)"flash"		,	shell_flash,		(const int8_t*)"clean flash device stores"},

	/*************************************************************************/
	/* debug command */
	/*************************************************************************/

	/* End Of Table */
	{(const int8_t*)0,(pf_cmd_func)0,(const int8_t*)0}
};

int32_t shell_flash(uint8_t* argv) {
	switch (*(argv + 6)) {
	case 'r': {
		if (eeprom_erase(EEPROM_RELAY_STATUS_ADDR, sizeof(relay_t)) == EEPROM_DRIVER_OK) {
			APP_DBG("eeprom erase ok\n");
		}
		else {
			APP_DBG("eeprom erase error\n");
		}
	}
		break;

	default:
		break;
	}
	return 0;
}

int32_t shell_set(uint8_t* argv) {
	switch (*(argv + 4)) {
	case '1': {
		APP_DBG("set device 1\n");
		io_rs485_relay1_pin_set();
	}
		break;

	case '2': {
		APP_DBG("set device 2\n");
		io_rs485_relay2_pin_set();
	}
		break;

	case '3': {
		APP_DBG("set device 3\n");
		io_rs485_relay3_pin_set();
	}
		break;

	case '4': {
		APP_DBG("set device 4\n");
		io_rs485_relay4_pin_set();
	}
		break;

	case '5': {
		APP_DBG("set device 5\n");
		io_rs485_relay5_pin_set();
	}
		break;

	case '6': {
		APP_DBG("set device 6\n");
		io_rs485_relay6_pin_set();
	}
		break;

	default:

		break;
	}
	return 0;

}

int32_t shell_reset_device(uint8_t* argv) {
	switch (*(argv + 4)) {
	case '1': {
		APP_DBG("set device 1\n");
		io_rs485_relay1_pin_reset();
	}
		break;

	case '2': {
		APP_DBG("set device 2\n");
		io_rs485_relay2_pin_reset();
	}
		break;

	case '3': {
		APP_DBG("set device 3\n");
		io_rs485_relay3_pin_reset();
	}
		break;

	case '4': {
		APP_DBG("set device 4\n");
		io_rs485_relay4_pin_reset();
	}
		break;

	case '5': {
		APP_DBG("set device 5\n");
		io_rs485_relay5_pin_reset();
	}
		break;

	case '6': {
		APP_DBG("set device 6\n");
		io_rs485_relay6_pin_reset();
	}
		break;

	default:

		break;
	}
	return 0;
}

/*****************************************************************************/
/*  command function definaion
 */
/*****************************************************************************/
int32_t shell_reset(uint8_t* argv) {
	(void)argv;
	xprintf("\033[2J\r");
	return 0;
}

int32_t shell_ver(uint8_t* argv) {
	(void)argv;
	firmware_header_t firmware_header;
	sys_ctrl_get_firmware_info(&firmware_header);

	LOGIN_PRINT("kernel version: %s\n", AK_VERSION);
	LOGIN_PRINT("firmware checksum: %04x\n", firmware_header.checksum);
	LOGIN_PRINT("firmware length: %d\n", firmware_header.bin_len);
	return 0;
}

int32_t shell_help(uint8_t* argv) {
	uint32_t idx = 0;
	switch (*(argv + 4)) {
	default:
		LOGIN_PRINT("\nCOMMANDS INFORMATION:\n\n");
		while(lgn_cmd_table[idx].cmd != (const int8_t*)0) {
			LOGIN_PRINT("%s\n\t%s\n\n", lgn_cmd_table[idx].cmd, lgn_cmd_table[idx].info);
			idx++;
		}
		break;
	}
	return 0;
}

int32_t shell_reboot(uint8_t* argv) {
	(void)argv;
	sys_ctrl_reset();
	return 0;
}

int32_t shell_fatal(uint8_t* argv) {
	fatal_log_t login_fatal_log;

	switch (*(argv + 6)) {
	case 't': {
		FATAL("test",0x007);
	}
		break;

	case '!':
		while(1);
		break;

	case '@':
		DISABLE_INTERRUPTS();
		while(1);
		break;

	case 'r':
		mem_set((uint8_t*)&login_fatal_log, 0, sizeof(fatal_log_t));
		eeprom_write(EEPROM_FATAL_LOG_ADDR, (uint8_t*)&login_fatal_log, sizeof(fatal_log_t));

		LOGIN_PRINT("reset fatal log OK\n");
		break;

	case 'l': {
		eeprom_read(EEPROM_FATAL_LOG_ADDR, (uint8_t*)&login_fatal_log, sizeof(fatal_log_t));

		LOGIN_PRINT("fatal times:\t%d\n", login_fatal_log.fatal_times);
		LOGIN_PRINT("restart times:\t%d\n", login_fatal_log.restart_times);

		LOGIN_PRINT("fatal type:\t%s\n", login_fatal_log.string);
		LOGIN_PRINT("fatal code:\t%d\n", login_fatal_log.code);

		LOGIN_PRINT("task id:\t%d\n", login_fatal_log.current_task.id);
		LOGIN_PRINT("task pri:\t%d\n", login_fatal_log.current_task.pri);
		LOGIN_PRINT("task entry:\t0x%x\n", login_fatal_log.current_task.task);

		LOGIN_PRINT("obj sig:\t%d\n", login_fatal_log.current_active_object.sig);
		LOGIN_PRINT("obj type:\t0x%x\n", get_msg_type(&login_fatal_log.current_active_object));
		LOGIN_PRINT("obj ref count:\t%d\n", get_msg_ref_count(&login_fatal_log.current_active_object));
		LOGIN_PRINT("obj wait time:\t%d\n", login_fatal_log.current_active_object.dbg_handler.start_exe - login_fatal_log.current_active_object.dbg_handler.start_post);
	}
		break;

	default:
		break;
	}

	return 0;
}

int32_t shell_pwrs(uint8_t* argv) {
	(void)argv;

	LOGIN_PRINT("\n\nMODBUS DEVICE NAME:\n");
	LOGIN_PRINT("                          power reset\n\n");

	LOGIN_PRINT("MODBUS SLAVE ID:\n");
	LOGIN_PRINT("                          11\n\n");
	LOGIN_PRINT("MODBUS FUNCTION SUPPORT:\n");
	LOGIN_PRINT("                          read coil status\n\n");
	LOGIN_PRINT("                          write single coil\n\n");
	LOGIN_PRINT("                          write register\n\n");
	return 0;
}
