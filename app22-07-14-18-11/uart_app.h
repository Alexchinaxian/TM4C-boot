/*
 * uart_app.h
 *
 *  Created on: 2022年6月21日
 *      Author: Xiao.Hexin.Alex
 */

#ifndef UART_APP_H_
#define UART_APP_H_

typedef struct _ModBus_LIST{
  //Modbus相关命令
   uint8_t Read_one;
   uint8_t Read_more;
   uint8_t Write;
} ModBus_LIST,*PModBus_LIST;

typedef struct _CBL_CMD_LIST{
  //Bootloader相关命令
  unsigned char RequestUpdate;      //请求升级命令
  unsigned char Erase;              //擦除APP区域数据
  unsigned char ExitBootLoader;     //退出Boot loader
} CBL_CMD_LIST,*PCBL_CMD_LIST;


typedef struct _Function_LIST{
  //功能码
  uint16_t ControlRegister;        //控制寄存器
  uint16_t StatusRegister;         //状态寄存器
  uint16_t SelectUpgradeChip;      //选择升级芯片
  uint16_t HighDownloadFileSize;   //下载文件大小高位
  uint16_t LowDownloadFileSize;   //下载文件大小低位
  uint16_t UpgradeBytePacket;      //升级每包byte数
  uint16_t StartAddress;           //升级数据的起始地址
} Function_LIST,*P_Function_LIST;

typedef struct _StatusRegister{
    uint16_t Allow_Updata;
    uint16_t NotAll_Updata;
    uint16_t ShakeHands_Success;
    uint16_t ShakeHands_Fail;
    uint16_t ChipStatus_OK;
    uint16_t ChipStatus_NO;
    uint16_t EraseSector_OK;
    uint16_t EraseSector_NO;
    uint16_t BlockData_OK;
    uint16_t BlockData_NO;
    uint16_t Updata_Success;
    uint16_t Updata_Fail;

}StatusRegister_List,*PStatusRegister;
void UART_BOOT_ExecutiveCommand(Receive_Package *pRxMessage);
void JumpToBootLoader(void);
uint32_t SwapWord(uint32_t x);
#endif /* UART_APP_H_ */
