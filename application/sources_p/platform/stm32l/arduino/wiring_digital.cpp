#include "Arduino.h"

#include "app_dbg.h"
#include "../io_cfg.h"
#include "../../../sys/sys_dbg.h"

void pinMode(uint8_t pin, uint8_t mode) {
	if(pin > GPIO_LIST_SIZE) {
		FATAL("SN", 0x01);
	}
	else if (mode != OUTPUT && mode != INPUT) {
		FATAL("SN", 0x03);
	}
#if defined(ARDUINO_DBG_PIN_EN)
	if (mode != INPUT)
	APP_DBG("pinMode(%d, OUTPUT)\n", pin);
	else
	APP_DBG("pinMode(%d, INPUT)\n", pin);
#endif
	gpio_list[pin].init(OUTPUT);
	gpio_list[pin].mode_status = mode;
}

void digitalWrite(uint8_t pin, uint8_t pinval) {
	if (pin > GPIO_LIST_SIZE || gpio_list[pin].mode_status != OUTPUT) {
		FATAL("SN", 0x02);
	}
	if (pinval == HIGH) {
		gpio_list[pin].set();

		#if defined(ARDUINO_DBG_PIN_EN)
				APP_DBG("digitalWrite(%d, HIGH)\n", pin);
		#endif
	}
	else if (pinval == LOW) {
		gpio_list[pin].reset();
		#if defined(ARDUINO_DBG_PIN_EN)
				APP_DBG("digitalWrite(%d, LOW)\n", pin);
		#endif
	}
}

uint8_t digitalRead(uint8_t pin) {
	if (pin > GPIO_LIST_SIZE || gpio_list[pin].mode_status != INPUT) {
		FATAL("SN", 0x04);
	}
	return *(uint8_t *)gpio_list[pin].read;
}
