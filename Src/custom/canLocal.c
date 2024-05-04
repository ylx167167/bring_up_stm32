#include "canlocal.h"'
#include "can.h"

/**
 *  @brief 初始化CAN滤波器，以及开启CAN
 *  @param
 *  @return
 */

void CAN_Local_Init()
{
    /*1step:设置CAN1滤波器参数 */
    CAN_FilterTypeDef sFilterConfig;         // 设置过滤器
    sFilterConfig.FilterActivation = ENABLE; // 使能过滤器
    sFilterConfig.FilterBank = 0; // 过滤器编号，使用一个CAN，则可选0-13；使用两个CAN可选0-27
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;  // 过滤器模式，掩码模式或列表模式
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; // 过滤器位宽
    sFilterConfig.FilterIdHigh = 0x0000;               // 过滤器验证码ID高16位，0-0xFFFF
    sFilterConfig.FilterIdLow = 0x0000;                // 过滤器验证码ID低16位，0-0xFFFF
    sFilterConfig.FilterMaskIdHigh = 0x0000;           // 过滤器掩码ID高16位，0-0xFFFF
    sFilterConfig.FilterMaskIdLow = 0x0000;            // 过滤器掩码ID低16位，0-0xFFFF
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; // FIFOx，0或1
    sFilterConfig.SlaveStartFilterBank = 14; // 从过滤器编号，0-27，对于单CAN实例该参数没有意义
    if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
        printf("can过滤器注册失败\r\n");

    /*2step:开启CAN1 */
    if (HAL_CAN_Start(&hcan) != HAL_OK)
        printf("can开启失败\r\n");

    /*3step:启动can 接收提醒*/
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        printf("CAN Notification Error\r\n");
}

/**
 *  @brief
 *  @param
 *  @return
 */
uint8_t CAN_Send_Msg(uint8_t *msg, uint8_t len)
{
    uint8_t i = 0;
    uint8_t message[8];
    uint32_t TxMailbox;
    CAN_TxHeaderTypeDef CAN_TxHeader;

    // 1step:设置数据帧帧头
    CAN_TxHeader.StdId = 0x12;       // 标准标识符(12bit)
    CAN_TxHeader.ExtId = 0x12;       // 扩展标识符(29bit)
    CAN_TxHeader.IDE = CAN_ID_STD;   // 使用标准帧
    CAN_TxHeader.RTR = CAN_RTR_DATA; // 数据帧
    CAN_TxHeader.DLC = len;          // 发送长度
    CAN_TxHeader.TransmitGlobalTime = DISABLE;

    // 2step:装载数据
    for (i = 0; i < len; i++)
    {
        message[i] = msg[i];
    }

    // 3step:发送CAN消息
    if (HAL_CAN_AddTxMessage(&hcan, &CAN_TxHeader, message, &TxMailbox) != HAL_OK)
        return 1;

    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 3)
    {
    }
    return 0;
}
/**
 *  @brief 重现中断回调数据 接收can总线上的数据
 *  @param
 *  @return
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{ // 收到CAN数据会触发接收中断，进入该回调函数
    uint32_t i;
    uint8_t RxData[8];
    CAN_RxHeaderTypeDef CAN_RxHeader;

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_RxHeader, RxData) == HAL_OK)
    {
        // 串口打印接收结果
        printf("GetRxMessage, CANID:0x%0X, Data:", CAN_RxHeader.StdId);
        for (i = 0; i < CAN_RxHeader.DLC; i++)
        {
            printf("%02x ", RxData[i]);
        }
        // 把接收的数据用CAN再发回去
        CAN_Send_Msg(RxData, CAN_RxHeader.DLC);
    }
}
