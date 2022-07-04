/*
 * main.c
 *
 *  Created on: 2022Äê7ÔÂ4ÈÕ
 *      Author: alex
 */

#include <stdint.h>
#include <stdbool.h>
#include "bl_config.h"
#include "bl_check.h"
#include "Uart_Driver.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
extern StartApplication();

void HandleUpdate() {
    UART_Config();
   boot_updater();
}

void main(void){
    if(CheckForceUpdate()){
        HandleUpdate();
    }
    else {
        StartApplication();
    }
}
