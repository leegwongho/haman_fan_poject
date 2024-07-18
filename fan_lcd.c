/*
 * fan_lcd.c
 *
 * Created: 2024-07-17 오전 11:06:44
 *  Author: jsmok
 */ 

// #include <avr/io.h>

#include "fan_lcd.h"

#define DEBOUNCE_DELAY 50

volatile uint8_t buttonData0 = 0;
volatile uint8_t buttonData1 = 0;
volatile uint8_t buttonData2 = 0;
volatile uint8_t prevButtonData0 = 0;
volatile uint8_t prevButtonData1 = 0;
volatile uint8_t prevButtonData2 = 0;
volatile uint8_t t_btn0 = 0;
volatile uint8_t t_btn1 = 0;
volatile uint8_t t_btn2 = 0;
char state1[4];
char state2[4];
char state3[6];


void LCD_Data(uint8_t data)
{
	LCD_DATA_PORT = data;	// 데이터 핀에 8비트 출력
	
}


void LCD_WritePin()
{
	LCD_RW_PORT &= ~(1<<LCD_RW);	// RW핀을 LOW로 설정하여 쓰기모드 진입
	
}


void LCD_EnablePin()
{
	LCD_E_PORT &= ~(1<<LCD_E);	// E핀을 LOW로 설정
	LCD_E_PORT |= (1<<LCD_E);	// E핀을 HIGH로 설정해서 동작 신호를 전송
	LCD_E_PORT &= ~(1<<LCD_E);	// E핀을 LOW로 설정
	_delay_us(2000);			// 1600 이상
}


void LCD_WriteCommand(uint8_t commandData)
{
	LCD_RS_PORT &= ~(1<<LCD_RS);	//RS핀을 LOW로 설정해서 명령어 모드로 진입
	LCD_WritePin();					// 데이터 쓰기모드 진입
	LCD_Data(commandData);			// 명령어를 데이터핀에 출력
	LCD_EnablePin();				// LCD 동작 신호 전송
}


void LCD_WriteData(uint8_t charData)
{
	LCD_RS_PORT |= (1<<LCD_RS);		// RS핀을 HIGH로 설정해서 문자 모드로 진입
	LCD_WritePin();					// 데이터 쓰기모드 진입
	LCD_Data(charData);				// 명령어를 데이터핀에 출력
	LCD_EnablePin();				// LCD 동작 신호 전송
}


void LCD_GotoXY(uint8_t row, uint8_t col) //row:가로, col:세로
{
	col %= 16;	// col을 16으로 제한
	row %= 2;	// row를 2로 제한
	uint8_t address = (0x40 * row) + col;		// 주소 계산
	uint8_t command = 0x80 + address;			// 커맨드 값 계산(주소설정)
	LCD_WriteCommand(command);					// 주소 커맨드를 전송
}


void LCD_WriteString(char *string)
{
	for (uint8_t i=0; string[i]; i++)
	{
		LCD_WriteData(string[i]);	// 문자열을 출력
	}
}


void LCD_WriteStringXY(uint8_t row, uint8_t col, char *string)
{
	LCD_GotoXY(row, col);
	LCD_WriteString(string);
}


void LCD_Init()
{
	LCD_DATA_DDR = 0xff;
	LCD_RS_DDR |= (1<<LCD_RS);
	LCD_RW_DDR |= (1<<LCD_RW);
	LCD_E_DDR |= (1<<LCD_E);
	
	_delay_ms(20);		// 초기화 시간 대기 (충분한 시간)
	LCD_WriteCommand(COMMAND_8BIT_MODE);		// 8bit 모드 설정
	_delay_ms(5);
	LCD_WriteCommand(COMMAND_8BIT_MODE);		// 8bit 모드 설정
	_delay_ms(2);
	LCD_WriteCommand(COMMAND_8BIT_MODE);		// 8bit 모드 설정
	LCD_WriteCommand(COMMAND_8BIT_MODE);		// 8bit 모드 설정
	LCD_WriteCommand(COMMAND_DISPLAY_OFF);
	LCD_WriteCommand(COMMAND_DISPLAY_CLEAR);
	LCD_WriteCommand(COMMAND_DISPLAY_ON);
	LCD_WriteCommand(COMMAND_ENTRY_MODE);

	// _delay_ms
}

void updateLCD(const char *state1, const char *state2, const char *state3)
{
	char buff[30];
	LCD_WriteStringXY(1, 0, state1);
	LCD_WriteStringXY(1, 4, state2);
	LCD_WriteStringXY(1, 8, state3);
	
	sprintf(buff, "%s %-5s %-11s", state1, state2, state3);
	LCD_WriteStringXY(1, 0, buff);
}

/*
ISR(INT0_vect) {
	_delay_ms(10);
	if (!(PIND & (1 << 0))) {
		if (buttonData0 && !prevButtonData0) {
			_delay_ms(DEBOUNCE_DELAY);
			if (buttonData0 == (PIND & (1<<0))) {
				t_btn0 = !t_btn0;
				state1 = t_btn0 ? "OFF" : "ON ";
				updateLCD(state1, state2, state3);
			}
		}
		prevButtonData0 = buttonData0;
	}
}

ISR(INT1_vect) {
	_delay_ms(10);
	if (!(PIND & (1 << 1))) {
		if (buttonData1 && !prevButtonData1) {
			_delay_ms(DEBOUNCE_DELAY);
			if (buttonData1 == (PIND & (1<<1))) {
				t_btn1 = (t_btn1 % 3) + 1;
				switch (t_btn1) {
					case 1: state2 = " 1"; break;
					case 2: state2 = " 2"; break;
					case 3: state2 = " 3"; break;
				}
				updateLCD(state1, state2, state3);
			}
		}
		prevButtonData1 = buttonData1;
	}
}

ISR(INT2_vect) {
	_delay_ms(10);
	if (!(PIND & (1 << 2))) {
		if (buttonData2 && !prevButtonData2) {
			//_delay_ms(DEBOUNCE_DELAY);
			if (buttonData2 == (PIND & (1<<2))) {
				t_btn2 = (t_btn2 % 3);
				t_btn2 = !t_btn2;
				state3 = t_btn2 ? "OFF" : "ON";
				updateLCD(state1, state2, state3);
			}
		}
		prevButtonData2 = buttonData2;
	}
}
*/