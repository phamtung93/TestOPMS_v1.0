#ifndef __WGPIO_H__
#define __WGPIO_H__

#include <stdio.h>
#include <stdint.h>

typedef void (*pf_digital_mode) (uint8_t);
typedef void (*pf_digital_write_high)(void);
typedef void (*pf_digital_write_low)(void);

typedef uint8_t (*pf_digital_read)();

typedef struct {
	uint8_t pin_number;
	pf_digital_mode init;
	pf_digital_write_high set;
	pf_digital_write_low reset;
	pf_digital_read read;
	uint8_t mode_status;
} pin_table_t;

enum {
	RS485_DIR,
};

extern pin_table_t gpio_list[];
extern uint8_t GPIO_LIST_SIZE;

#endif // __WGPIO_H__
