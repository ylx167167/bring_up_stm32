#include "can_diy.h"

extern CAN_TxHeaderTypeDef Can_Tx;
extern uint8_t Txdata[8];
void Can_User_Init(void)
{
    CAN_FilterTypeDef sFilterConfig; // 设置过滤器
    HAL_StatusTypeDef Hal_status;
    sFilterConfig.FilterActivation = ENABLE;              // 开启过滤器
    sFilterConfig.FilterBank = 1;                         // 过滤器1
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;     // 设为掩码模式
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;    // 设置为32位
    sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO1; // 接收到的报文放到FIFO0中
    sFilterConfig.FilterIdHigh = 0;                       // 基本ID放入到STID中
    sFilterConfig.FilterIdLow = 0;
    sFilterConfig.FilterMaskIdHigh = 0;
    sFilterConfig.FilterMaskIdLow = 0;
    sFilterConfig.SlaveStartFilterBank = 0;

    Hal_status = HAL_CAN_ConfigFilter(&hcan, &sFilterConfig); // HAL层注册过滤器
    Hal_status = HAL_CAN_Start(&hcan);                        // 开启can
    if (Hal_status != HAL_OK)
    {
        printf("can开启失败\r\n");
    }
    printf("can开启成功\r\n");
    Hal_status = HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    if (Hal_status != HAL_OK)
    {
        printf("开启挂起中段允许失败\r\n");
    }
}

/*
 发送命令函数
 StdId 			标准帧ID
 ExtId 			扩展帧ID  当标志位 IDE为CAN_ID_STD时 扩展帧无效
 IDE 			扩展帧标志位  CAN_ID_STD为标准ID CAN_ID_EXT为使用扩展ID
 RTR  			0(CAN_RTR_DATA)为数据帧 1(CAN_RTR_REMOTE)为远程帧
 DLC  			数据长度
*/
void sendOrder(uint32_t StdId, uint32_t ExtId, uint8_t IDE, uint8_t RTR, uint8_t DLC)
{
    uint32_t pTxMailbox = 0;

    Can_Tx.StdId = StdId; // 标准ID
    Can_Tx.ExtId = ExtId; // 扩展ID
    Can_Tx.IDE = IDE;     // CAN_ID_STD为标准ID CAN_ID_EXT为使用扩展ID
    Can_Tx.RTR = RTR;     // 0(CAN_RTR_DATA)为数据帧 1(CAN_RTR_REMOTE)为远程帧
    Can_Tx.DLC = DLC;     // 数据长度
    printf("TX ID:0x%X\r\n", ExtId);
    printf("TX DATA:%02X%02X%02X%02X%02X%02X%02X%02X\r\n", Txdata[0], Txdata[1], Txdata[2], Txdata[3], Txdata[4], Txdata[5], Txdata[6], Txdata[7]);
    HAL_CAN_AddTxMessage(&hcan, &Can_Tx, Txdata, &pTxMailbox);
}
void sendmessage(uint32_t StdId, uint32_t ExtId, uint8_t IDE, uint8_t RTR, uint8_t DLC, float send_data)
{
    uint32_t pTxMailbox = 0;
    uint8_t i;
    Can_Tx.StdId = StdId; // 标准ID
    Can_Tx.ExtId = ExtId; // 扩展ID
    Can_Tx.IDE = IDE;     // CAN_ID_STD为标准ID CAN_ID_EXT为使用扩展ID
    Can_Tx.RTR = RTR;     // 0(CAN_RTR_DATA)为数据帧 1(CAN_RTR_REMOTE)为远程帧
    Can_Tx.DLC = DLC;     // 数据长度
    // 将浮点数转化成4个字节存在tdata[4]----tdata[7]中
    send_data = send_data * 100;
    Txdata[4] = (int)send_data & 0x00ff;
    Txdata[3] = (int)send_data >> 8;
    Txdata[1] = 0x01;
    printf("TX ID:0x%X\r\n", Can_Tx.ExtId);
    printf("TX DATA:%02X%02X%02X%02X%02X%02X%02X%02X\r\n", Txdata[0], Txdata[1], Txdata[2], Txdata[3], Txdata[4], Txdata[5], Txdata[6], Txdata[7]);
    HAL_CAN_AddTxMessage(&hcan, &Can_Tx, Txdata, &pTxMailbox);
}
