#include "eeprom.h"

/*
 * hi2c1 is created in main.c
 */
extern I2C_HandleTypeDef hi2c1;


/*
 * Check whether EEPROM is connected
 */
HAL_StatusTypeDef EEPROM_IsReady(void)
{
    return HAL_I2C_IsDeviceReady(&hi2c1,
                                 EEPROM_I2C_ADDR,
                                 3,
                                 100);
}


/*
 * Write data to 24LC512
 */
HAL_StatusTypeDef EEPROM_Write(uint16_t memory_address,
                               uint8_t *data,
                               uint16_t size)
{
    HAL_StatusTypeDef status;

    while (size > 0)
    {
        uint16_t page_offset;
        uint16_t remaining;
        uint16_t write_size;

        /*
         * Find position inside current 128-byte page
         */
        page_offset = memory_address % EEPROM_PAGE_SIZE;

        /*
         * Bytes remaining in current page
         */
        remaining = EEPROM_PAGE_SIZE - page_offset;

        /*
         * Do not cross page boundary
         */
        if (size < remaining)
        {
            write_size = size;
        }
        else
        {
            write_size = remaining;
        }


        status = HAL_I2C_Mem_Write(&hi2c1,
                                   EEPROM_I2C_ADDR,
                                   memory_address,
                                   I2C_MEMADD_SIZE_16BIT,
                                   data,
                                   write_size,
                                   HAL_MAX_DELAY);

        if (status != HAL_OK)
        {
            return status;
        }


        /*
         * 24LC512 internal write cycle
         */
        HAL_Delay(5);


        memory_address += write_size;
        data += write_size;
        size -= write_size;
    }

    return HAL_OK;
}


/*
 * Read data from 24LC512
 */
HAL_StatusTypeDef EEPROM_Read(uint16_t memory_address,
                              uint8_t *data,
                              uint16_t size)
{
    return HAL_I2C_Mem_Read(&hi2c1,
                            EEPROM_I2C_ADDR,
                            memory_address,
                            I2C_MEMADD_SIZE_16BIT,
                            data,
                            size,
                            HAL_MAX_DELAY);
}
