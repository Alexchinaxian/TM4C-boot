#include "main.h"
int main(void)
{
    UART_Config();
    TM4CSSI2_Config();
    while(1)
    {
        ReceivePacket(&rxBuffer);
        if(rxBuffer.ID == 0x01){
            UART_BOOT_ExecutiveCommand(&rxBuffer);
        }
    }
}
