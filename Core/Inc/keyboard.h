#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#include "pca9538.h"

#define KEYBOARD_I2C_ADDR		0xE2	/* ?хде? */

typedef enum {
	BTN_NULL = 0,
	BTN_1 = 1,	BTN_5 = 5,	BTN_9 = 9,
	BTN_2 = 2,	BTN_6 = 6,	BTN_10 = 10,
	BTN_3 = 3,	BTN_7 = 7,	BTN_11 = 11,
	BTN_4 = 4,	BTN_8 = 8,	BTN_12 = 12
} BUTTON_t;

HAL_StatusTypeDef keyboard_Init(void);
HAL_StatusTypeDef keyboard_CheckButton(BUTTON_t* btn);
HAL_StatusTypeDef keyboard_WaitButton(BUTTON_t* btn);


static inline char keyboard_ConvertButtonPhoneCode(BUTTON_t button){
	switch(button){
		case BTN_1:
			return '1';
		case BTN_2:
			return '4';
		case BTN_3:
			return '7';
		case BTN_4:
			return '*';
		case BTN_5:
			return '2';
		case BTN_6:
			return '5';
		case BTN_7:
			return '8';
		case BTN_8:
			return '0';
		case BTN_9:
			return '3';
		case BTN_10:
			return '6';
		case BTN_11:
			return '9';
		case BTN_12:
			return '#';
		default:
			return 0;
	}
}

#endif /* INC_KEYBOARD_H_ */
