#include "main.h"

void SSI2IntHandler(void){
    uint32_t ui32Status;
    //读取 SSIMIS（SSI 屏蔽中断状态）。
    ui32Status = MAP_SSIIntStatus(SSI2_BASE, true);
    //清除 SSI 中断。
    MAP_SSIIntClear(SSI2_BASE, ui32Status);
    //关闭 RX FIFO 中断。
    MAP_SSIIntDisable(SSI2_BASE, SSI_RXFF);
    g_breceiveFlag = 1;
}

void TM4CSSI2_Config(void){
    //启用 SSI2使用。
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    //配置引脚PD0 PD1 PD2 PD3
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    MAP_GPIOPinConfigure(GPIO_PD1_SSI2XDAT0);
    MAP_GPIOPinConfigure(GPIO_PD0_SSI2XDAT1);
    MAP_GPIOPinConfigure(GPIO_PD2_SSI2FSS);
    MAP_GPIOPinConfigure(GPIO_PD3_SSI2CLK);

    /*
    为 SPI 主模式配置和启用 SSI2_BASE 端口。
    */
    MAP_GPIOPinTypeSSI(GPIO_PORTD_BASE,GPIO_PIN_0 |
                   GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    MAP_SSIConfigSetExpClk(SSI2_BASE, g_ui32SysClock, SSI_FRF_MOTO_MODE_0,
                           SSI_MODE_MASTER, 2000000, 8); 
    //启用处理器中断。
    IntMasterEnable(); 
    //在 RX FIFO 满时启用 SSI1 中断。
    MAP_SSIIntEnable(SSI2_BASE, SSI_RXFF);
    // 在处理器 (NVIC) 上启用 SSI1 中断。
    IntEnable(INT_SSI2);
    //启用 SSI2模块。
    MAP_SSIEnable(SSI2_BASE);
}
