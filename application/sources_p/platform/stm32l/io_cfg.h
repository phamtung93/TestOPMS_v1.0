/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   05/09/2016
 * @Update:
 * @AnhHH: Add io function for sth11 sensor.
 ******************************************************************************
**/
#ifndef __IO_CFG_H__
#define __IO_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"
#include "system_stm32l1xx.h"
#include "core_cm3.h"
#include "core_cmFunc.h"

#include "app_data.h"

#include "../driver/rtc/rtc.h"
#include "../driver/eeprom/eeprom.h"

#define PIN_INPUT 0x0
#define PIN_OUTPUT 0x1

/*
 * define pin for arduino pinMode/digitalWrite/digitalRead
 * NOTE: define value MUST be deferrent
 */

#define SHT1X_CLK_PIN					(0x01)
#define SHT1X_DATA_PIN					(0x02)

#define SSD1306_CLK_PIN					(0x03)
#define SSD1306_DATA_PIN				(0x04)

#define DS1302_CLK_PIN					(0x05)
#define DS1302_DATA_PIN					(0x06)
#define DS1302_CE_PIN					(0x07)

/*****************************************************************************
 *Pin map led life
******************************************************************************/
#define LED_LIFE_IO_PIN					(GPIO_Pin_14)
#define LED_LIFE_IO_PORT				(GPIOB)
#define LED_LIFE_IO_CLOCK				(RCC_AHBPeriph_GPIOB)
/****************************************************************************
 * RS485 defines
*****************************************************************************/
#define USART_RS485							(USART1)
#define USART_RS485_CLK						(RCC_APB2Periph_USART1)
#define USART_RS485_IRQn					(USART1_IRQn)

#define USART_RS485_TX_PIN					(GPIO_Pin_6)
#define USART_RS485_TX_GPIO_PORT			(GPIOB)
#define USART_RS485_TX_GPIO_CLK				(RCC_AHBPeriph_GPIOB)
#define USART_RS485_TX_SOURCE				(GPIO_PinSource6)
#define USART_RS485_TX_AF					(GPIO_AF_USART1)

#define USART_RS485_RX_PIN					(GPIO_Pin_7)
#define USART_RS485_RX_GPIO_PORT			(GPIOB)
#define USART_RS485_RX_GPIO_CLK				(RCC_AHBPeriph_GPIOB)
#define USART_RS485_RX_SOURCE				(GPIO_PinSource7)
#define USART_RS485_RX_AF					(GPIO_AF_USART1)

/*RS485 dir IO*/
#define RS485_DIR_IO_PIN					(GPIO_Pin_8)
#define RS485_DIR_IO_PORT					(GPIOB)
#define RS485_DIR_IO_CLOCK					(RCC_AHBPeriph_GPIOB)

/*****************************************************************************
 *Pin map relay
******************************************************************************/
#define RELAY_1_IO_PIN					(GPIO_Pin_1)
#define RELAY_1_IO_PORT					(GPIOB)
#define RELAY_1_IO_CLOCK				(RCC_AHBPeriph_GPIOB)

#define RELAY_2_IO_PIN					(GPIO_Pin_0)
#define RELAY_2_IO_PORT					(GPIOB)
#define RELAY_2_IO_CLOCK				(RCC_AHBPeriph_GPIOB)

#define RELAY_3_IO_PIN					(GPIO_Pin_7)
#define RELAY_3_IO_PORT					(GPIOA)
#define RELAY_3_IO_CLOCK				(RCC_AHBPeriph_GPIOA)

#define RELAY_4_IO_PIN					(GPIO_Pin_6)
#define RELAY_4_IO_PORT					(GPIOA)
#define RELAY_4_IO_CLOCK				(RCC_AHBPeriph_GPIOA)

#define RELAY_5_IO_PIN					(GPIO_Pin_5)
#define RELAY_5_IO_PORT					(GPIOA)
#define RELAY_5_IO_CLOCK				(RCC_AHBPeriph_GPIOA)

#define RELAY_6_IO_PIN					(GPIO_Pin_4)
#define RELAY_6_IO_PORT					(GPIOA)
#define RELAY_6_IO_CLOCK				(RCC_AHBPeriph_GPIOA)

enum {
	COIL_1,
	COIL_2,
	COIL_3,
	COIL_4,
	COIL_5,
	COIL_6,
	COIL_7,
	COIL_8,
	COIL_9,
	COIL_10,
	COIL_11,
	COIL_12,
	SLAVE_RESET = 0x4321
};

/*****************************************************************************
 * CLI Periphral define - UART3
******************************************************************************/
#define USART_CLI							(USART3)
#define USART_CLI_CLK						(RCC_APB1Periph_USART3)
#define USART_CLI_IRQn						(USART3_IRQn)

#define USART_CLI_TX_PIN					(GPIO_Pin_10)
#define USART_CLI_TX_GPIO_PORT				(GPIOB)
#define USART_CLI_TX_GPIO_CLK				(RCC_AHBPeriph_GPIOB)
#define USART_CLI_TX_SOURCE					(GPIO_PinSource10)
#define USART_CLI_TX_AF						(GPIO_AF_USART3)

#define USART_CLI_RX_PIN					(GPIO_Pin_11)
#define USART_CLI_RX_GPIO_PORT				(GPIOB)
#define USART_CLI_RX_GPIO_CLK				(RCC_AHBPeriph_GPIOB)
#define USART_CLI_RX_SOURCE					(GPIO_PinSource11)
#define USART_CLI_RX_AF						(GPIO_AF_USART3)

/*****************************************************************************
 *io uart for RS485
******************************************************************************/
extern void io_RS485_dir_pin_input();
extern void io_RS485_dir_pin_output();
extern void io_RS485_dir_pin_set();
extern void io_RS485_dir_pin_reset();

extern void modbus_coil_init();
extern void digitalwrite_high_rs485_dir(void);
extern void digitalwrite_low_rs485_dir(void);
extern void modbus_pin_select_address_init();
extern void pinmode_rs485_dir(uint8_t mode);
extern void io_uart_rs485_transfer(uint8_t byte);
extern void io_uart_rs485_cfg(uint32_t baudrate );

extern uint8_t digitalread_rs485_dir();
extern uint8_t io_uart_RS485_get_byte();
extern uint8_t modbus_read_pin_select_status();

/******************************************************************************
* button function
*******************************************************************************/
extern void io_button_mode_init();
extern void io_button_up_init();
extern void io_button_down_init();

extern uint8_t io_button_mode_read();
extern uint8_t io_button_up_read();
extern uint8_t io_button_down_read();

/******************************************************************************
* led status function
*******************************************************************************/
extern void led_life_init();
extern void led_life_on();
extern void led_life_off();

extern void led_adc_init();
extern void led_adc_on();
extern void led_adc_off();

/*****************************************************************************
 *io relay for RS485
******************************************************************************/
extern void io_rs485_relay_init_output();
extern void io_rs485_relay12_pin_set();
extern void io_rs485_relay12_pin_reset();
extern void io_rs485_relay11_pin_set();
extern void io_rs485_relay11_pin_reset();
extern void io_rs485_relay10_pin_set();
extern void io_rs485_relay10_pin_reset();
extern void io_rs485_relay9_pin_set();
extern void io_rs485_relay9_pin_reset();
extern void io_rs485_relay8_pin_set();
extern void io_rs485_relay8_pin_reset();
extern void io_rs485_relay7_pin_set();
extern void io_rs485_relay7_pin_reset();
extern void io_rs485_relay6_pin_set();
extern void io_rs485_relay6_pin_reset();
extern void io_rs485_relay5_pin_set();
extern void io_rs485_relay5_pin_reset();
extern void io_rs485_relay4_pin_set();
extern void io_rs485_relay4_pin_reset();
extern void io_rs485_relay3_pin_set();
extern void io_rs485_relay3_pin_reset();
extern void io_rs485_relay2_pin_set();
extern void io_rs485_relay2_pin_reset();
extern void io_rs485_relay1_pin_set();
extern void io_rs485_relay1_pin_reset();
/******************************************************************************
* adc function
* + themistor sensor
*
* Note: MUST be enable internal clock for adc module.
*******************************************************************************/
/* configure adc peripheral */
extern void io_cfg_adc1(void);

/* adc configure for CT sensor */
extern void adc_ct_io_cfg();
extern uint16_t adc_ct_io_read(uint8_t);

/* adc configure for thermistor sensor */
extern void adc_thermistor_io_cfg();
extern uint16_t adc_thermistor_io_read(uint8_t);

/******************************************************************************
* eeprom function
*******************************************************************************/
#define EEPROM_BASE_ADDRESS         (0x08080000)
#define EEPROM_MAX_SIZE             (0x1000)

extern uint8_t  io_eeprom_read(uint32_t, uint8_t*, uint32_t);
extern uint8_t  io_eeprom_write(uint32_t, uint8_t*, uint32_t);
extern uint8_t  io_eeprom_erase(uint32_t, uint32_t);


/******************************************************************************
* CLI function define - UART 3
*******************************************************************************/

void io_uart_3_init(uint16_t baudrate) ;
void io_uart_3_send_chr(uint8_t chr) ;
uint8_t io_uart_3_get_chr() ;

#ifdef __cplusplus
}
#endif

#endif //__IO_CFG_H__
