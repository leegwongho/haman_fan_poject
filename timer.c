/*
 * timer.c
 *
 * Created: 2024-07-18 오후 5:04:57
 *  Author: USER
 */ 
#include "timer.h"
#include "motor.h"

volatile uint16_t timer;
volatile uint8_t timer_on;

void shut_down(){
	if (motor_on) {
		timer_on = !timer_on;
		if (timer_on)
		PORTC |= (1 << PORTC7);
		else
		PORTC &= ~(1 << PORTC7);
		} else {
		timer_on = 0;
		PORTC &= ~(1 << PORTC7);
	}
}

// 외부 인터럽트 서비스 루틴
ISR(INT3_vect){
	// 디바운싱을 위해 50ms 대기
	_delay_ms(50);
	// 버튼이 여전히 눌려있는지 확인
	if (!(PIND & (1 << PIND3))){
		shut_down();
		// 버튼이 떼어질 때까지 대기
		while (!(PIND & (1 << PIND3)));
		// 추가 디바운싱을 위해 다시 50ms 대기
		_delay_ms(50);
	}
}