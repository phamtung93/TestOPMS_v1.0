
#include "../ak/fsm.h"
#include "../ak/port.h"
#include "../ak/message.h"
#include "../ak/timer.h"

#include "../sys/sys_irq.h"
#include "../sys/sys_io.h"
#include "../sys/sys_ctrl.h"
#include "../sys/sys_dbg.h"
#include "../sys/sys_arduino.h"

#include "../common/cmd_line.h"
#include "../common/utils.h"
#include "../common/xprintf.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_rs485_modbus.h"

Modbus slave;

rspw_slave_registers_t register_list;

uint8_t  SLAVE_ID;
uint16_t au16data[8];
uint16_t time_use_ = 0;
uint32_t finish_time = 0;
uint16_t request_count = 0;
uint8_t  finish_rev_data = 0;
uint8_t  receive_timeout = 5;

void iqr_rs485_rev() {
	slave.getData_serial(io_uart_RS485_get_byte());

	if (finish_rev_data == 1) {
		finish_rev_data = 0;
		timer_set(OTB_TASK_RS485_ID, OTB_RS485_SLAVE_UPDATE, OTB_RS485_SLAVE_UPDATE_INTERVAL, TIMER_ONE_SHOT);
	}

	finish_time = millis();
}

void task_rs485(ak_msg_t* msg) {
	switch (msg->sig) {
	case OTB_RS485_INIT: {
		slave.init(SLAVE_ID, RS485_DIR, 9600);
		finish_rev_data = 1;
	}
		break;

	case OTB_RS485_SLAVE_UPDATE: {

		if (finish_rev_data == 0) {
			if (abs((int8_t)(millis()-finish_time)) > receive_timeout) {
				/*
					APP_DBG("OTB_RS485_SLAVE_UPDATE\n");
					APP_DBG("abs(millis()-finish_time: = %d - %d = %d\n", millis(), finish_time, abs((int8_t)(millis()-finish_time)));
				*/
				slave.poll(au16data, sizeof(au16data));
				finish_rev_data = 1;
			}
			else {
				finish_rev_data = 1;
			}
		}
	}
		break;

	case OTB_RS485_SLAVE_DEVICE_1_RESET_REQ: {
		APP_DBG("OTB_RS485_SLAVE_COIL_1_RESET_REQ\n");
		APP_PRINT("Device 1: ON\n");
		io_rs485_relay1_pin_reset();
	}
		break;

	case OTB_RS485_SLAVE_DEVICE_2_RESET_REQ: {
		APP_DBG("OTB_RS485_SLAVE_COIL_2_RESET_REQ\n");
		APP_PRINT("Device 2: ON\n");
		io_rs485_relay2_pin_reset();
	}
		break;

	case OTB_RS485_SLAVE_DEVICE_3_RESET_REQ: {
		APP_DBG("OTB_RS485_SLAVE_COIL_3_RESET_REQ\n");
		APP_PRINT("Device 3: ON\n");
		io_rs485_relay3_pin_reset();
	}
		break;

	case OTB_RS485_SLAVE_DEVICE_4_RESET_REQ: {
		APP_DBG("OTB_RS485_SLAVE_COIL_4_RESET_REQ\n");
		APP_PRINT("Device 4: ON\n");
		io_rs485_relay4_pin_reset();
	}
		break;

	case OTB_RS485_SLAVE_DEVICE_5_RESET_REQ: {
		APP_DBG("OTB_RS485_SLAVE_COIL_5_RESET_REQ\n");
		APP_PRINT("Device 5: ON\n");
		io_rs485_relay5_pin_reset();
	}
		break;

	case OTB_RS485_SLAVE_DEVICE_6_RESET_REQ: {
		APP_DBG("OTB_RS485_SLAVE_COIL_6_RESET_REQ\n");
		APP_PRINT("Device 6: ON\n");
		io_rs485_relay6_pin_reset();
	}
		break;

	case OTB_RS485_SLAVE_RESET_REQ: {
		APP_PRINT("OTB_RS485_SLAVE_RESET_SETTING_REQ\n");
	}
		break;

	default:
		break;
	}
}

void slave_modbus_reset() {
	sys_ctrl_reset();
}

void register_init(pf_modbus_func5 callback_func5, pf_modbus_func6 callback_func6) {
	register_list.func5.callback = callback_func5;
	register_list.func6.callback = callback_func6;
}

uint8_t	func1_read_coil_status(uint8_t pin, relay_t* coils) {
	uint8_t status = 0;

	switch (pin) {
	case COIL_1:
		status = coils->coil[pin];
		break;

	case COIL_2:
		status = coils->coil[pin];
		break;

	case COIL_3:
		status = coils->coil[pin];
		break;

	case COIL_4:
		status = coils->coil[pin];
		break;

	case COIL_5:
		status = coils->coil[pin];
		break;

	case COIL_6:
		status = coils->coil[pin];
		break;

	default:
		break;
	}

	return status;
}

void coils_update_status(rspw_slave_registers_t * coils_status_update, relay_t* coils_eeprom_status) {
	for(uint8_t i = COIL_1; i < OTB_TOTAL_COIL; i++) {
		coils_status_update->status[i] = coils_eeprom_status->coil[i];
		coils_startup_update(i, coils_status_update->status[i]);
	}
}

void func5_control_coil(uint16_t reg_list, uint16_t data_write) {

	if(data_write == 0xFF00) {
		switch(reg_list) {
		case COIL_1: {
			APP_DBG("Device 1: OFF\n");
			io_rs485_relay1_pin_set();
		}
			break;

		case COIL_2: {
			APP_PRINT("Device 2: OFF\n");
			io_rs485_relay2_pin_set();
		}
			break;

		case COIL_3: {
			APP_DBG("Device 3: OFF\n");
			io_rs485_relay3_pin_set();
		}
			break;

		case COIL_4: {
			APP_PRINT("Device 4: OFF\n");
			io_rs485_relay4_pin_set();
		}
			break;

		case COIL_5: {
			APP_PRINT("Device 5: OFF\n");
			io_rs485_relay5_pin_set();
		}
			break;

		case COIL_6: {
			APP_PRINT("Device 6: OFF\n");
			io_rs485_relay6_pin_set();
		}
			break;

		default:
			APP_PRINT("f\n");
			break;
		}
	}
	else if(data_write == 0x0000) {
		switch(reg_list) {
		case COIL_1: {
			APP_PRINT("Device 1: ON\n");
			io_rs485_relay1_pin_reset();
		}
			break;

		case COIL_2: {
			APP_PRINT("Device 2: ON\n");
			io_rs485_relay2_pin_reset();
		}
			break;

		case COIL_3: {
			APP_PRINT("Device 3: ON\n");
			io_rs485_relay3_pin_reset();
		}
			break;

		case COIL_4: {
			APP_PRINT("Device 4: ON\n");
			io_rs485_relay4_pin_reset();
		}
			break;

		case COIL_5: {
			APP_PRINT("Device 5: ON\n");
			io_rs485_relay5_pin_reset();
		}
			break;

		case COIL_6: {
			APP_PRINT("Device 6: ON\n");
			io_rs485_relay6_pin_reset();
		}
			break;

		default:
			APP_PRINT("f\n");
			break;
		}
	}
	else {
		APP_PRINT("RAIN: wrong data write", 0x06);
	}

	uint8_t status = (data_write > 0)?1:0;

	relay_t coils;
	memset((uint8_t *)&coils, 0, sizeof(relay_t));

	eeprom_read(EEPROM_RELAY_STATUS_ADDR, (uint8_t*)&coils, sizeof(relay_t));
	coils.coil[reg_list] = status;
	register_list.status[reg_list] = status;
	eeprom_write(EEPROM_RELAY_STATUS_ADDR, (uint8_t*)&coils, sizeof(relay_t));
}

void coils_startup_update(uint16_t register_list, uint16_t data_write) {

	if(data_write == 1) {
		switch(register_list) {
		case COIL_1: {
			io_rs485_relay1_pin_set();
		}
			break;

		case COIL_2: {
			io_rs485_relay2_pin_set();
		}
			break;

		case COIL_3: {
			io_rs485_relay3_pin_set();
		}
			break;

		case COIL_4: {
			io_rs485_relay4_pin_set();
		}
			break;

		case COIL_5: {
			io_rs485_relay5_pin_set();
		}
			break;

		case COIL_6: {
			io_rs485_relay6_pin_set();
		}
			break;

		default:
			APP_PRINT("f\n");
			break;
		}
	}
	else if(data_write == 0) {
		switch(register_list) {
		case COIL_1: {
			io_rs485_relay1_pin_reset();
		}
			break;

		case COIL_2: {
			io_rs485_relay2_pin_reset();
		}
			break;

		case COIL_3: {
			io_rs485_relay3_pin_reset();
		}
			break;

		case COIL_4: {
			io_rs485_relay4_pin_reset();
		}
			break;
		case COIL_5: {
			io_rs485_relay5_pin_reset();
		}
			break;

		case COIL_6: {
			io_rs485_relay6_pin_reset();
		}
			break;

		default:
			APP_PRINT("f\n");
			break;
		}
	}
	else {
		APP_PRINT("RAIN: wrong data write", 0x07);
	}
}

void func6_control_coil(uint16_t register_list, uint16_t data_write) {
	uint16_t reset_time = data_write;

	switch(register_list) {
	case COIL_1: {
		APP_DBG("Device 1:	");
		io_rs485_relay1_pin_set();
		timer_set(OTB_TASK_RS485_ID, OTB_RS485_SLAVE_DEVICE_1_RESET_REQ, reset_time, TIMER_ONE_SHOT);
	}
		break;

	case COIL_2: {
		APP_DBG("Device 2:	");
		io_rs485_relay2_pin_set();
		timer_set(OTB_TASK_RS485_ID, OTB_RS485_SLAVE_DEVICE_2_RESET_REQ, reset_time, TIMER_ONE_SHOT);
	}
		break;

	case COIL_3: {
		APP_DBG("Device 3:	");
		io_rs485_relay3_pin_set();
		timer_set(OTB_TASK_RS485_ID, OTB_RS485_SLAVE_DEVICE_3_RESET_REQ, reset_time, TIMER_ONE_SHOT);
	}
		break;

	case COIL_4: {
		APP_DBG("Device 4:	");
		io_rs485_relay4_pin_set();
		timer_set(OTB_TASK_RS485_ID, OTB_RS485_SLAVE_DEVICE_4_RESET_REQ, reset_time, TIMER_ONE_SHOT);
	}
		break;

	case COIL_5: {
		APP_DBG("Device 5:	");
		io_rs485_relay5_pin_set();
		timer_set(OTB_TASK_RS485_ID, OTB_RS485_SLAVE_DEVICE_5_RESET_REQ, reset_time, TIMER_ONE_SHOT);
	}
		break;

	case COIL_6: {
		APP_DBG("Device 6:	");
		io_rs485_relay6_pin_set();
		timer_set(OTB_TASK_RS485_ID, OTB_RS485_SLAVE_DEVICE_6_RESET_REQ, reset_time, TIMER_ONE_SHOT);
	}
		break;

	case SLAVE_RESET: {
		timer_set(OTB_TASK_RS485_ID, OTB_RS485_SLAVE_RESET_REQ, 500, TIMER_ONE_SHOT);
	}
		break;

	default:
		APP_PRINT("f\n");
		break;
	}
}
