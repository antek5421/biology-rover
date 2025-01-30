#include "self_test.h"
#include "can.h"
#include "tim.h"
#include "stm32f4xx_hal_def.h"
#include "types.h"

uint8_t count = 0;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint32_t TxMailbox;

uint8_t TxData[8];
uint8_t RxData[8];


void Test_Init() {
//    PUMP_Test();
	CAN_Test();
    SERVO_Test();
    LED_Test();
}

void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t angle)
{
    // Ensure the angle is within 0-180 degrees
    if(angle > 180) angle = 180;
    if(angle < 0) angle = 0;

    // Map the angle to pulse width (1000 to 2000)
    uint16_t pulse_width = 1000 + (angle * 1000 / 180);

    // Set the pulse width for the corresponding channel
    __HAL_TIM_SET_COMPARE(htim, Channel, pulse_width);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,&RxHeader, RxData);
	count++;
}

void CAN_Test() {
	  HAL_CAN_Start(&hcan1);

	  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

	  TxHeader.DLC = 1;
	  TxHeader.ExtId = 0;
	  TxHeader.IDE = CAN_ID_STD;
	  TxHeader.RTR = CAN_RTR_DATA;
	  TxHeader.StdId = 0x103;
	  TxHeader.TransmitGlobalTime = DISABLE;

	  TxData[0] = 0xf3;

	  HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
	  HAL_CAN_RxFifo0MsgPendingCallback(&hcan1);
}

Status CAN_Send_Test() {
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailBox;
    uint8_t TxData[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    TxHeader.StdId = 0x123;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailBox) != HAL_OK) {
        return Status_ERROR;
    }

    while (HAL_CAN_IsTxMessagePending(&hcan1, TxMailBox)) {}

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

    if (rxHeader.StdId != 0x123) {
        return Status_ERROR;
    }

    for (int i = 0; i < 8; i++) {
        if (rxData[i] != (i + 1)) {
            return Status_ERROR;
        }
    }

    return Status_OK;
}

void SERVO_Test() {

	  set_servo_angle(&htim1, TIM_CHANNEL_1, 180);
      set_servo_angle(&htim3, TIM_CHANNEL_3, 180);
      set_servo_angle(&htim3, TIM_CHANNEL_4, 180);

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
