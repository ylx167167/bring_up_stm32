
#include "can.h"
#include "gConfig.h"

#include "canLocal.h"
#include "gpio.h"
#include "syscall.h"

#include "struct_can.h"

#if (HARDWARE_CAN == 1) && (EXP_CAN == 1)
uint8_t gSendData[8] = {'8', '2', '3', '4', '5', '6', '7', '8'};
uint16_t g_sendTimes = 0;
uint16_t g_revTimes = 0;
uint16_t g_openFlag = 0;

struct CAN_context can1Ctx;

/**
 *  @brief 初始化CAN滤波器，以及开启CAN
 *  @param
 *  @return
 */
void CAN_Local_Init(void)
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
        // printf("can过滤器注册失败\r\n");
        g_openFlag = 1;

    /*2step:启动can 接收提醒*/
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        // printf("CAN Notification Error\r\n");
        g_openFlag = 3;

    /*3step:开启CAN1 */
    if (HAL_CAN_Start(&hcan) != HAL_OK)
        // printf("can开启失败\r\n");
        g_openFlag = 2;
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
    uint32_t txmailBox;
    CAN_TxHeaderTypeDef CAN_TxHeader;

    // 1step:设置数据帧帧头
    CAN_TxHeader.StdId = 0x13;       // 标准标识符(12bit)
    CAN_TxHeader.ExtId = 0x13;       // 扩展标识符(29bit)
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
    if (HAL_CAN_AddTxMessage(&hcan, &CAN_TxHeader, message, &txmailBox) != HAL_OK)
        return 1;
    else
        return 2;
}

void OnCanMessage(CAN_HandleTypeDef *canHandle, CAN_RxHeaderTypeDef *_head, uint8_t *_data)
{
    uint8_t id = 0;
    uint8_t cmd = 0;
    // ID 32~0x7FF. 4+7=11
    if (canHandle->Instance == CAN1)
    {
        id = _head->StdId >> 7;    // 4Bits ID & 7Bits Msg
        cmd = _head->StdId & 0x7F; // 4Bits ID & 7Bits Msg
        switch (cmd)
        {
        case 0x7F:
            Led_Ctrl(LED_DOWM_C13);
            break;
        case 0x02:
            Led_Ctrl(LED_UP_C13);
            break;
        default:

            break;
        }
    }
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
    uint8_t _sendbuf[3] = {'1', '6', '7'};

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_RxHeader, RxData) == HAL_OK)
    {
        // 串口打印接收结果
        printf("GetRxMessage, CANID:0x%0X, Data:", CAN_RxHeader.StdId);
        for (i = 0; i < CAN_RxHeader.DLC; i++)
        {
            printf("%02x ", RxData[i]);
        }
        // 处理Can接收到的数据
        OnCanMessage(hcan, &CAN_RxHeader, RxData);
        // 把接收的数据用CAN再发回去
        CAN_Send_Msg(_sendbuf, 3);
    }
}

void Exp_LocalCan_send(void)
{
    // g_sendTimes = CAN_Send_Msg(gSendData, 8);
    delay_ms_soft(500);
}

#endif