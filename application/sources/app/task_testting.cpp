#include "fsm.h"
#include "port.h"
#include "message.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"
#include "sys_irq.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_testting.h"

#define RECEIVE_FLAG_UNSET				0
#define RECEIVE_FLAG_SET				1
#define RESULT_FAILED					0
#define RESULT_OK						1

button_t button_cli;
button_t button_rs485;
button_t button_all_test;

typedef enum {
OFF = 0,
ON
}gpo_t;
typedef struct {
	char*   cli_str;
	uint8_t cli_receive_flag;
	uint8_t cli_result;

	char*   rs485_str;
	uint8_t rs485_receive_flag;
	uint8_t rs485_result;

	uint8_t gpi_result[8];

	uint8_t fan_result[4];

	gpo_t	gpo_status;

} testting_result_t;

testting_result_t testting_result;

void testting_result_init(testting_result_t* testting_result) {
	testting_result->cli_str	= (char*)"AT CLI_TEST";
	testting_result->rs485_str	= (char*)"AT RS485_TEST";

	testting_result->cli_receive_flag	= 0;
	testting_result->rs485_receive_flag = 0;

	testting_result->gpo_status = OFF;

	testting_result->cli_result =   RESULT_FAILED;
	testting_result->rs485_result = RESULT_FAILED;

	for (uint8_t var = 0; var < 8; ++var) {
		testting_result->gpi_result[var] = RESULT_FAILED;
	}

	for (uint8_t var = 0; var < 4; ++var) {
		testting_result->fan_result[var] = RESULT_FAILED;
	}

	// init all funtions
	io_uart3_cli_init(115200);
	io_cli_led_init ();

	io_rs485_uart_init ();

	io_rs485_dir_pin_init();
	io_rs485_led_init ();

	io_rs485_led_on();

	io_input_control_init ();
	io_input_leds_init();

	io_fan_fn_leds_init ();

	io_temp_leds_init ();

	// off all pin leds
	io_cli_led_on ();
	io_rs485_led_on ();

	for (uint8_t var = 0; var < 8; ++var) {
		io_input_leds_on(var);
	}

	io_temp_1_led_on ();
	io_temp_2_led_on ();
	io_temp_3_led_on ();
	io_temp_4_led_on ();

	for (uint8_t var = 0; var < 4; ++var) {
		io_fan_leds_on(var);
	}
}

void task_testting(ak_msg_t* msg) {
	switch (msg->sig) {

	case TESTTING_INIT:
			APP_PRINT ("TESTTING_INIT\n");
			testting_result_init(&testting_result);
		break;

	case TESTTING_CLI_CHECK_REQ:

			APP_PRINT ("TESTTING_CLI_CHECK_REQ\n");
			io_cli_enable();
			testting_result.cli_receive_flag = ENABLE;
			sys_ctrl_delay_ms (10);
			CLI_PRINT ("whoare");
			io_uart3_cli_put ('\n');
			io_uart3_cli_put ('\r');

		break;

	case TESTTING_CLI_RECV:
			if (testting_result.cli_receive_flag) {
				io_cli_disable();

				uint8_t* data = get_data_common_msg(msg);

				APP_PRINT ("TESTTING_CLI_RECV : %s", data);
				testting_result.cli_receive_flag = DISABLE;

			}

		break;

	case TESTTING_RS485_CHECK_REQ:
		APP_PRINT ("TESTTING_RS485_CHECK_REQ\n");

		// send AT string to RS485 PORT
		// set waiting rs485_receive_flag
		break;

	case TESTTING_RS485_RECV:
		APP_PRINT ("TESTTING_RS485_RECV\n");

		break;

	case TESTTING_GPO_CHECK_REQ:
		APP_PRINT ("TESTTING_GPO_CHECK_REQ\n");

		if(testting_result.gpo_status == ON) {
			CLI_PRINT ("general output set all off\n\r");
			testting_result.gpo_status = OFF;
		} else {
			CLI_PRINT ("general output set all on\n\r");
			testting_result.gpo_status = ON;
		}

		// send AT string to RS485 PORT
		// set waiting rs485_receive_flag
		break;

	default:
		break;
	}
}

void button_press_cb(button_t* button) {
	switch (button->id) {
	case 1 :
		APP_PRINT ("");
		if (button->state == BUTTON_SW_STATE_PRESSED) {
			APP_PRINT ("button_cli\n");
			task_post_pure_msg(AC_TASK_TESTTING_ID, TESTTING_CLI_CHECK_REQ);
		}
		break;

	case 2:
		APP_PRINT ("button_rs485\n");
		if (button->state == BUTTON_SW_STATE_PRESSED) {
			APP_PRINT ("BUTTON_SW_STATE_PRESSED %d\n", 2);
			task_post_pure_msg(AC_TASK_TESTTING_ID, TESTTING_RS485_CHECK_REQ);
		}
		break;

	case 3:
		APP_PRINT ("button_all_test\n");
		if (button->state == BUTTON_SW_STATE_PRESSED) {
			APP_PRINT ("BUTTON_SW_STATE_PRESSED %d\n", 3);
			task_post_pure_msg(AC_TASK_TESTTING_ID, TESTTING_GPI_CHECK_REQ);
		}
		break;
	}
}

#define CLI_MAX_LENGTH			32
uint8_t cli_receiv_data[CLI_MAX_LENGTH];
uint8_t cli_receiv_data_len = 0;

// USART3 cli get char
void cli_irq_add(uint8_t byte) {
	if (byte == '\n') {
		if (cli_receiv_data_len > 0 ) {
			cli_receiv_data[cli_receiv_data_len] = 0;
			// send data to task testing
			//xprintf ("%s",cli_receiv_data);
			task_post_common_msg (AC_TASK_TESTTING_ID, TESTTING_CLI_RECV,(uint8_t*) cli_receiv_data, cli_receiv_data_len);

			// reset cli_receiv_data_len
			cli_receiv_data_len = 0;
		}

	} else {
		if (cli_receiv_data_len < CLI_MAX_LENGTH - 2) {
			cli_receiv_data[cli_receiv_data_len++] = byte;
		}

	}

	return;
}

#define RS485_MAX_LENGTH			32
uint8_t rs485_receiv_data[RS485_MAX_LENGTH];
uint8_t rs485_receiv_data_len = 0;
// USART2 rs485 get byte
void rs485_irq_add(uint8_t byte) {


	if (byte == '\n' || byte == '\r') {
		if (rs485_receiv_data_len > 0 ) {

			rs485_receiv_data[rs485_receiv_data_len] = 0;

			// send data to task testing

			xprintf ("[rs485_receiv]: %s\n", rs485_receiv_data);

			task_post_common_msg (AC_TASK_TESTTING_ID, TESTTING_RS485_RECV, rs485_receiv_data, rs485_receiv_data_len);

			// reset rs485_receiv_data_len
			rs485_receiv_data_len = 0;
		}

	} else {
		if (rs485_receiv_data_len < RS485_MAX_LENGTH) {
			rs485_receiv_data[rs485_receiv_data_len++] = byte;
		}

	}

	return;
}
