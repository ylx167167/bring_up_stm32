#include <cmsis_os.h>
#include <stm32f1xx_hal.h>

struct CAN_context
{
    CAN_HandleTypeDef *handle;
    uint8_t node_id;
    uint64_t serial_number;

    uint32_t node_ids_in_use_0[4]; // 128 bits (indicate if a node ID was in use up to 1 second ago)
    uint32_t node_ids_in_use_1[4]; // 128 bits (indicats if a node ID was in use 1-2 seconds ago)

    uint32_t last_heartbeat_mailbox;
    uint32_t tx_msg_cnt;

    uint8_t node_id_rng_state;

    osSemaphoreId_t sem_send_heartbeat;

    // count occurrence various callbacks
    uint32_t TxMailboxCompleteCallbackCnt;
    uint32_t TxMailboxAbortCallbackCnt;
    int RxFifo0MsgPendingCallbackCnt;
    int RxFifo0FullCallbackCnt;
    int RxFifo1MsgPendingCallbackCnt;
    int RxFifo1FullCallbackCnt;
    int SleepCallbackCnt;
    int WakeUpFromRxMsgCallbackCnt;
    int ErrorCallbackCnt;

    uint32_t received_msg_cnt;
    uint32_t received_ack;
    uint32_t unexpected_errors;
    uint32_t unhandled_messages;
};