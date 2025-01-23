#include "self_test.h"
#include "can.h"
#include "tim.h"
#include "stm32f4xx_hal_def.h"
#include "types.h"

void Test_Init() {
    CAN_Configuration();
    if (CAN_Send_Test() != Status_OK) {
        ErrorHandler();
        return;
    }
    if (CAN_Receive_Test() != Status_OK) {
        ErrorHandler();
        return;
    }

    PUMP_Test();
    SERVO_Test(&htim1, TIM_CHANNEL_1);
    SERVO_Test(&htim1, TIM_CHANNEL_2);
    SERVO_Test(&htim1, TIM_CHANNEL_3);
    MX_CAN1_Init();
}

void CAN_Configuration() {
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;

    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
        ErrorHandler();
        return;
    }

    hcan1.Instance = CAN1;
    hcan1.Init.Mode = CAN_MODE_LOOPBACK;
    hcan1.Init.Prescaler = 6;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan1) != HAL_OK) {
        ErrorHandler();
        return;
    }
    if (HAL_CAN_Start(&hcan1) != HAL_OK) {
        ErrorHandler();
        return;
    }
}

Status CAN_Send_Test() {
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailBox;
    uint8_t TxData[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    TxHeader.StdId = 0x00;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailBox) != HAL_OK) {
        return Status_ERROR;
    }

    return Status_OK;
}

Status CAN_Receive_Test() {
    CAN_RxHeaderTypeDef rxHeader;
    uint8_t rxData[8];

    if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0) {
        return Status_ERROR;
    }

    if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK) {
        return Status_ERROR;
    }

    if (rxHeader.StdId != 0x00) {
        return Status_ERROR;
    }

    return Status_OK;
}

void SERVO_Test(TIM_HandleTypeDef* tim, uint32_t channel) {
    for(int i = 500; i <= 2500; i += 10) {
        __HAL_TIM_SET_COMPARE(tim, channel, i);
        HAL_Delay(20);
    }
}

void ToggleDelay(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin, uint32_t delay_ms) {
    HAL_GPIO_TogglePin(GPIO_Port, GPIO_Pin);
    HAL_Delay(delay_ms);
    HAL_GPIO_TogglePin(GPIO_Port, GPIO_Pin);
}
void LED_Test(){
	ToggleDelay(LED_1_GPIO_Port, LED_1_Pin, 500);
	ToggleDelay(LED_2_GPIO_Port, LED_2_Pin, 500);
	ToggleDelay(LED_3_GPIO_Port, LED_3_Pin, 500);
	ToggleDelay(LED_4_GPIO_Port, LED_4_Pin, 500);

}
void PUMP_Test() {
	ToggleDelay(PUMP_1_GPIO_Port, PUMP_1_Pin, 500);
	ToggleDelay(PUMP_2_GPIO_Port, PUMP_2_Pin, 500);
    ToggleDelay(PUMP_3_GPIO_Port, PUMP_3_Pin, 500);
    ToggleDelay(PUMP_4_GPIO_Port, PUMP_4_Pin, 500);
}

void ErrorHandler() {
    HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
}
