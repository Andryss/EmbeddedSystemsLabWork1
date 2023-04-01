#include "keyboard.h"

HAL_StatusTypeDef keyboard_Init(void){
	HAL_StatusTypeDef status = PCA9538_SetDefaultConfig(KEYBOARD_I2C_ADDR);
	if (status != HAL_OK) {
		UART_Transmit("Can't set keyboard default config\n");
		return status;
	}
	uint8_t out_def = 0x00;
	status = PCA9538_Write_Register(KEYBOARD_I2C_ADDR, OUTPUT_PORT, &out_def);
	if (status != HAL_OK) {
		UART_Transmit("Can't set keyboard output port\n");
	}
	return status;
}

HAL_StatusTypeDef keyboard_CheckButton(BUTTON_t* btn){
	if (btn == NULL) return HAL_ERROR;

	HAL_StatusTypeDef status;
	uint8_t input_port;

	for (uint8_t i = 0; i < 4; i++) { // row
		uint8_t conf = ~(0x01 << i);

		status = PCA9538_Write_Register(KEYBOARD_I2C_ADDR, OUTPUT_PORT, &conf);
		if (status != HAL_OK) {
			UART_Transmit("Can't write keyboard config register\n");
			return status;
		}
		
		HAL_Delay(10);
		
		status = PCA9538_Read_Inputs(KEYBOARD_I2C_ADDR, &input_port);
		if (status != HAL_OK) {
			UART_Transmit("Can't read keyboard input port\n");
			return status;
		}

		input_port = (input_port >> 4) & 0x7; // col
		if (input_port) {
			if (!(input_port & 0x4)) {
				*btn = BTN_9 + i;
			} else if (!(input_port & 0x2)) {
				*btn = BTN_5 + i;
			} else if (!(input_port & 0x1)) {
				*btn = BTN_1 + i;
			}
			return HAL_OK;
		}

		HAL_Delay(10);
	}
	*btn = BTN_NULL;
	return HAL_OK;
}

HAL_StatusTypeDef keyboard_WaitButton(BUTTON_t* btn){
	if (btn == NULL) return HAL_ERROR;

	while(1){
		HAL_StatusTypeDef status = keyboard_CheckButton(btn);
		if (status != HAL_OK) {
			UART_Transmit("Can't check keyboard buttons\n");
			return status;
		}

		if (*btn != BTN_NULL) break;
		HAL_Delay(25);
	}
	return HAL_OK;
}

