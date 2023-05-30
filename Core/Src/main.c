#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_rtc.h"

#include <stdio.h>

// Ð?a ch? b?t d?u c?a EEPROM
#define EEPROM_START_ADDRESS 0x080E0000

// Ð?a ch? luu gi?a giá tr? ngày gi?
#define DATETIME_EEPROM_ADDRESS EEPROM_START_ADDRESS

// Khai báo bi?n hrtc
RTC_HandleTypeDef hrtc;


// Khai báo bi?n luu tr? giá tr? ngày gi?
RTC_DateTypeDef date;
RTC_TimeTypeDef time;

// Hàm ghi d? li?u vào EEPROM
void EEPROM_Write(uint32_t address, uint32_t data)
{
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);
    HAL_FLASH_Lock();
}

// Hàm d?c d? li?u t? EEPROM
uint32_t EEPROM_Read(uint32_t address)
{
    return (uint32_t)address;
}

int main(void)
{
    // Kh?i t?o c?u hình cho RTC
    RTC_HandleTypeDef hrtc;
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    HAL_RTC_Init(&hrtc);

    // Ð?c giá tr? ngày gi? hi?n hành t? RTC
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

    // Ghi giá tr? ngày gi? vào EEPROM
    EEPROM_Write(DATETIME_EEPROM_ADDRESS, time.Seconds);
    EEPROM_Write(DATETIME_EEPROM_ADDRESS + 4, time.Minutes);
    EEPROM_Write(DATETIME_EEPROM_ADDRESS + 8, time.Hours);
    EEPROM_Write(DATETIME_EEPROM_ADDRESS + 12, date.Date);
    EEPROM_Write(DATETIME_EEPROM_ADDRESS + 16, date.Month);
    EEPROM_Write(DATETIME_EEPROM_ADDRESS + 20, date.Year);

    // Ð?c l?i giá tr? t? EEPROM d? ki?m tra
    uint32_t seconds = EEPROM_Read(DATETIME_EEPROM_ADDRESS);
    uint32_t minutes = EEPROM_Read(DATETIME_EEPROM_ADDRESS + 4);
    uint32_t hours = EEPROM_Read(DATETIME_EEPROM_ADDRESS + 8);
    uint32_t day = EEPROM_Read(DATETIME_EEPROM_ADDRESS + 12);
    uint32_t month = EEPROM_Read(DATETIME_EEPROM_ADDRESS + 16);
    uint32_t year = EEPROM_Read(DATETIME_EEPROM_ADDRESS + 20);

    // In ra giá tr? d?c du?c t? EEPROM
    printf("Date and time stored in EEPROM: %02d/%02d/%02d %02d:%02d:%02d\r\n", day, month, year, hours, minutes, seconds);

    while (1)
    {
        // Chuong trình chính
    }
}