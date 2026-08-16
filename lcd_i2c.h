#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "main.h"

/*
 * JHD-2X16-I2C
 *
 * Common PCF8574 address:
 * 0x27
 *
 * HAL address:
 * 0x27 << 1 = 0x4E
 */

#define LCD_I2C_ADDR    (0x27 << 1)


void LCD_Init(void);

void LCD_Clear(void);

void LCD_SetCursor(uint8_t row,
                   uint8_t column);

void LCD_Print(char *string);

void LCD_PrintHex8(uint8_t value);

#endif
