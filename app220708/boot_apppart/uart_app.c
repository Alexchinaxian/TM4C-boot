/*
 * uart_app.c
 *
 *  Created on: 2022年6月21日
 *      Author: Xiao.Hexin.Alex
 */
#include "main.h"
ModBus_LIST Mod_List = {
    .Read_one = 0x03,
    .Read_more = 0x06,
    .Write= 0x10
};
static bool UpdateFlasg = 0;
CBL_CMD_LIST CMD_List ={
    .RequestUpdate = 0x7E,   //请求升级命令
    .Erase = 0x0A,  //擦除APP区域数据
    .ExitBootLoader = 0x0B      //退出Boot loader
};
Function_LIST Fonction_List = {
    .ControlRegister = 0x6000,       //控制寄存器
    .StatusRegister = 0x6001,         //状态寄存器
    .SelectUpgradeChip = 0x6002,    //选择升级芯片
    .HighDownloadFileSize  = 0x6003,   //下载文件大小高位
    .LowDownloadFileSize = 0x6004, //下载文件大小低位
    .UpgradeBytePacket = 0x6005,    //升级每包byte数
    .StartAddress = 0x6006           //升级数据的起始地址
};
StatusRegister_List StatusRegister_list = {
  .Allow_Updata = 0xAA,
  .NotAll_Updata = 0xBB,
  .ShakeHands_Success =  0xCC,
  .ShakeHands_Fail =  0x33,
  .ChipStatus_OK = 0x40,
  .ChipStatus_NO = 0x41,
  .EraseSector_OK = 0x42,
  .EraseSector_NO = 0x43,
  .BlockData_OK = 0x44,
  .BlockData_NO = 0x45,
  .Updata_Success = 0xDD,
  .Updata_Fail = 0x55
};
void UART_BOOT_ExecutiveCommand(Receive_Package *pRxMessage){
    if(pRxMessage->ADDRESS.Address == Fonction_List.ControlRegister){
        if (pRxMessage->packetData[0] == 0x7E){
            TxMessage.Function_Code = Mod_List.Write;
            TxMessage.ADDRESS.Address = Fonction_List.ControlRegister;
            TxMessage.packetData = 0x01;
        }
    }
    else  if(pRxMessage->ADDRESS.Address == Fonction_List.StatusRegister){
    //请求升级
        if(pRxMessage->packetData[0] == CMD_List.RequestUpdate){
            TxMessage.Function_Code = 0x03;
            TxMessage.ADDRESS.Address = 0x02;
           //条件
           TxMessage.packetData = StatusRegister_list.Allow_Updata;
        }
    }
    //选择芯片
   else if(pRxMessage->ADDRESS.Address == Fonction_List.SelectUpgradeChip){
       //选择TM4C
       if(pRxMessage->packetData[2] == 0xF1){
           TxMessage.Function_Code = Mod_List.Write;
           TxMessage.ADDRESS.Address = Fonction_List.SelectUpgradeChip;
           TxMessage.packetData = 0x01;
           UpdateFlasg = 1;
       }

       else if(pRxMessage->packetData[2] == 0xF2){
           TxMessage.Function_Code = Mod_List.Write;
           TxMessage.ADDRESS.Address = Fonction_List.SelectUpgradeChip;
           TxMessage.packetData = 0x01;
       }
       //预留
       else{

       }
   }
   else{

   }
    TxMessage.ID = pRxMessage->ID;
    sendPacket(&TxMessage);
    if(UpdateFlasg){
        JumpToBootLoader();
    }
}
void JumpToBootLoader(void)
{
    // We must make sure we turn off SysTick and its interrupt before entering
    // the boot loader!
    //
    HWREG(NVIC_ST_CTRL) &= ~(NVIC_ST_CTRL_INTEN);
    HWREG(NVIC_ST_CTRL) &= ~(NVIC_ST_CTRL_ENABLE);
    //
    // Disable all processor interrupts.  Instead of disabling them
    // one at a time, a direct write to NVIC is done to disable all
    // peripheral interrupts.
    //
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;
    HWREG(NVIC_DIS2) = 0xffffffff;
    HWREG(NVIC_DIS3) = 0xffffffff;

    //
    // Return control to the boot loader.  This is a call to the SVC
    // handler in the boot loader.
    //
    (*((void (*)(void))(*(uint32_t *)0x2c)))();
}
uint32_t SwapWord(uint32_t x)
{
    __asm("    rev     r0, r0\n"
          "    bx      lr\n"); // need this to make sure r0 is returned
    return(x + 1); // return makes compiler happy - ignored
}
