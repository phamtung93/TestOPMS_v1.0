#ifndef __TASK_TESTTING__
#define __TASK_TESTTING__
#include "stm32l1xx.h"

// include driver
#include "button.h"

#include "io_cfg.h"
#include "xprintf.h"


extern button_t button_all_test;
extern button_t button_rs485;
extern button_t button_cli;

extern void button_press_cb(button_t* button);

// Rectang xprintf for CLI function
//#define CLI_PRINT(fmt, ...)       \
//	{ void (*xprintf_stream_io_out_p)(uint8_t); \
//	xprintf_stream_io_out_p	= xprintf_stream_io_out; \
//	xprintf_stream_io_out = io_uart3_cli_put; \
//	xprintf( fmt, ##__VA_ARGS__); \
//	xprintf_stream_io_out = xprintf_stream_io_out_p; }
#define CLI_PRINT(fmt)		uart3_cli_send_string(fmt)


// Rectang xprintf for RS485 function
#define RS485_PRINT(fmt, ...)       \
	{ void (*xprintf_stream_io_out_p)(uint8_t); \
	xprintf_stream_io_out_p	= xprintf_stream_io_out; \
	xprintf_stream_io_out = io_rs485_uart_send; \
	xprintf( fmt, ##__VA_ARGS__); \
	xprintf_stream_io_out = xprintf_stream_io_out_p; }

#endif //__TASK_TESTTING__
