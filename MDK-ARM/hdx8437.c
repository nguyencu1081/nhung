#include "hdx8437.h"
#include "LCD_lib.h"
#include "math.h"
#include <stdlib.h>

SPI_HandleTypeDef HSPI1;

uint8_t spi1_communication(uint8_t send_char)
{
	uint8_t chRetry = 0;
	uint8_t chTemp = 0;

	while(!(__HAL_SPI_GET_FLAG(&HSPI1, SPI_FLAG_TXE))) {
		if (++ chRetry > 200) {
			return 0;
		}
	}

	HAL_SPI_Transmit(&HSPI1, &send_char, 1, HAL_MAX_DELAY);

	chRetry=0;
	while (!(__HAL_SPI_GET_FLAG(&HSPI1, SPI_FLAG_RXNE))) {
		if (++ chRetry > 200) {
			return 0;
		}
	}

	chTemp = HAL_SPI_Receive(&HSPI1, &send_char, 1, HAL_MAX_DELAY);
	
	/* Wait until the BSY flag is set */ 
	while(__HAL_SPI_GET_FLAG(&HSPI1, SPI_FLAG_BSY)) {
	}
	
	return chTemp;
}

void spi1_init(void)
{

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	__HAL_RCC_SPI1_CLK_ENABLE();
	
	
	HSPI1.Instance = SPI1;
  HSPI1.Init.Mode = SPI_MODE_MASTER;
  HSPI1.Init.Direction = SPI_DIRECTION_2LINES;
  HSPI1.Init.DataSize = SPI_DATASIZE_8BIT;
  HSPI1.Init.CLKPolarity = SPI_POLARITY_LOW;
  HSPI1.Init.CLKPhase = SPI_PHASE_1EDGE;
  HSPI1.Init.NSS = SPI_NSS_SOFT;
  HSPI1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  HSPI1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  HSPI1.Init.TIMode = SPI_TIMODE_DISABLE;
  HSPI1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  HSPI1.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&HSPI1);
}


void lcd_write_byte(uint8_t chByte, uint8_t chCmd)
{
   if(chCmd)
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	 else
		 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	 spi1_communication(chByte);
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}

void lcd_write_word(uint16_t hwData)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		spi1_communication(hwData >> 8);
		spi1_communication(hwData & 0xFF);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}

void lcd_write_register(uint8_t chRegister, uint8_t chValue)
{
	lcd_write_byte(chRegister, 0);
	lcd_write_byte(chValue, 1);
}

void lcd_ctrl_port_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	

	RCC->AHB1ENR = RCC->AHB1ENR | RCC_AHB1ENR_GPIOBEN;
	
	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_6 | GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
//	GPIO_InitStruct.Pin = GPIO_PIN_4;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	GPIO_InitStruct.Pull= GPIO_PULLUP;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}

void lcd_init(void)
{
  lcd_ctrl_port_init();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
  spi1_init();
	
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

	lcd_write_register(0xEA,0x00);
	lcd_write_register(0xEB,0x20);
	lcd_write_register(0xEC,0x0C);
	lcd_write_register(0xED,0xC4);
	lcd_write_register(0xE8,0x38);
	lcd_write_register(0xE9,0x10);
	lcd_write_register(0xF1,0x01);
	lcd_write_register(0xF2,0x10);
	lcd_write_register(0x40,0x01); 
	lcd_write_register(0x41,0x00); 
	lcd_write_register(0x42,0x00); 
	lcd_write_register(0x43,0x10); 
	lcd_write_register(0x44,0x0E); 
	lcd_write_register(0x45,0x24); 
	lcd_write_register(0x46,0x04); 
	lcd_write_register(0x47,0x50); 
	lcd_write_register(0x48,0x02); 
	lcd_write_register(0x49,0x13); 
	lcd_write_register(0x4A,0x19); 
	lcd_write_register(0x4B,0x19); 
	lcd_write_register(0x4C,0x16); 
	lcd_write_register(0x50,0x1B); 
	lcd_write_register(0x51,0x31); 
	lcd_write_register(0x52,0x2F); 
	lcd_write_register(0x53,0x3F); 
	lcd_write_register(0x54,0x3F); 
	lcd_write_register(0x55,0x3E); 
	lcd_write_register(0x56,0x2F); 
	lcd_write_register(0x57,0x7B); 
	lcd_write_register(0x58,0x09); 
	lcd_write_register(0x59,0x06); 
	lcd_write_register(0x5A,0x06); 
	lcd_write_register(0x5B,0x0C); 
	lcd_write_register(0x5C,0x1D); 
	lcd_write_register(0x5D,0xCC); 
	lcd_write_register(0x1B,0x1B);
	lcd_write_register(0x1A,0x01);
	lcd_write_register(0x24,0x2F);
	lcd_write_register(0x25,0x57);
	lcd_write_register(0x23,0x88);
	lcd_write_register(0x18,0x34);
	lcd_write_register(0x19,0x01);
	lcd_write_register(0x01,0x00);
	lcd_write_register(0x1F,0x88);
	lcd_write_register(0x1F,0x80);
	lcd_write_register(0x1F,0x90);
	lcd_write_register(0x1F,0xD0);
	lcd_write_register(0x17,0x05); 
	lcd_write_register(0x36,0x02); 
	lcd_write_register(0x28,0x38);
	lcd_write_register(0x28,0x3F);
	lcd_write_register(0x16,0x18); 
	lcd_write_register(0x02,0x00);
	lcd_write_register(0x03,0x00);
	lcd_write_register(0x04,0x00);
	lcd_write_register(0x05,0xEF);
	
	lcd_write_register(0x06,0x00);
	lcd_write_register(0x07,0x00);
	lcd_write_register(0x08,0x01);
	lcd_write_register(0x09,0x3F);
	
	lcd_clear_screen(WHITE);
}
void lcd_clear_screen(uint16_t hwColor)
{
	uint32_t i, wCount = LCD_WIDTH;
	wCount *= LCD_HEIGHT;
	
	lcd_set_cursor(0, 0);
	lcd_write_byte(0x22, 0);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	for (i = 0; i < wCount; i ++) {
		spi1_communication(hwColor >> 8);
		spi1_communication(hwColor & 0xFF);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}

void lcd_set_cursor(uint16_t hwXpos, uint16_t hwYpos)
{
	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
  lcd_write_register(0x02, hwXpos >> 8);
	lcd_write_register(0x03, hwXpos & 0xFF); 
	lcd_write_register(0x06, hwYpos >> 8);
	lcd_write_register(0x07, hwYpos & 0xFF); 
}


void lcd_display_char(	 uint16_t hwXpos, 
                         uint16_t hwYpos, 
                         uint8_t chChr,   
                         uint8_t chSize,  
                         uint16_t hwColor)
{      	
	uint8_t i, j, chTemp;
	uint16_t hwYpos0 = hwYpos, hwColorVal = 0;

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}

    for (i = 0; i < chSize; i ++) {
				if (FONT_1206 == chSize) {
					chTemp = c_chFont1206[chChr - 0x20][i];  
				}
				else if (FONT_1608 == chSize) { 
					chTemp = c_chFont1608[chChr - 0x20][i];
				}			
        for (j = 0; j < 8; j ++) {
					if (chTemp & 0x80) {
						hwColorVal = hwColor;
						lcd_draw_dot(hwXpos, hwYpos, hwColorVal);
					}			
					chTemp <<= 1;
					hwYpos ++;
					if ((hwYpos - hwYpos0) == chSize) {
						hwYpos = hwYpos0;
						hwXpos ++;
						break;
					}
				}  	 
    } 
}

void lcd_display_string(	uint16_t hwXpos,uint16_t hwYpos,
													const uint8_t *pchString,
													uint8_t chSize,uint16_t hwColor)
{

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
    while (*pchString != '\0') {       
        if (hwXpos > (LCD_WIDTH - chSize / 2)) {
					hwXpos = 0;
					hwYpos += chSize;
					if (hwYpos > (LCD_HEIGHT - chSize)) {
						hwYpos = hwXpos = 0;
						lcd_clear_screen(0x00);
					}
				}
		
        lcd_display_char(hwXpos, hwYpos, (uint8_t)*pchString, chSize, hwColor);
        hwXpos += chSize / 2;
        pchString ++;
    } 
}


void lcd_display_GB2312(  uint8_t gb, uint16_t color_front, 
													uint16_t postion_x,uint16_t postion_y )
{
	uint8_t i, j,chTemp;
	uint16_t hwYpos0 = postion_y, hwColorVal = 0;
	
	if (postion_x >= LCD_WIDTH || postion_y >= LCD_HEIGHT) {
		return;
	}
	
	for (i = 0; i < 32; i++) {
		chTemp = GB2312[gb][i];
		for (j = 0; j < 8; j++) {
			if (chTemp & 0x80) {
					hwColorVal = color_front;
				if(i<15)
					lcd_draw_dot(postion_x, postion_y, hwColorVal);
				else
					lcd_draw_dot(postion_x-16, postion_y+8, hwColorVal);
			}
			chTemp <<= 1;
			postion_y ++;
			if ((postion_y - hwYpos0) == 8) {
				postion_y = hwYpos0;
				postion_x ++;
				break;
			}
		}
	}
}


void lcd_draw_dot(uint16_t hwXpos, uint16_t hwYpos, uint16_t hwColor)
{
	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	lcd_set_cursor(hwXpos, hwYpos);
	lcd_write_byte(0x22, 0);
	lcd_write_word(hwColor);
}


void lcd_draw_bigdot(uint32_t color_front,
                     uint32_t x,uint32_t y )
{
    lcd_draw_dot(color_front,x,y);
    lcd_draw_dot(color_front,x,y+1);
    lcd_draw_dot(color_front,x,y-1);

    lcd_draw_dot(color_front,x+1,y);
    lcd_draw_dot(color_front,x+1,y+1);
    lcd_draw_dot(color_front,x+1,y-1);
    
    lcd_draw_dot(color_front,x-1,y);    
    lcd_draw_dot(color_front,x-1,y+1);
    lcd_draw_dot(color_front,x-1,y-1);
    
}

static uint32_t _pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	
	while(n --) result *= m;    
	return result;
}


void lcd_display_num(			uint16_t hwXpos,  uint16_t hwYpos, 
                          uint32_t chNum,  uint8_t chLen,   
                          uint8_t chSize,  uint16_t hwColor)
{         	
	uint8_t i;
	uint8_t chTemp, chShow = 0;

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
	for(i = 0; i < chLen; i ++) {
		chTemp = (chNum / _pow(10, chLen - i - 1)) % 10;
		if(chShow == 0 && i < (chLen - 1)) {
			if(chTemp == 0) {
				lcd_display_char(hwXpos + (chSize / 2) * i, hwYpos, ' ', chSize, hwColor);
				continue;
			} else {
				chShow = 1;
			}	 
		}
	 	lcd_display_char(hwXpos + (chSize / 2) * i, hwYpos, chTemp + '0', chSize, hwColor); 
	}
} 


void lcd_draw_line(		uint16_t hwXpos0, uint16_t hwYpos0, 
                      uint16_t hwXpos1, uint16_t hwYpos1, 
                      uint16_t hwColor) 
{
	int x = hwXpos1 - hwXpos0;
	int y = hwYpos1 - hwYpos0;
	int dx = abs(x), sx = hwXpos0 < hwXpos1 ? 1 : -1;
	int dy = -abs(y), sy = hwYpos0 < hwYpos1 ? 1 : -1;
	int err = dx + dy, e2;

	if (hwXpos0 >= LCD_WIDTH || hwYpos0 >= LCD_HEIGHT || hwXpos1 >= LCD_WIDTH || hwYpos1 >= LCD_HEIGHT) {
		return;
	}
    
    for (;;){
        lcd_draw_dot(hwXpos0, hwYpos0 , hwColor);
        e2 = 2 * err;
        if (e2 >= dy) {     
            if (hwXpos0 == hwXpos1) break;
            err += dy; hwXpos0 += sx;
        }
        if (e2 <= dx) {
            if (hwYpos0 == hwYpos1) break;
            err += dx; hwYpos0 += sy;
        }
    }
}


void lcd_draw_circle(		uint16_t hwXpos, uint16_t hwYpos, 
                        uint16_t hwRadius,uint16_t hwColor) 
{
	int x = -hwRadius, y = 0, err = 2 - 2 * hwRadius, e2;

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
    do {
        lcd_draw_dot(hwXpos - x, hwYpos + y, hwColor);
        lcd_draw_dot(hwXpos + x, hwYpos + y, hwColor);
        lcd_draw_dot(hwXpos + x, hwYpos - y, hwColor);
        lcd_draw_dot(hwXpos - x, hwYpos - y, hwColor);
        e2 = err;
        if (e2 <= y) {
            err += ++ y * 2 + 1;
            if(-x == y && e2 <= x) e2 = 0;
        }
        if(e2 > x) err += ++ x * 2 + 1;
    } while(x <= 0);
}


void lcd_fill_rect(uint16_t hwXpos, 
                   uint16_t hwYpos, uint16_t hwWidth,
                   uint16_t hwHeight,uint16_t hwColor)
{
	uint16_t i, j;

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
	for(i = 0; i < hwHeight; i ++){
		for(j = 0; j < hwWidth; j ++){
			lcd_draw_dot(hwXpos + j, hwYpos + i, hwColor);
		}
	}
}


void lcd_draw_v_line(		uint16_t hwXpos,uint16_t hwYpos, 
                        uint16_t hwHeight,uint16_t hwColor)  
{	
	uint16_t i, y1 = MIN(hwYpos + hwHeight, LCD_HEIGHT - 1);

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
    for (i = hwYpos; i < y1; i ++) {
        lcd_draw_dot(hwXpos, i, hwColor);
    }
}


void lcd_draw_h_line(		uint16_t hwXpos, uint16_t hwYpos,  
                        uint16_t hwWidth,uint16_t hwColor)
{	
	uint16_t i, x1 = MIN(hwXpos + hwWidth, LCD_WIDTH - 1);

	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}
	
    for (i = hwXpos; i < x1; i ++) {
        lcd_draw_dot(i, hwYpos, hwColor);
    }
}


void lcd_draw_rect(		uint16_t hwXpos,  
                      uint16_t hwYpos,uint16_t hwWidth, 
                      uint16_t hwHeight,uint16_t hwColor) 
{
	if (hwXpos >= LCD_WIDTH || hwYpos >= LCD_HEIGHT) {
		return;
	}

	lcd_draw_h_line(hwXpos, hwYpos, hwWidth, hwColor);
	lcd_draw_h_line(hwXpos, hwYpos + hwHeight, hwWidth, hwColor);
	lcd_draw_v_line(hwXpos, hwYpos, hwHeight, hwColor);
	lcd_draw_v_line(hwXpos + hwWidth, hwYpos, hwHeight + 1, hwColor);
}


void lcd_clear_Rect(	uint32_t color_front, 
											uint32_t hwXpos,uint32_t hwYpos, 
											uint32_t hwXpos1,uint32_t hwYpos1)
{
	uint16_t i, j;

	if (hwXpos1 >= LCD_WIDTH || hwYpos1 >= LCD_HEIGHT) {
		return;
	}
	
	for(i = 0; i < hwYpos1-hwYpos+1; i ++){
		for(j = 0; j < hwXpos1-hwXpos+1; j ++){
			lcd_draw_dot(hwXpos + j, hwYpos + i, color_front);
		}
	}
}