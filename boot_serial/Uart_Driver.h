/*
 * Uart_Driver.h
 *
 *  Created on: 2022骞�6鏈�20鏃�
 *      Author: Xiao.Hexin.Alex
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "stdint.h"
typedef struct {
    union CRC
    {
        uint16_t CRC;
        struct
        {
         uint8_t crc_L;
         uint8_t crc_H;
        };
    }CRC;
    union Address
    {
        uint16_t Address;
        struct
        {
         uint8_t addressL;
         uint8_t addressH;
        };
    }ADDRESS;
    uint8_t packetData[100];
    uint8_t CMD;
    uint8_t ID;
} Receive_Package ;



union FlashBuff
    {
        uint32_t flash_buff[4*1024];
        struct flash
        {
         uint8_t buff_1;
         uint8_t buff_2;
         uint8_t buff_3;
         uint8_t buff_4;
        }flashbuff[4*1024];
    }FlashBuff;


typedef struct {
    union Send_CRC
    {
        uint16_t CRC;
        struct
        {
         uint8_t crc_L;
         uint8_t crc_H;
        };
    }CRC;
    union Send_Address
    {
        uint16_t Address;
        struct
        {
         uint8_t addressL;
         uint8_t addressH;
        };
    }ADDRESS;
    uint8_t packetData;
    uint8_t Function_Code;
    uint8_t ID;
} Send_Package ;
void UART_Config(void);
int BMS_ReceivePacket(Receive_Package *packet);
void BMS_UARTReceive(uint8_t *pui8Data, uint32_t ui32Size);
void UARTWriteByte(uint32_t uart_base, char byte);
void sendPacket(Send_Package *p);

#endif /* UART_DRIVER_H_ */
