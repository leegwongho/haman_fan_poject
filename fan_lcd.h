/*
 * fan_lcd.h
 *
 * Created: 2024-07-17 오전 11:07:21
 *  Author: jsmok
 */ 


#ifndef FAN_LCD_H_
#define FAN_LCD_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


#define LCD_DATA_DDR		DDRF
#define LCD_DATA_PORT		PORTF
#define LCD_DATA_PIN		PINF
#define LCD_RS_DDR			DDRE
#define LCD_RW_DDR			DDRE
#define LCD_E_DDR			DDRE
#define LCD_RS_PORT			PORTE
#define LCD_RW_PORT			PORTE
#define LCD_E_PORT			PORTE
#define LCD_RS				7	//6
#define LCD_RW				6	//7
#define LCD_E				5	//5

#define COMMAND_DISPLAY_CLEAR		0x01	// Clear all display
#define COMMAND_DISPLAY_ON			0x0C	// 화면 ON, 커서 OFF, 커서 점멸 OFF
#define COMMAND_DISPLAY_OFF			0x08	// 화면 OFF, 커서 OFF, 커서 점멸 OFF
#define COMMAND_ENTRY_MODE			0x06	// 커서 우측 이동, 화면이동 없음
#define COMMAND_8BIT_MODE			0x38	// 8비트, 화면 2행, 5X8 Font
#define COMMAND_4BIT_MODE			0x28	// 4비트, 화면 2행, 5X8 Font

extern char state1[4];
extern char state2[4];
extern char state3[6];

extern volatile uint8_t t_btn0;
extern volatile uint8_t t_btn1;
extern volatile uint8_t t_btn2;
extern volatile uint8_t prevButtonData0;
extern volatile uint8_t prevButtonData1;
extern volatile uint8_t prevButtonData2;

void LCD_Data(uint8_t data);
void LCD_WritePin();
void LCD_EnablePin();
void LCD_WriteCommand(uint8_t commandData);
void LCD_WriteData(uint8_t charData);
void LCD_GotoXY(uint8_t row, uint8_t col);
void LCD_WriteString(char *string);
void LCD_WriteStringXY(uint8_t row, uint8_t col, char *string);
void LCD_Init();
void updateLCD(const char *state1, const char *state2, const char *state3);

#endif /* FAN_LCD_H_ */