#include "../sys/sys_io.h"
#include "../sys/sys_dbg.h"

#include "../sys/sys_dbg.h"

#include "modbus_rtu.h"

#include "task_rs485_modbus.h"

/**
 * @brief
 * Full constructor for a Master/Slave through USB/RS232C
 *
 * @param u8id   node address 0=master, 1..247=slave
 * @param u8serno  serial port used 0..3
 * @ingroup setup
 * @overload Modbus::Modbus(uint8_t u8id, uint8_t u8serno)
 * @overload Modbus::Modbus(uint8_t u8id)
 * @overload Modbus::Modbus()
 */
Modbus::Modbus() {
	/*  None */
}

void Modbus::init(uint8_t u8id, uint8_t u8txenpin, long baudrate) {
	this->u8id = u8id;
	this->u8txenpin = u8txenpin;
	this->u16timeOut = 300;

	begin(baudrate);
}

/**
 * @brief
 * Method to write a new slave ID address
 *
 * @param 	u8id	new slave address between 1 and 247
 * @ingroup setup
 */
void Modbus::setID( uint8_t u8id) {
	if (( u8id != 0) && (u8id <= 247)) {
		this->u8id = u8id;
	}
}

/**
 * @brief
 * Method to read current slave ID address
 *
 * @return u8id	current slave address between 1 and 247
 * @ingroup setup
 */
uint8_t Modbus::getID() {
	return this->u8id;
}

/**
 * @brief
 * Initialize time-out parameter
 *
 * Call once class has been instantiated, typically within setup().
 * The time-out timer is reset each time that there is a successful communication
 * between Master and Slave. It works for both.
 *
 * @param time-out value (ms)
 * @ingroup setup
 */
void Modbus::setTimeOut( uint16_t u16timeOut) {
	this->u16timeOut = u16timeOut;
}

/**
 * @brief
 * Return communication Watchdog state.
 * It could be usefull to reset outputs if the watchdog is fired.
 *
 * @return TRUE if millis() > u32timeOut
 * @ingroup loop
 */
boolean Modbus::getTimeOutState() {
	return (millis() > u32timeOut);
}

/**
 * @brief
 * Get input messages counter value
 * This can be useful to diagnose communication
 *
 * @return input messages counter
 * @ingroup buffer
 */
uint16_t Modbus::getInCnt() {
	return u16InCnt;
}

/**
 * @brief
 * Get transmitted messages counter value
 * This can be useful to diagnose communication
 *
 * @return transmitted messages counter
 * @ingroup buffer
 */
uint16_t Modbus::getOutCnt() {
	return u16OutCnt;
}

/**
 * @brief
 * Get errors counter value
 * This can be useful to diagnose communication
 *
 * @return errors counter
 * @ingroup buffer
 */
uint16_t Modbus::getErrCnt() {
	return u16errCnt;
}

/**
 * Get modbus master state
 *
 * @return = 0 IDLE, = 1 WAITING FOR ANSWER
 * @ingroup buffer
 */
uint8_t Modbus::getState()
{
	return u8state;
}

/**
 * Get the last error in the protocol processor
 *
 * @returnreturn   NO_REPLY = 255      Time-out
 * @return   EXC_FUNC_CODE = 1   Function code not available
 * @return   EXC_ADDR_RANGE = 2  Address beyond available space for Modbus registers
 * @return   EXC_REGS_QUANT = 3  Coils or registers number beyond the available space
 * @ingroup buffer
 */
uint8_t Modbus::getLastError()
{
	return u8lastError;
}

/**
 * @brief
 * *** Only Modbus Master ***
 * Generate a query to an slave with a modbus_t telegram structure
 * The Master must be in COM_IDLE mode. After it, its state would be COM_WAITING.
 * This method has to be called only in loop() section.
 *
 * @see modbus_t
 * @param modbus_t  modbus telegram structure (id, fct, ...)
 * @ingroup loop
 * @todo finish function 15
 */
int8_t Modbus::query( modbus_t telegram)
{
	uint8_t u8regsno, u8bytesno;
	if (u8id!=0) return -2;
	if (u8state != COM_IDLE) return -1;

	if ((telegram.u8id==0) || (telegram.u8id>247)) return -3;

	au16regs = telegram.au16reg;

	// telegram header
	au8Buffer[ ID ]         = telegram.u8id;
	au8Buffer[ FUNC ]       = telegram.u8fct;
	au8Buffer[ ADD_HI ]     = highByte(telegram.u16RegAdd );
	au8Buffer[ ADD_LO ]     = lowByte( telegram.u16RegAdd );

	switch( telegram.u8fct )
	{
	case MB_FC_READ_COILS:
	case MB_FC_READ_DISCRETE_INPUT:
	case MB_FC_READ_REGISTERS:
	case MB_FC_READ_INPUT_REGISTER:
		au8Buffer[ NB_HI ]      = highByte(telegram.u16CoilsNo );
		au8Buffer[ NB_LO ]      = lowByte( telegram.u16CoilsNo );
		u8BufferSize = 6;
		break;
	case MB_FC_WRITE_COIL:
		au8Buffer[ NB_HI ]      = ((au16regs[0] > 0) ? 0xff : 0);
		au8Buffer[ NB_LO ]      = 0;
		u8BufferSize = 6;
		break;
	case MB_FC_WRITE_REGISTER:
		au8Buffer[ NB_HI ]      = highByte(au16regs[0]);
		au8Buffer[ NB_LO ]      = lowByte(au16regs[0]);
		u8BufferSize = 6;
		break;
	case MB_FC_WRITE_MULTIPLE_COILS: // TODO: implement "sending coils"
		u8regsno = telegram.u16CoilsNo / 16;
		u8bytesno = u8regsno * 2;
		if ((telegram.u16CoilsNo % 16) != 0) {
			u8bytesno++;
			u8regsno++;
		}

		au8Buffer[ NB_HI ]      = highByte(telegram.u16CoilsNo );
		au8Buffer[ NB_LO ]      = lowByte( telegram.u16CoilsNo );
		au8Buffer[ NB_LO+1 ]    = u8bytesno;
		u8BufferSize = 7;

		u8regsno = u8bytesno = 0; // now auxiliary registers
		for (uint16_t i = 0; i < telegram.u16CoilsNo; i++) {
		}
		break;

	case MB_FC_WRITE_MULTIPLE_REGISTERS:
		au8Buffer[ NB_HI ]      = highByte(telegram.u16CoilsNo );
		au8Buffer[ NB_LO ]      = lowByte( telegram.u16CoilsNo );
		au8Buffer[ NB_LO+1 ]    = (uint8_t) ( telegram.u16CoilsNo * 2 );
		u8BufferSize = 7;

		for (uint16_t i=0; i< telegram.u16CoilsNo; i++) {
			au8Buffer[ u8BufferSize ] = highByte( au16regs[ i ] );
			u8BufferSize++;
			au8Buffer[ u8BufferSize ] = lowByte( au16regs[ i ] );
			u8BufferSize++;
		}
		break;
	}

	sendTxBuffer();
	u8state = COM_WAITING;

	return 0;
}

/**
	Function get data from interrupt usart
 */
void Modbus::getData_serial(uint8_t c) {
	au8Buffer[u8BufferSize] = c;
	u8BufferSize ++;
	if (u8BufferSize >= MAX_BUFFER) {
		bBuffOverflow = true;	/*overload*/
	}
	else {
		bBuffOverflow = false;
	}

}

/**
 * @brief *** Only for Modbus Master ***
 * This method checks if there is any incoming answer if pending.
 * If there is no answer, it would change Master state to COM_IDLE.
 * This method must be called only at loop section.
 * Avoid any delay() function.
 *
 * Any incoming data would be redirected to au16regs pointer,
 * as defined in its modbus_t query telegram.
 *
 * @params	nothing
 * @return errors counter
 * @ingroup loop
 */

int8_t Modbus::poll()
{
	// transfer Serial buffer frame to auBuffer
	u8lastRec = 0;
	int8_t i8state = getRxBuffer();
	if (i8state == 0) {
		u8state = COM_IDLE;
		u8lastError = NO_REPLY;
		u16errCnt++;
		return i8state;
	}
	else if (i8state < 7) {
		u8state = COM_IDLE;
		u16errCnt++;
		return i8state;
	}

	// validate message: id, CRC, FCT, exception
	uint8_t u8exception = validateAnswer();
	if (u8exception != 0) {
		u8state = COM_IDLE;
		return u8exception;
	}

	// process answer
	switch( au8Buffer[ FUNC ] )
	{
	case MB_FC_READ_COILS:
	case MB_FC_READ_DISCRETE_INPUT:
		// call get_FC1 to transfer the incoming message to au16regs buffer
		get_FC1( );
		break;
	case MB_FC_READ_INPUT_REGISTER:
	case MB_FC_READ_REGISTERS :
		// call get_FC3 to transfer the incoming message to au16regs buffer
		get_FC3( );
		break;
	case MB_FC_WRITE_COIL:
	case MB_FC_WRITE_REGISTER :
	case MB_FC_WRITE_MULTIPLE_COILS:
	case MB_FC_WRITE_MULTIPLE_REGISTERS :
		// nothing to do
		break;
	default:
		break;
	}

	u8state = COM_IDLE;
	return u8BufferSize;
}

/**
 * @brief
 * *** Only for Modbus Slave ***
 * This method checks if there is any incoming query
 * Afterwards, it would shoot a validation routine plus a register query
 * Avoid any delay() function !!!!
 * After a successful frame between the Master and the Slave, the time-out timer is reset.
 *
 * @param *regs  register table for communication exchange
 * @param u8size  size of the register table
 * @return 0 if no query, 1..4 if communication error, >4 if correct query processed
 * @ingroup loop
 */
int8_t Modbus::poll( uint16_t *regs, uint8_t u8size ) {
	uint8_t u8exception;
	au16regs = regs;
	u8regsize = u8size;

	if (au8Buffer[ ID ] != u8id) {
		u8BufferSize = 0;
		return 0;
	}

	int8_t i8state = getRxBuffer();

	u8lastError = i8state;
	if (i8state < 7) {
		u8exception = EXC_BUFFER;
		if (u8exception != NO_REPLY) {
			buildException( u8exception );
			sendTxBuffer();
		}
		u8lastError = u8exception;
		u8BufferSize = 0;
		return i8state;
	}

	u8exception = validateRequest();

	if (u8exception > 0) {

		xprintf("checksum error:%d\n", u8exception);
		if (u8exception != NO_REPLY) {
			buildException( u8exception );
			sendTxBuffer();
		}

		u8lastError = u8exception;
		u8BufferSize = 0;

		return u8exception;
	}

	u32timeOut = millis() + long(u16timeOut);
	u8lastError = 0;

	/* process message */
	switch( au8Buffer[ FUNC ] ) {
	case MB_FC_READ_COILS:
	case MB_FC_READ_DISCRETE_INPUT:
		return process_FC1( regs, u8size );
		break;

	case MB_FC_READ_INPUT_REGISTER:
	case MB_FC_READ_REGISTERS :
		APP_DBG("MB_FC_READ_REGISTERS\n");
		return process_FC3( regs, u8size );
		break;

	case MB_FC_WRITE_COIL:
		APP_DBG("MB_FC_WRITE_COIL\n");
		return process_FC5( regs, u8size );
		break;

	case MB_FC_WRITE_REGISTER :
		APP_DBG("MB_FC_WRITE_REGISTER\n");
		return process_FC6( regs, u8size );
		break;

	case MB_FC_WRITE_MULTIPLE_COILS:
		APP_DBG("MB_FC_WRITE_MULTIPLE_COILS\n");
		return process_FC15( regs, u8size );
		break;

	case MB_FC_WRITE_MULTIPLE_REGISTERS :
		APP_DBG("MB_FC_WRITE_MULTIPLE_REGISTERS\n");
		return process_FC16( regs, u8size );
		break;

	default:
		break;
	}
	return i8state;
}

/* _____PRIVATE FUNCTIONS_____________________________________________________ */

/**
 * @brief
 * Initialize class object.
 *
 * Sets up the serial port using specified baud rate.
 * Call once class has been instantiated, typically within setup().
 *
 * @see http://arduino.cc/en/Serial/Begin#.Uy4CJ6aKlHY
 * @param speed   baud rate, in standard increments (300..115200)
 * @ingroup setup
 */
void Modbus::begin(uint32_t u32baudrate) {

	io_uart_rs485_cfg(u32baudrate);
	pinMode(u8txenpin, OUTPUT);
	GPIO_ResetBits(RS485_DIR_IO_PORT, RS485_DIR_IO_PIN);
	u8lastRec = u8BufferSize = 0;
	u16InCnt = u16OutCnt = u16errCnt = 0;
}

/**
 * @brief
 * Initialize default class object.
 *
 * Sets up the serial port using 19200 baud.
 * Call once class has been instantiated, typically within setup().
 *
 * @overload Modbus::begin(uint16_t u16BaudRate)
 * @ingroup setup
 */
void Modbus::begin() {
	begin(9600);
}

int8_t Modbus::getFunc() {
	return func;
}

void Modbus::ResetFunc() {
	func = 0;
}

/**
 * @brief
 * This method moves Serial buffer data to the Modbus au8Buffer.
 *
 * @return buffer size if OK, ERR_BUFF_OVERFLOW if u8BufferSize >= MAX_BUFFER
 * @ingroup buffer
 */

int8_t Modbus::getRxBuffer() {
	if (bBuffOverflow) {
		u16errCnt++;
		return ERR_BUFF_OVERFLOW;
	}
	u16InCnt++;

	return u8BufferSize;
}

/**
 * @brief
 * This method transmits au8Buffer to Serial line.
 * Only if u8txenpin != 0, there is a flow handling in order to keep
 * the RS485 transceiver in output state as long as the message is being sent.
 * This is done with UCSRxA register.
 * The CRC is appended to the buffer before starting to send it.
 *
 * @param nothing
 * @return nothing
 * @ingroup buffer
 */
void Modbus::sendTxBuffer() {
	/* append CRC to message */
	uint16_t u16crc = calcCRC( u8BufferSize );

	au8Buffer[ u8BufferSize ] = u16crc >> 8;
	u8BufferSize++;
	au8Buffer[ u8BufferSize ] = u16crc & 0x00ff;
	u8BufferSize++;

	digitalWrite(u8txenpin, HIGH);

	for (uint8_t i = 0; i < u8BufferSize; i++) {
		io_uart_rs485_transfer(au8Buffer[i]);
	}

	u8BufferSize = 0;
	bBuffOverflow = false;
	digitalWrite(u8txenpin, LOW);
	/* set time-out for master */
	u32timeOut = millis() + (uint32_t) u16timeOut;

	/* increase message counter */
	u16OutCnt++;
}

/**
 * @brief
 * This method calculates CRC
 *
 * @return uint16_t calculated CRC value for the message
 * @ingroup buffer
 */
uint16_t Modbus::calcCRC(uint8_t u8length)
{
	uint16_t temp, temp2, flag;  /*unsigned int temp, temp2, flag;*/
	temp = 0xFFFF;
	for (uint8_t i = 0; i < u8length; i++) {
		temp = temp ^ au8Buffer[i];
		for (uint8_t j = 1; j <= 8; j++) {
			flag = temp & 0x0001;
			temp >>=1;
			if (flag)
				temp ^= 0xA001;
		}
	}
	// Reverse byte order.
	temp2 = temp >> 8;
	temp = (temp << 8) | temp2;
	temp &= 0xFFFF;
	// the returned value is already swapped
	// crcLo byte is first & crcHi byte is last
	return temp;
}

/**
 * @brief
 * This method validates slave incoming messages
 *
 * @return 0 if OK, EXCEPTION if anything fails
 * @ingroup buffer
 */
uint8_t Modbus::validateRequest() {
	// check message crc vs calculated crc
	uint16_t u16MsgCRC =
			((au8Buffer[u8BufferSize - 2] << 8)
			| au8Buffer[u8BufferSize - 1]); // combine the crc Low & High bytes
	if ( calcCRC( u8BufferSize-2 ) != u16MsgCRC )
	{
		u16errCnt ++;
		return ERR_BAD_CRC;
	}

	// check fct code
	boolean isSupported = false;
	for (uint8_t i = 0; i< sizeof( fctsupported ); i++)
	{
		if (fctsupported[i] == au8Buffer[FUNC])
		{
			isSupported = 1;
			break;
		}
	}
	if (!isSupported)
	{
		u16errCnt ++;
		return EXC_FUNC_CODE;
	}

	// check start address & nb range
	uint16_t u16regs = 0;
	uint8_t u8regs;
	switch ( au8Buffer[ FUNC ] ) {
	case MB_FC_READ_COILS:
	case MB_FC_READ_DISCRETE_INPUT:
	case MB_FC_WRITE_MULTIPLE_COILS:
		u16regs = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ]) / 16;
		u16regs += word( au8Buffer[ NB_HI ], au8Buffer[ NB_LO ]) /16;
		u8regs  = (uint8_t) u16regs;
		//if (u8regs > u8regsize) return EXC_ADDR_RANGE;
		break;
	case MB_FC_WRITE_COIL:
		u16regs = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ]) / 16;
		u8regs  = (uint8_t) u16regs;
		//if (u8regs > u8regsize) return EXC_ADDR_RANGE;
		break;
	case MB_FC_WRITE_REGISTER :
		u16regs = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ]);
		u8regs = (uint8_t) u16regs;
		//if (u8regs > u8regsize) return EXC_ADDR_RANGE;
		break;
	case MB_FC_READ_REGISTERS :
	case MB_FC_READ_INPUT_REGISTER :
	case MB_FC_WRITE_MULTIPLE_REGISTERS :
		u16regs = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ]);
		u16regs += word( au8Buffer[ NB_HI ], au8Buffer[ NB_LO ]);
		u8regs = (uint8_t) u16regs;
		if (u8regs > u8regsize) return EXC_ADDR_RANGE;
		break;
	}
	return 0; // OK, no exception code thrown
}

/**
 * @brief
 * This method validates master incoming messages
 *
 * @return 0 if OK, EXCEPTION if anything fails
 * @ingroup buffer
 */
uint8_t Modbus::validateAnswer()
{
	// check message crc vs calculated crc
	uint16_t u16MsgCRC =
			((au8Buffer[u8BufferSize - 2] << 8)
			| au8Buffer[u8BufferSize - 1]); // combine the crc Low & High bytes
	if ( calcCRC( u8BufferSize-2 ) != u16MsgCRC )
	{
		u16errCnt ++;
		return NO_REPLY;
	}

	// check exception
	if ((au8Buffer[ FUNC ] & 0x80) != 0)
	{
		u16errCnt ++;
		return ERR_EXCEPTION;
	}

	// check fct code
	boolean isSupported = false;
	for (uint8_t i = 0; i< sizeof( fctsupported ); i++)
	{
		if (fctsupported[i] == au8Buffer[FUNC])
		{
			isSupported = 1;
			break;
		}
	}
	if (!isSupported)
	{
		u16errCnt ++;
		return EXC_FUNC_CODE;
	}

	return 0; // OK, no exception code thrown
}

/**
 * @brief
 * This method builds an exception message
 *
 * @ingroup buffer
 */
void Modbus::buildException( uint8_t u8exception ) {

	/* get the original FUNC code */
	uint8_t u8func = au8Buffer[ FUNC ];

	au8Buffer[ ID ]      = u8id;
	au8Buffer[ FUNC ]    = u8func + 0x80;

	au8Buffer[ 2 ]       = u8exception;
	u8BufferSize         = EXCEPTION_SIZE;
}

/**
 * This method processes functions 1 & 2 (for master)
 * This method puts the slave answer into master data buffer
 *
 * @ingroup register
 * TODO: finish its implementation
 */
void Modbus::get_FC1() {
	uint8_t u8byte;
	u8byte = 0;
	printf("BYTE: %02X\n",u8byte);
}

/**
 * This method processes functions 3 & 4 (for master)
 * This method puts the slave answer into master data buffer
 *
 * @ingroup register
 */
void Modbus::get_FC3() {
	uint8_t u8byte, i;
	u8byte = 3;

	for (i=0; i< au8Buffer[ 2 ] /2; i++)
	{
		au16regs[ i ] = word(
					au8Buffer[ u8byte ],
					au8Buffer[ u8byte +1 ]);
		u8byte += 2;
	}
}

/**
 * @brief
 * This method processes functions 1 & 2
 * This method reads a bit array and transfers it to the master
 *
 * @return u8BufferSize Response to master length
 * @ingroup discrete
 */

int8_t Modbus::process_FC1( uint16_t *regs, uint8_t u8size )
{
	uint8_t u8bytesno, u8bitsno;
	uint8_t u8CopyBufferSize;
	uint16_t u16currentCoil, u16coil;

	// get the first and last coil from the message
	uint16_t u16StartCoil = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ] ); //0
	uint16_t u16Coilno    = word( au8Buffer[ NB_HI ], au8Buffer[ NB_LO ] ); // 12

	// put the number of bytes in the outcoming message
	u8bytesno = (uint8_t) (u16Coilno / 8); // 12/8 = 1
	if (u16Coilno % 8 != 0) u8bytesno ++;  // 1++ = 2 byte to respone
	au8Buffer[ ADD_HI ]  = u8bytesno; //   save numper of byte coil in outcoming message
	u8BufferSize         = ADD_LO;

	// read each coil from the register map and put its value inside the outcoming message
	u8bitsno = 0;

	relay_t coils;
	eeprom_read(EEPROM_RELAY_STATUS_ADDR, (uint8_t*)&coils, sizeof(relay_t));

	for (u16currentCoil = 0; u16currentCoil < u16Coilno; u16currentCoil++) {
		u16coil = u16StartCoil + u16currentCoil;

		bitWrite(au8Buffer[ u8BufferSize ], u8bitsno, func1_read_coil_status(u16coil, &coils));
		u8bitsno ++;

		if (u8bitsno > 7) {
			u8bitsno = 0;
			u8BufferSize++;
		}
	}

	// send outcoming message
	if (u16Coilno % 8 != 0) u8BufferSize ++;
	u8CopyBufferSize = u8BufferSize +2;
	sendTxBuffer();

	func = MB_FC_READ_COILS;
	return u8CopyBufferSize;
}

/**
 * @brief
 * This method processes functions 3 & 4
 * This method reads a word array and transfers it to the master
 *
 * @return u8BufferSize Response to master length
 * @ingroup register
 */
int8_t Modbus::process_FC3( uint16_t *regs, uint8_t u8size ) {
	uint16_t u8StartAdd = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ] );
	uint16_t u8regsno = word( au8Buffer[ NB_HI ], au8Buffer[ NB_LO ] );
	uint8_t u8CopyBufferSize;
	uint8_t i;

	au8Buffer[ 2 ]       = u8regsno * 2;
	u8BufferSize         = 3;

	for (i = u8StartAdd; i < u8StartAdd + u8regsno; i++) {
		if (u8StartAdd == FUNC3_GET_INFO) {
			fatal_log_t app_fatal_log;
			eeprom_read(EEPROM_FATAL_LOG_ADDR, (uint8_t*)&app_fatal_log, sizeof(fatal_log_t));

			au8Buffer[ u8BufferSize ] = HARDWARE_VER;
			u8BufferSize++;
			au8Buffer[ u8BufferSize ] = SOFTWARE_VER;
			u8BufferSize++;
			au8Buffer[ u8BufferSize ] = u8id;
			u8BufferSize++;
			au8Buffer[ u8BufferSize ] = DEVICE_TYPE;
			u8BufferSize++;
			au8Buffer[ u8BufferSize ] = (uint8_t)app_fatal_log.restart_times;
			u8BufferSize++;
			au8Buffer[ u8BufferSize ] = (uint8_t)app_fatal_log.fatal_times;
			u8BufferSize++;
		}
	}

	u8CopyBufferSize = u8BufferSize +2;
	sendTxBuffer();

	func = MB_FC_READ_REGISTERS;
	return u8CopyBufferSize;
}

/**
 * @brief
 * This method processes function 5
 * This method writes a value assigned by the master to a single bit
 *
 * @return u8BufferSize Response to master length
 * @ingroup discrete
 */
int8_t Modbus::process_FC5( uint16_t *regs, uint8_t u8size ) {
	(void)(regs);
	(void)(u8size);

	uint8_t u8exception = 0;
	uint8_t u8CopyBufferSize;

	uint16_t u16coil = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ]);
	uint16_t coil_addr = u16coil -1;

	uint16_t coil_status = word( au8Buffer[ NB_HI ], au8Buffer[ NB_LO ]);

	if (coil_addr >= OTB_TOTAL_COIL) {
		u8exception = EXC_ADDR_RANGE;
		buildException( u8exception );
		sendTxBuffer();
		u8BufferSize = 0;
		return 1;
	}

	/* write to coil */
	register_list.func5.callback(coil_addr, coil_status);
	register_list.status[coil_addr] = coil_status;

	/* send answer to master */
	u8BufferSize = 6;
	u8CopyBufferSize = u8BufferSize + 2;
	sendTxBuffer();

	func = MB_FC_WRITE_COIL;
	return u8CopyBufferSize;
}

/**
 * @brief
 * This method processes function 6
 * This method writes a value assigned by the master to a single word
 *
 * @return u8BufferSize Response to master length
 * @ingroup register
 */
int8_t Modbus::process_FC6( uint16_t *regs, uint8_t u8size ) {
	uint16_t u8add = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ] );
	uint8_t u8CopyBufferSize;
	uint16_t u16val = word( au8Buffer[ NB_HI ], au8Buffer[ NB_LO ] );

	uint16_t coil_addr = u8add - 1;

	/* allway check first */
	if (u8add == slave_reset_addr) {
		APP_DBG("[MODBUS] reset slave modbus and fatal log\n");

		/* reset fatal log and reboot*/
		fatal_log_t app_fatal_log;
		mem_set((uint8_t*)&app_fatal_log, 0, sizeof(fatal_log_t));
		eeprom_write(EEPROM_FATAL_LOG_ADDR, (uint8_t*)&app_fatal_log, sizeof(fatal_log_t));

		register_list.func6.callback(u8add, u16val);
	}
	else if (register_list.status[coil_addr] == 0) {
		register_list.func6.callback(coil_addr, u16val);
	}
	else {
		APP_DBG("device is turned off\n");
	}

	// keep the same header
	u8BufferSize         = RESPONSE_SIZE;

	u8CopyBufferSize = u8BufferSize +2;
	sendTxBuffer();

	func = MB_FC_WRITE_REGISTER;
	return u8CopyBufferSize;
}

/**
 * @brief
 * This method processes function 15
 * This method writes a bit array assigned by the master
 *
 * @return u8BufferSize Response to master length
 * @ingroup discrete
 */
int8_t Modbus::process_FC15( uint16_t *regs, uint8_t u8size ) {
	uint8_t u8currentRegister, u8currentBit, u8frameByte, u8bitsno;
	uint8_t u8CopyBufferSize;
	uint16_t u16currentCoil, u16coil;
	boolean bTemp;

	// get the first and last coil from the message
	uint16_t u16StartCoil = word( au8Buffer[ ADD_HI ], au8Buffer[ ADD_LO ] );
	uint16_t u16Coilno = word( au8Buffer[ NB_HI ], au8Buffer[ NB_LO ] );

	// read each coil from the register map and put its value inside the outcoming message
	u8bitsno = 0;
	u8frameByte = 7;
	for (u16currentCoil = 0; u16currentCoil < u16Coilno; u16currentCoil++) {

		u16coil = u16StartCoil + u16currentCoil;
		u8currentRegister = (uint8_t) (u16coil / 16);
		u8currentBit = (uint8_t) (u16coil % 16);

		bTemp = bitRead(
					au8Buffer[ u8frameByte ],
					u8bitsno );

		bitWrite(
					regs[ u8currentRegister ],
					u8currentBit,
					bTemp );

		u8bitsno ++;

		if (u8bitsno > 7)
		{
			u8bitsno = 0;
			u8frameByte++;
		}
	}

	// send outcoming message
	// it's just a copy of the incomping frame until 6th byte
	u8BufferSize         = 6;
	u8CopyBufferSize = u8BufferSize +2;
	sendTxBuffer();
	return u8CopyBufferSize;
}

/**
 * @brief
 * This method processes function 16
 * This method writes a word array assigned by the master
 *
 * @return u8BufferSize Response to master length
 * @ingroup register
 */
int8_t Modbus::process_FC16( uint16_t *regs, uint8_t u8size ) {
	//uint8_t u8func = au8Buffer[ FUNC ];  // get the original FUNC code
	uint8_t u8StartAdd = au8Buffer[ ADD_HI ] << 8 | au8Buffer[ ADD_LO ];
	uint8_t u8regsno = au8Buffer[ NB_HI ] << 8 | au8Buffer[ NB_LO ];
	uint8_t u8CopyBufferSize;
	uint8_t i;
	uint16_t temp;

	/* build header */
	au8Buffer[ NB_HI ]		= 0;
	au8Buffer[ NB_LO ]		= u8regsno;
	u8BufferSize			= RESPONSE_SIZE;

	/* write registers */
	for (i = 0; i < u8regsno; i++) {
		temp = word(au8Buffer[ (BYTE_CNT + 1) + i * 2 ], au8Buffer[ (BYTE_CNT + 2) + i * 2 ]);
		regs[ u8StartAdd + i ] = temp;
	}

	u8CopyBufferSize = u8BufferSize +2;
	sendTxBuffer();

	return u8CopyBufferSize;
}
