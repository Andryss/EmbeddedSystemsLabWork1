#ifndef INC_UART_UTIL_H_
#define INC_UART_UTIL_H_

#include <string.h>
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart4;

HAL_StatusTypeDef UART_Transmit(char* message);

#endif /* INC_UART_UTIL_H_ */
