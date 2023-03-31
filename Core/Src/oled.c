#include "oled.h"

static uint8_t OLED_Buffer[1024];

typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} OLED_t;

static OLED_t OLED;


static HAL_StatusTypeDef oled_WriteCommands(uint8_t* commands, size_t size) {
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1,OLED_I2C_ADDR,0x00,1,commands,size,10*size);
	if (status != HAL_OK) {
		UART_Transmit("Can't write to OLED display (many commands)\n");
	}
	return status;
}

static uint8_t init_commands[] = {
	0xAE, 0x20, 0x10, 0xB0,
	0xC8, 0x00, 0x10, 0x40,
	0x81, 0xFF, 0xA1, 0xA6,
	0xA8, 0x3F, 0xA4, 0xD3,
	0x00, 0xD5, 0xF0, 0xD9,
	0x22, 0xDA, 0x12, 0xDB,
	0x20, 0x8D, 0x14, 0xAF
};

HAL_StatusTypeDef oled_Init(void) {
	HAL_Delay(100);

	HAL_StatusTypeDef status = oled_WriteCommands(init_commands, sizeof(init_commands));
	if (status != HAL_OK) {
		UART_Transmit("Can't initialize OLED display\n");
		return status;
	}

	oled_Fill(Black);
	status = oled_UpdateScreen();
	if (status != HAL_OK) {
		UART_Transmit("Can't update OLED display\n");
		return status;
	}

	oled_ResetCursor();

	OLED.Initialized = 1;
	return status;
}

void oled_SetCursor(uint8_t x, uint8_t y) {
	OLED.CurrentX = x;
	OLED.CurrentY = y;
}

void oled_ResetCursor(void) {
	oled_SetCursor(0, 0);
}

bool oled_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR color) {
	if (x >= OLED_WIDTH || y >= OLED_HEIGHT) return false;

	int16_t byte_addr = x + (y / 8) * OLED_WIDTH;
	uint8_t position = 1 << (y % 8);
	if (color == White) {
		OLED_Buffer[byte_addr] |= position;
	} else {
		OLED_Buffer[byte_addr] &= ~position;
	}
	return true;
}

void oled_Fill(OLED_COLOR color) {
	for(uint32_t i = 0; i < sizeof(OLED_Buffer); i++) {
		OLED_Buffer[i] = (color == White) ? 0xFF : 0x00;
	}
}

bool oled_WriteChar(char ch, FontDef Font, OLED_COLOR color) {
	if (OLED_WIDTH <= (OLED.CurrentX + Font.FontWidth) ||
			OLED_HEIGHT <= (OLED.CurrentY + Font.FontHeight)) {
		return false;
	}

	for (uint32_t i = 0; i < Font.FontHeight; i++) {
		uint32_t b = Font.data[(ch - ' ') * Font.FontHeight + i];
		for (uint32_t j = 0; j < Font.FontWidth; j++) {
			if ((b << j) & 0x8000) {
				oled_DrawPixel(OLED.CurrentX + j, OLED.CurrentY + i, (OLED_COLOR) color);
			} else {
				oled_DrawPixel(OLED.CurrentX + j, OLED.CurrentY + i, (OLED_COLOR)!color);
			}
		}
	}

	OLED.CurrentX += Font.FontWidth;

	return true;
}

bool oled_WriteString(char* str, size_t size, FontDef Font, OLED_COLOR color) {
	while (size-- > 0) {
		if (!oled_WriteChar(*str, Font, color)) {
			return false;
		}
		str++;
	}
	return true;
}

static uint8_t updscr_commands[] = {
	0xB0, 0x00, 0x10
};

HAL_StatusTypeDef oled_UpdateScreen(void) {
	HAL_StatusTypeDef status;

	for (uint8_t i = 0; i < 8; i++) {
		updscr_commands[0] = 0xB0 + i;

		status = oled_WriteCommands(updscr_commands, sizeof(updscr_commands));
		if (status != HAL_OK) {
			UART_Transmit("Can't set OLED display write address\n");
			return status;
		}

		status = HAL_I2C_Mem_Write(&hi2c1,OLED_I2C_ADDR,0x40,1,&(OLED_Buffer[OLED_WIDTH * i]),OLED_WIDTH,100);
		if (status != HAL_OK) {
			UART_Transmit("Can't send OLED display screen line\n");
			return status;
		}
	}

	return status;
}
