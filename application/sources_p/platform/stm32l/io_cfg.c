/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   05/09/2016
 * @Update:
 * @AnhHH: Add io function for sth11 sensor.
 ******************************************************************************
**/
#include <stdint.h>
#include <stdbool.h>

#include "ak.h"
#include "timer.h"

#include "io_cfg.h"

#include "../sys/sys_dbg.h"
#include "../sys/sys_ctrl.h"

#include "../app/app_dbg.h"

#include "../common/utils.h"

/******************************************************************************
* led status function
*******************************************************************************/
void led_life_init() {
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(LED_LIFE_IO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_LIFE_IO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_LIFE_IO_PORT, &GPIO_InitStructure);
}

void led_life_on() {
	GPIO_SetBits(LED_LIFE_IO_PORT, LED_LIFE_IO_PIN);
}

void led_life_off() {
	GPIO_ResetBits(LED_LIFE_IO_PORT, LED_LIFE_IO_PIN);
}

/******************************************************************************
* eeprom function
* when using function DATA_EEPROM_ProgramByte,
* must be DATA_EEPROM_unlock- DATA_EEPROM_ProgramByte- DATA_EEPROM_lock
*******************************************************************************/
uint8_t io_eeprom_read(uint32_t address, uint8_t* pbuf, uint32_t len) {
	uint32_t eeprom_address = 0;

	eeprom_address = address + EEPROM_BASE_ADDRESS;

	if ((uint8_t*)pbuf == (uint8_t*)0 || len == 0 ||(address + len)> EEPROM_MAX_SIZE) {
		return EEPROM_DRIVER_NG;
	}

	DATA_EEPROM_Unlock();
	mem_cpy(pbuf, (const uint8_t*)eeprom_address, len);
	DATA_EEPROM_Lock();

	return EEPROM_DRIVER_OK;
}

uint8_t io_eeprom_write(uint32_t address, uint8_t* pbuf, uint32_t len) {
	uint32_t eeprom_address = 0;
	uint32_t index = 0;
	uint32_t flash_status;

	eeprom_address = address + EEPROM_BASE_ADDRESS;

	if ((uint8_t*)pbuf == (uint8_t*)0 || len == 0 ||(address + len)> EEPROM_MAX_SIZE) {
		return EEPROM_DRIVER_NG;
	}

	DATA_EEPROM_Unlock();

	while (index < len) {
		flash_status = DATA_EEPROM_ProgramByte((eeprom_address + index), (uint32_t)(*(pbuf + index)));

		if(flash_status == FLASH_COMPLETE) {
			index++;
		}
		else {
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
							| FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
		}
	}

	DATA_EEPROM_Lock();

	return EEPROM_DRIVER_OK;
}

uint8_t io_eeprom_erase(uint32_t address, uint32_t len) {
	uint32_t eeprom_address = 0;
	uint32_t index = 0;
	uint32_t flash_status;

	eeprom_address = address + EEPROM_BASE_ADDRESS;

	if (len == 0 ||(address + len)> EEPROM_MAX_SIZE) {
		return EEPROM_DRIVER_NG;
	}

	DATA_EEPROM_Unlock();

	while(index < len) {
		sys_ctrl_soft_watchdog_reset();
		sys_ctrl_independent_watchdog_reset();

		flash_status = DATA_EEPROM_ProgramByte(eeprom_address + index, 0x00);

		if(flash_status == FLASH_COMPLETE) {
			index++;
		}
		else {
			FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
							| FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);
		}
	}

	DATA_EEPROM_Lock();

	return EEPROM_DRIVER_OK;
}

/*****************************************************************************
 *io uart for RS485
******************************************************************************/
void io_uart_rs485_cfg(uint32_t baudrate ) {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(USART_RS485_TX_GPIO_CLK | USART_RS485_RX_GPIO_CLK, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(USART_RS485_CLK, ENABLE);

	/* Connect PXx to USART_RS485_TX */
	GPIO_PinAFConfig(USART_RS485_TX_GPIO_PORT, USART_RS485_TX_SOURCE, USART_RS485_TX_AF);

	/* Connect PXx to USART_RS485_RX */
	GPIO_PinAFConfig(USART_RS485_RX_GPIO_PORT, USART_RS485_RX_SOURCE, USART_RS485_RX_AF);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = USART_RS485_TX_PIN;
	GPIO_Init(USART_RS485_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_RS485_RX_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(USART_RS485_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USART_RS485 configuration */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_RS485, &USART_InitStructure);

	/* Enable the USART_RS485 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART_RS485_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearITPendingBit(USART_RS485, USART_IT_RXNE);
	USART_ITConfig(USART_RS485, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART_RS485, ENABLE);
}

/******************************************************************************
* RS485 dir pin mode select
*******************************************************************************/

void pinmode_rs485_dir(uint8_t mode) {

	GPIO_InitTypeDef GPIO_InitStructure;
	if (mode == PIN_OUTPUT) {
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RS485_DIR_IO_CLOCK, ENABLE);

		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_Pin		= RS485_DIR_IO_PIN;

		GPIO_Init(RS485_DIR_IO_PORT, &GPIO_InitStructure);
	}
	else if (mode == PIN_INPUT) {
		RCC_APB2PeriphClockCmd(RS485_DIR_IO_CLOCK, ENABLE);

		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
		GPIO_InitStructure.GPIO_Pin		= RS485_DIR_IO_PIN;
		GPIO_Init(RS485_DIR_IO_PORT, &GPIO_InitStructure);
	}
}

void digitalwrite_high_rs485_dir(void) {
	GPIO_SetBits(RS485_DIR_IO_PORT, RS485_DIR_IO_PIN); // reset dir pin -> enable RX function
}

void digitalwrite_low_rs485_dir(void) {
	GPIO_ResetBits(RS485_DIR_IO_PORT, RS485_DIR_IO_PIN); // reset dir pin -> enable RX function
}

uint8_t digitalread_rs485_dir() {
	return GPIO_ReadOutputDataBit(RS485_DIR_IO_PORT, RS485_DIR_IO_PIN);
}

void io_uart_rs485_transfer(uint8_t byte) {
	while (USART_GetFlagStatus(USART_RS485, USART_FLAG_TXE) == RESET);
	/* put transnission data */
	USART_SendData(USART_RS485, (uint8_t)byte);
	/* wait transmission completed */
	while (USART_GetFlagStatus(USART_RS485, USART_FLAG_TC) == RESET);
}

uint8_t io_uart_RS485_get_byte() {
	volatile uint8_t c = 0;
	if(USART_GetITStatus(USART_RS485, USART_IT_RXNE) == SET) {
		c = (uint8_t)USART_ReceiveData(USART_RS485);
		APP_PRINT("%d:", c);
		USART_ClearITPendingBit(USART_RS485, USART_IT_RXNE);
	}
	return c;
}

void modbus_pin_select_address_init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t modbus_read_pin_select_status() {
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13);
}

void modbus_coil_init() {

	GPIO_ResetBits(RELAY_1_IO_PORT, RELAY_1_IO_PIN);
	GPIO_ResetBits(RELAY_2_IO_PORT, RELAY_2_IO_PIN);
	GPIO_ResetBits(RELAY_3_IO_PORT, RELAY_3_IO_PIN);
	GPIO_ResetBits(RELAY_4_IO_PORT, RELAY_4_IO_PIN);
	GPIO_ResetBits(RELAY_5_IO_PORT, RELAY_5_IO_PIN);
	GPIO_ResetBits(RELAY_6_IO_PORT, RELAY_6_IO_PIN);

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RELAY_1_IO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin		= RELAY_1_IO_PIN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(RELAY_1_IO_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RELAY_2_IO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin		= RELAY_2_IO_PIN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(RELAY_2_IO_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RELAY_3_IO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin		= RELAY_3_IO_PIN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(RELAY_3_IO_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RELAY_4_IO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin		= RELAY_4_IO_PIN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(RELAY_4_IO_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RELAY_5_IO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin		= RELAY_5_IO_PIN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(RELAY_5_IO_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RELAY_6_IO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin		= RELAY_6_IO_PIN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(RELAY_6_IO_PORT, &GPIO_InitStructure);

}

void io_rs485_relay1_pin_set() {
	APP_DBG("RELAY1		SET\n");
	GPIO_SetBits(RELAY_1_IO_PORT, RELAY_1_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay1_pin_reset() {
	APP_DBG("RELAY1		RESET\n");
	GPIO_ResetBits(RELAY_1_IO_PORT, RELAY_1_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay2_pin_set() {
	APP_DBG("RELAY2		SET\n");
	GPIO_SetBits(RELAY_2_IO_PORT, RELAY_2_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay2_pin_reset() {
	APP_DBG("RELAY2		RESET\n");
	GPIO_ResetBits(RELAY_2_IO_PORT, RELAY_2_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay3_pin_set() {
	APP_DBG("RELAY3		SET\n");
	GPIO_SetBits(RELAY_3_IO_PORT, RELAY_3_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay3_pin_reset() {
	APP_DBG("RELAY3		RESET\n");
	GPIO_ResetBits(RELAY_3_IO_PORT, RELAY_3_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay4_pin_set() {
	APP_DBG("RELAY4		SET\n");
	GPIO_SetBits(RELAY_4_IO_PORT, RELAY_4_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay4_pin_reset() {
	APP_DBG("RELAY4		RESET\n");
	GPIO_ResetBits(RELAY_4_IO_PORT, RELAY_4_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay5_pin_set() {
	APP_DBG("RELAY5		SET\n");
	GPIO_SetBits(RELAY_5_IO_PORT, RELAY_5_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay5_pin_reset() {
	APP_DBG("RELAY5		RESET\n");
	GPIO_ResetBits(RELAY_5_IO_PORT, RELAY_5_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay6_pin_set() {
	APP_DBG("RELAY6		SET\n");
	GPIO_SetBits(RELAY_6_IO_PORT, RELAY_6_IO_PIN); // reset dir pin -> enable RX function
}

void io_rs485_relay6_pin_reset() {
	APP_DBG("RELAY6		RESET\n");
	GPIO_ResetBits(RELAY_6_IO_PORT, RELAY_6_IO_PIN); // reset dir pin -> enable RX function
}


/**
  * @brief  Initialize UART Periphral and Enable Pin clock
  * @param  None
  * @return None
  */

void io_uart_3_init(uint16_t baudrate) {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(USART_CLI_TX_GPIO_CLK | USART_CLI_RX_GPIO_CLK, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(USART_CLI_CLK, ENABLE);

	/* Connect PXx to USART_CLI_TX */
	GPIO_PinAFConfig(USART_CLI_TX_GPIO_PORT, USART_CLI_TX_SOURCE, USART_CLI_TX_AF);

	/* Connect PXx to USART_CLI_RX */
	GPIO_PinAFConfig(USART_CLI_RX_GPIO_PORT, USART_CLI_RX_SOURCE, USART_CLI_RX_AF);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = USART_CLI_TX_PIN;
	GPIO_Init(USART_CLI_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_CLI_RX_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(USART_CLI_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USART_CLI configuration */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_CLI, &USART_InitStructure);

	/* Enable the USART_CLI Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART_CLI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearITPendingBit(USART_CLI, USART_IT_RXNE);
	USART_ITConfig(USART_CLI, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART_CLI, ENABLE);
}

/**
  * @brief  Send a chareter
  * @param  chr
  * @return None
  */
void io_uart_cli_transfer(uint8_t chr) {
	while (USART_GetFlagStatus(USART_CLI, USART_FLAG_TXE) == RESET);
	/* put transnission data */
	USART_SendData(USART_CLI, (uint8_t)chr);
	/* wait transmission completed */
	while (USART_GetFlagStatus(USART_CLI, USART_FLAG_TC) == RESET);
}

/**
  * @brief  get a charecter received
  * @param  None
  * @return a charecter received
  */
uint8_t io_uart_cli_get_byte() {
	volatile uint8_t c = 0;
	if(USART_GetITStatus(USART_CLI, USART_IT_RXNE) == SET) {
		c = (uint8_t)USART_ReceiveData(USART_CLI);
		APP_PRINT("%d:", c);
		USART_ClearITPendingBit(USART_CLI, USART_IT_RXNE);
	}
	return c;
}
