#ifndef SPI_Driver_H
#define SPI_Driver_H

bool g_breceiveFlag ;
void TM4CSSI2_Config(void);
void SSI2IntHandler(void);
void vSPI_Receive(uint8_t *ucp_Data, uint16_t us_Size);
void vSPI_Transmit(uint8_t *ucp_Data, uint16_t us_Size);
uint8_t ucSPI_RW(uint8_t uc_SendData);
#endif
