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
#include <util/delay.h>
#include <avr/interrupt.h>

/*
   Global variable used in interrupt
   level - compare match level
   updown - steps -1 down 1 up
 */
volatile uint8_t level;
volatile int updown;
	
ISR(TIMER0_COMP_vect) {
	level += updown;
	OCR0 = level;
}

int main (void) {
	uint8_t steps;

	steps = 10;
	/* start from top */
	level = 0xff;
	/* down by 1 step at a time */
	updown = -steps;

	/* must output the OC0 port PB3 on Mega16*/
	PORTB = 0;
	DDRB = _BV(PB3);

	/* compare match to the top, max slow speed */
	OCR0 = 0xff;

	/* Generate an Interrupt on a compare match */
	TIMSK = _BV(OCIE0);

	/* CTC mode with toggle OC0 on compare match */
	/* prescale to 1024 */
	/* start */
	TCCR0 = _BV(WGM01) | _BV(COM00) | _BV(CS02) | _BV(CS00);

	sei(); /* test if it's needed to be enabled */

	for (;;) {
		if (level < 50)
			updown = steps; /* go up */

		if (level > 220)
			updown = -steps; /* go down */

	/*
	_delay_ms(50);
*/
	}

	cli();

	return (0);
}

