#ifndef __APP_EEPROM_H__
#define __APP_EEPROM_H__
#include <stdint.h>
#include "app.h"

/**
  *****************************************************************************
  * EEPROM define address.
  *
  *****************************************************************************
  */
#define EEPROM_START_ADDR								(0X0000)
#define EEPROM_END_ADDR									(0X1000)

#define EEPROM_APP_SETTING_ADDR							(0x0000)		/* setting information */

#define EEPROM_RELAY_STATUS_ADDR						(0x0900)		/* relay status backup */

#define EEPROM_FATAL_LOG_ADDR							(0x0F00)		/* fatal information */

#endif //__APP_EEPROM_H__
