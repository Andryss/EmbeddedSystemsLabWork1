#ifndef INC_OLED_H_
#define INC_OLED_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "fonts.h"
#include "uart_util.h"

#define OLED_I2C_ADDR		0x78	/* ?хде? */
#define OLED_WIDTH			128
#define OLED_HEIGHT			64

typedef enum {
	Black = 0x00,
	White = 0x01
} OLED_COLOR;

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef oled_Init(void);
void oled_SetCursor(uint8_t x, uint8_t y);
void oled_ResetCursor(void);
bool oled_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color);
void oled_Fill(OLED_COLOR color);
bool oled_WriteChar(char ch, FontDef Font, OLED_COLOR color);
bool oled_WriteString(char* str, size_t size, FontDef Font, OLED_COLOR color);
HAL_StatusTypeDef oled_UpdateScreen(void);

#endif /* INC_OLED_H_ */
