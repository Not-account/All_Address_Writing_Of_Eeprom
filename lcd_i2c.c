#include "lcd_i2c.h"


/*
 * hi2c1 is created in main.c
 */
extern I2C_HandleTypeDef hi2c1;


/*
 * PCF8574 control bits
 */
#define LCD_RS          0x01
#define LCD_RW          0x02
#define LCD_EN          0x04
#define LCD_BACKLIGHT   0x08


/*
 * Send byte to PCF8574
 */
static void LCD_ExpanderWrite(uint8_t data)
{
    uint8_t value;

    value = data | LCD_BACKLIGHT;

    HAL_I2C_Master_Transmit(&hi2c1,
                            LCD_I2C_ADDR,
                            &value,
                            1,
                            HAL_MAX_DELAY);
}


/*
 * Enable pulse
 */
static void LCD_EnablePulse(uint8_t data)
{
    LCD_ExpanderWrite(data | LCD_EN);

    HAL_Delay(1);

    LCD_ExpanderWrite(data & ~LCD_EN);

    HAL_Delay(1);
}


/*
 * Send 4 bits
 */
static void LCD_Send4Bits(uint8_t data)
{
    LCD_ExpanderWrite(data);

    LCD_EnablePulse(data);
}


/*
 * Send LCD command
 */
static void LCD_SendCommand(uint8_t command)
{
    uint8_t high;
    uint8_t low;

    high = command & 0xF0;

    low = (command << 4) & 0xF0;

    LCD_Send4Bits(high);

    LCD_Send4Bits(low);
}


/*
 * Send character
 */
static void LCD_SendData(uint8_t data)
{
    uint8_t high;
    uint8_t low;

    high = (data & 0xF0) | LCD_RS;

    low = ((data << 4) & 0xF0) | LCD_RS;

    LCD_Send4Bits(high);

    LCD_Send4Bits(low);
}


/*
 * LCD initialization
 */
void LCD_Init(void)
{
    HAL_Delay(50);


    /*
     * LCD power-up sequence
     */

    LCD_Send4Bits(0x30);

    HAL_Delay(5);


    LCD_Send4Bits(0x30);

    HAL_Delay(1);


    LCD_Send4Bits(0x30);

    HAL_Delay(1);


    LCD_Send4Bits(0x20);

    HAL_Delay(1);


    /*
     * 4-bit mode
     * 2 lines
     * 5x8 font
     */
    LCD_SendCommand(0x28);


    /*
     * Display ON
     * Cursor OFF
     * Blink OFF
     */
    LCD_SendCommand(0x0C);


    /*
     * Entry mode
     */
    LCD_SendCommand(0x06);


    /*
     * Clear display
     */
    LCD_SendCommand(0x01);

    HAL_Delay(2);
}


/*
 * Clear LCD
 */
void LCD_Clear(void)
{
    LCD_SendCommand(0x01);

    HAL_Delay(2);
}


/*
 * Set cursor
 *
 * row = 0 -> first line
 * row = 1 -> second line
 */
void LCD_SetCursor(uint8_t row,
                   uint8_t column)
{
    uint8_t address;

    if (row == 0)
    {
        address = 0x80 + column;
    }
    else
    {
        address = 0xC0 + column;
    }

    LCD_SendCommand(address);
}


/*
 * Print string
 */
void LCD_Print(char *string)
{
    while (*string != '\0')
    {
        LCD_SendData((uint8_t)*string);

        string++;
    }
}


/*
 * Print one byte in HEX
 *
 * Example:
 *
 * 0xFF -> FF
 * 0x25 -> 25
 */
void LCD_PrintHex8(uint8_t value)
{
    const char hex[] = "0123456789ABCDEF";

    LCD_SendData(hex[(value >> 4) & 0x0F]);

    LCD_SendData(hex[value & 0x0F]);
}
