#ifndef __GENERAL_OUTPUT_H__
#define __GENERAL_OUTPUT_H__


#include <stdint.h>

#include "../sys/sys_ctrl.h"
#include "../sys/sys_io.h"
#include "../sys/sys_dbg.h"

typedef void (*pf_func)(void);

extern void general_ouput_init();
extern void set_general_output_status(uint8_t pin, uint8_t status);
extern uint8_t get_general_output_status(uint8_t pin);

#endif // __GENERAL_OUTPUT_H__
