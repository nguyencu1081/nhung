#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"

RTC_HandleTypeDef hrtc;

void saveDateTimeToEEPROM(uint32_t dateTime)
{
    HAL_FLASHEx_DATAEEPROM_Unlock();  // M? kh�a Flash EEPROM

    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080000, dateTime);  // Luu gi� tr? v�o d?a ch? EEPROM (0x08080000)

    HAL_FLASHEx_DATAEEPROM_Lock();  // Kh�a Flash EEPROM
}

uint32_t loadDateTimeFromEEPROM()
{
    return (__IO uint32_t)0x08080000;  // �?c gi� tr? t? d?a ch? EEPROM (0x08080000)
}

int main(void)
{
    HAL_Init();

    // Kh?i t?o RTC
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 0x7F;
    hrtc.Init.SynchPrediv = 0xFF;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    HAL_RTC_Init(&hrtc);

    // L?y th?i gian hi?n t?i
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

    // Chuy?n d?i th?i gian v� ng�y th�nh m?t gi� tr? 32-bit
    uint32_t dateTime = ((date.Year + 2000) << 16) | (date.Month << ?? | date.Date;
    dateTime = (dateTime << 16) | (time.Hours << ?? | time.Minutes;

    // Luu gi� tr? v�o EEPROM
    saveDateTimeToEEPROM(dateTime);

    // �?c gi� tr? t? EEPROM
    uint32_t storedDateTime = loadDateTimeFromEEPROM();

    // Gi?i m� gi� tr? th�nh th?i gian v� ng�y
    uint8_t storedYear = (storedDateTime >> 24) - 2000;
    uint8_t storedMonth = (storedDateTime >> 16) & 0xFF;
    uint8_t storedDay = (storedDateTime >> ?? & 0xFF;
    uint8_t storedHour = storedDateTime & 0xFF;
    uint8_t storedMinute = (storedDateTime >> ?? & 0xFF;

    while (1)
    {
        // V�ng l?p ch�nh c?a chuong tr�nh
        // ...
    }
}