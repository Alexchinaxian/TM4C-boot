/*
 * Uart_Driver.c
 *
 *  Created on: 2022��6��20��
 *      Author: Xiao.Hexin.Alex
 */
#include "main.h"
void UART_Config(void){
g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |
                                     SYSCTL_OSC_MAIN |
                                     SYSCTL_USE_PLL |
                                 SYSCTL_CFG_VCO_480),80000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
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
    IntMasterEnable();
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
 }

void UARTIntHandler(void){
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true);
    ReceivePacket(&rxBuffer);
    memcpy(&Buffer,&rxBuffer,sizeof(Receive_Package));
    Reciveflag = 1;
    UARTIntClear(UART0_BASE, ui32Status);

}
void UARTReceive(uint8_t *pui8Data, uint32_t ui32Size){
    while(ui32Size--)
    {
        while((HWREG(UART0_BASE + UART_O_FR) & UART_FR_RXFE))
        {
        }
        *pui8Data++ = HWREG(UART0_BASE + UART_O_DR);
    }
}
int ReceivePacket(Receive_Package *packet){
    uint16_t index;
    packet->ID = UARTCharGet(UART0_BASE);
    packet->CMD = UARTCharGet(UART0_BASE);
    packet->ADDRESS.addressH= UARTCharGet(UART0_BASE);
    packet->ADDRESS.addressL= UARTCharGet(UART0_BASE);
    if(packet->ADDRESS.Address == 0x6001){
        packet->packetData[1] = UARTCharGet(UART0_BASE);
    }
    else if(packet->ADDRESS.Address == 0x6002){
       for (index = 0;index < 3; index++) {
           packet->packetData[index] = UARTCharGet(UART0_BASE);
       }
    }
    else{
    }
    packet->CRC.crc_H= UARTCharGet(UART0_BASE);
    packet->CRC.crc_L= UARTCharGet(UART0_BASE);
    return(0);
}


void sendPacket(Send_Package *p) {
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2);
    UARTWriteByte(UART0_BASE, p->ID);
    UARTWriteByte(UART0_BASE, p->Function_Code);
    UARTWriteByte(UART0_BASE, p->ADDRESS.addressH);
    UARTWriteByte(UART0_BASE, p->ADDRESS.addressL);
    if(p->ADDRESS.Address == 0x6003){
        return;
    }
    UARTWriteByte(UART0_BASE, p->packetData);
    UARTWriteByte(UART0_BASE,p->CRC.crc_H );
    UARTWriteByte(UART0_BASE,p->CRC.crc_L );
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);
}
void UARTWriteByte(uint32_t uart_base, char byte) {
    UARTCharPut(uart_base, byte);
    while(UARTBusy(uart_base));
}
