#ifndef _CAN_DIY_H_
#define _CAN_DIY_H_
#include "can.h"
#include "stdio.h"

void Can_User_Init(void);

void sendmessage(uint32_t StdId, uint32_t ExtId, uint8_t IDE, uint8_t RTR, uint8_t DLC, float send_data);

void sendmessage(uint32_t StdId, uint32_t ExtId, uint8_t IDE, uint8_t RTR, uint8_t DLC, float send_data);
void sendOrder(uint32_t StdId, uint32_t ExtId, uint8_t IDE, uint8_t RTR, uint8_t DLC);

#endif