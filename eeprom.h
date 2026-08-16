#ifndef EEPROM_H
#define EEPROM_H

#include "main.h"

/*
 * 24LC512
 *
 * A0 = GND
 * A1 = GND
 * A2 = GND
 *
 * 7-bit address = 0x50
 *
 * HAL uses 8-bit shifted address:
 * 0x50 << 1 = 0xA0
 */

#define EEPROM_I2C_ADDR      (0x50 << 1)

#define EEPROM_SIZE          65536U
#define EEPROM_PAGE_SIZE     128U

HAL_StatusTypeDef EEPROM_IsReady(void);

HAL_StatusTypeDef EEPROM_Write(uint16_t memory_address,
                               uint8_t *data,
                               uint16_t size);

HAL_StatusTypeDef EEPROM_Read(uint16_t memory_address,
                              uint8_t *data,
                              uint16_t size);

#endif
