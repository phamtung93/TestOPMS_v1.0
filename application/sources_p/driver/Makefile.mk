CFLAGS		+= -I./sources/driver/led
CFLAGS		+= -I./sources/driver/sensor_digital

CPPFLAGS	+= -I./sources/driver/epprom
CPPFLAGS	+= -I./sources/driver/exor
CPPFLAGS	+= -I.sources/driver/Arduino-LiquidCrystal-I2C-library
CPPFLAGS	+= -I.sources/driver/LiquidCrystal-595
CPPFLAGS	+= -I./sources/driver/modbus_rtu
CPPFLAGS	+= -I./sources/driver/EmonLib
CPPFLAGS	+= -I./sources/driver/general_output

VPATH += sources/driver/led
VPATH += sources/driver/button
VPATH += sources/driver/eeprom
VPATH += sources/driver/sensor_digital
VPATH += sources/driver/exor
VPATH += sources/driver/Arduino-LiquidCrystal-I2C-library
VPATH += sources/driver/LiquidCrystal-595
VPATH += sources/driver/modbus_rtu
VPATH += sources/driver/EmonLib
VPATH += sources/driver/general_output

SOURCES += sources/driver/led/led.c
SOURCES += sources/driver/button/button.c
SOURCES += sources/driver/sensor_digital/sensor_digital.c


SOURCES_CPP += sources/driver/eeprom/eeprom.cpp
SOURCES_CPP += sources/driver/exor/exor.cpp
SOURCES_CPP += sources/driver/Arduino-LiquidCrystal-I2C-library/LiquidCrystal_I2C.cpp
SOURCES_CPP += sources/driver/LiquidCrystal-595/LiquidCrystal595.cpp
SOURCES_CPP += sources/driver/modbus_rtu/modbus_rtu.cpp
SOURCES_CPP += sources/driver/EmonLib/EmonLib.cpp
SOURCES_CPP += sources/driver/general_output/general_output.cpp
