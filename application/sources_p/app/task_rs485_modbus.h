#ifndef __TASK_RS485_H__
#define __TASK_RS485_H__
#include <stdint.h>

#include "../driver/modbus_rtu/modbus_rtu.h"

#define HARDWARE_VER	1
#define SOFTWARE_VER	2
#define DEVICE_TYPE		1

typedef void (*pf_modbus_func5)(uint16_t, uint16_t);
typedef void (*pf_modbus_func6)(uint16_t, uint16_t);

typedef void (*pf_gpio_set)();
typedef void (*pf_gpio_reset)();

typedef struct {
	pf_modbus_func5 callback;
} func_5_register_t;

typedef struct {
	pf_modbus_func6 callback;
} func_6_register_t;

typedef struct {
	func_5_register_t func5;
	func_6_register_t func6;
	uint16_t status[OTB_TOTAL_COIL];
} rspw_slave_registers_t;

typedef struct {
	uint16_t addr;
	uint16_t status;
}data_reset_coil_t;

extern uint8_t SLAVE_ID;

extern rspw_slave_registers_t register_list;
extern void slave_modbus_reset();
extern void func6_control_coil(uint16_t register_list, uint16_t data_write);
extern void register_init(pf_modbus_func5 callback_func5, pf_modbus_func6 callback_func6);
extern void func5_control_coil(uint16_t register_list, uint16_t data_write);
extern uint8_t func1_read_coil_status(uint8_t pin, relay_t * coils);
extern void coils_update_status(rspw_slave_registers_t * coils_status_update, relay_t* coils_eeprom_status);
extern void coils_startup_update(uint16_t register_list, uint16_t data_write);

#endif // __TASK_RS485_H__
