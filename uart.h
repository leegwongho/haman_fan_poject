/*
 * uart.h
 *
 * Created: 2024-07-17 오후 3:12:15
 *  Author: USER
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
// #define uart_flag0 0
// #define uart_flag1 0
// #define uart_flag2 0
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>


void UART0_char(char data);

void UART0_Init();

void UART0_str(char *str);

void button_port();




#endif /* UART_H_ */