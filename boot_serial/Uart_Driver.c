/*
 * Uart_Driver.c
 *
 *  Created on: 2022骞�6鏈�20鏃�
 *      Author: Xiao.Hexin.Alex
 */
#include "stdint.h"
#include "stdbool.h"
#include "Uart_Driver.h"
#include "inc/hw_flash.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_uart.h"
#include "inc/hw_nvic.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/flash.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"


uint32_t g_ui32SysClock;
void UART_Config(void)
{
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |
                                         SYSCTL_OSC_MAIN |
                                         SYSCTL_USE_PLL |
                                     SYSCTL_CFG_VCO_480),80000000);

        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//uart0瀵瑰簲鐨勫璁惧紩鑴氫负PA0锛孭A1
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                                (UART_CONFIG_WLEN_8 |  UART_CONFIG_STOP_ONE |
                                 UART_CONFIG_PAR_NONE));
        IntEnable(INT_UART0);
        UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
 }
void BMS_UARTReceive(uint8_t *pui8Data, uint32_t ui32Size){
    while(ui32Size--)
    {
        while((HWREG(UART0_BASE + UART_O_FR) & UART_FR_RXFE))
        {
        }
        *pui8Data++ = HWREG(UART0_BASE + UART_O_DR);
    }
}
int BMS_ReceivePacket(Receive_Package *packet){
    int index;
    uint8_t num;
    packet->ID = UARTCharGet(UART0_BASE);
    packet->CMD = UARTCharGet(UART0_BASE);
    packet->ADDRESS.addressH= UARTCharGet(UART0_BASE);
    packet->ADDRESS.addressL= UARTCharGet(UART0_BASE);
    if(packet->ADDRESS.Address == 0x6000){
        if(packet->CMD == 0x06){
            for (index = 0;index < 1; index++) {
                      packet->packetData[index] = UARTCharGet(UART0_BASE);
            }
        }
        else if (packet->CMD == 0x10){
            for (index = 0;index < 3; index++) {
                      packet->packetData[index] = UARTCharGet(UART0_BASE);
            }
        }
    }
    else if(packet->ADDRESS.Address == 0x6001){
       for (index = 0;index < 1; index++) {
           packet->packetData[index] = UARTCharGet(UART0_BASE);
       }
    }
    else  if(packet->ADDRESS.Address == 0x6002){
        for (index = 0;index < 3; index++) {
            packet->packetData[index] = UARTCharGet(UART0_BASE);
        }
     }
    else  if(packet->ADDRESS.Address == 0x6003){
        if(packet->CMD == 0x10){
            for (index = 0;index < 8; index++) {
                packet->packetData[index] = UARTCharGet(UART0_BASE);
            }
        }
        else if(packet->CMD == 0x03){
            for (index = 0;index < 1; index++) {
                packet->packetData[index] = UARTCharGet(UART0_BASE);
            }
        }
     }
    else if(packet->ADDRESS.Address == 0x6006){
        if(packet->CMD == 0x10){
            packet->packetData[0]= UARTCharGet(UART0_BASE);
            num = 16;
            for (index = 1; index < num; index++) {
                packet->packetData[index] = UARTCharGet(UART0_BASE);
            }
        }
    }
    else{
    }
    packet->CRC.crc_H= UARTCharGet(UART0_BASE);
    packet->CRC.crc_L= UARTCharGet(UART0_BASE);
    return (0);
}
void sendPacket(Send_Package *p) {
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2);
    UARTWriteByte(UART0_BASE, p->ID);
    UARTWriteByte(UART0_BASE, p->Function_Code);
    UARTWriteByte(UART0_BASE, p->ADDRESS.addressH);
    UARTWriteByte(UART0_BASE, p->ADDRESS.addressL);
    UARTWriteByte(UART0_BASE, p->packetData);
    UARTWriteByte(UART0_BASE,p->CRC.crc_H );
    UARTWriteByte(UART0_BASE,p->CRC.crc_L );
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);
}
void UARTWriteByte(uint32_t uart_base, char byte) {
    UARTCharPut(uart_base, byte);
    while(UARTBusy(uart_base));
}
