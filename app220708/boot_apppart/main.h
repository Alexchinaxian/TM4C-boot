#ifndef MAIN_H_
#define MAIN_H_

#include <Uart_Driver.h>
#include "stdint.h"
#include "stdbool.h"
#include "uart_app.h"
#include "Uart_Driver.h"

#include "inc/hw_flash.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_uart.h"
#include "inc/hw_nvic.h"

#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/flash.h"
#include "driverlib/uart.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"


uint64_t g_ui32SysClock;
Send_Package TxMessage;
Receive_Package rxBuffer;
Receive_Package Buffer;
bool Reciveflag;
#endif /* MAIN_H_ */
