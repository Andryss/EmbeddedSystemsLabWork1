#include "uart_util.h"

HAL_StatusTypeDef UART_Transmit(char* message){
	return HAL_UART_Transmit(&huart4, (uint8_t*) message, strlen(message), 100);
}
