/*
 * self_test.h
 *
 *  Created on: Oct 12, 2024
 *      Author: antoni
 */

#ifndef INC_SELF_TEST_H_
#define INC_SELF_TEST_H_

#include "types.h"
#include "tim.h"

void CAN_Test();

void LED_Test();

void SERVO_Test();
void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t angle);

void PUMP_Test();
#endif /* INC_SELF_TEST_H_ */
