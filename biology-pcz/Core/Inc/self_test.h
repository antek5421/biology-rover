/*
 * self_test.h
 *
 *  Created on: Oct 12, 2024
 *      Author: antoni
 */

#ifndef INC_SELF_TEST_H_
#define INC_SELF_TEST_H_

#include "types.h"

void CAN_Test_Init();
Status CAN_Send_Test();
Status CAN_Receive_Test();

void LED_Test();

void SERVO_Test();

void PUMP_Test();
#endif /* INC_SELF_TEST_H_ */
