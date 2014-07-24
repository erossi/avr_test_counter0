/*
   Copyright (C) 2009 Enrico Rossi

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Enrico Rossi <e.rossi@tecnobrain.com>
 */

#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
   Global variable used in interrupt
 */
volatile int step_counter;
volatile uint8_t flags;
	
ISR(TIMER0_COMP_vect) {
	step_counter++;
	flags = 1;
}


int main (void) {
	uint8_t level, bottom, top;

	top = 100;
	bottom = 30;
	step_counter = 0;
	flags = 0;

	/* must output the OC0 port PB3 on Mega16*/
	PORTB = 0;
	DDRB = _BV(PB3);

	PORTA = 0;
	DDRA = 0xff;

	/* compare match to the top, max slow speed */
	OCR0 = top;

	/* Generate an Interrupt on a compare match */
	TIMSK = _BV(OCIE0);

	/* CTC mode with toggle OC0 on compare match */
	/* prescale to 64 */
	/* start */

	sei();

	for (;;) {
		step_counter = 0;
		flags = 0;
		PORTA = 0;
		/*
		_delay_ms(100);
		*/
		TCCR0 = _BV(WGM01) | _BV(COM00) | _BV(CS01) | _BV(CS00);

		for (level = top; level >= bottom; level--) {
		       	while (!flags);
			OCR0 = level;
			flags = 0;
		}

		while (step_counter < 329)
			_delay_us(100);

		flags = 0;

		for (level = bottom; level <= top; level++) {
		       	while (!flags);
			OCR0 = level;
			flags = 0;
		}

		TCCR0 = 0;
		_delay_ms(10);
		PORTA = 1;
		_delay_ms(1000);
	}

	cli();

	return (0);
}

