/*
  pde_main.inc - Main module of PDE for TOARU-duino
  Copyright (c) 2011 HATTORI, Hiroki.    All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  
 */
#include <cores/arduino/WProgram.h>
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int main(void) __attribute__ ((noreturn));
int main(void)
{
    sei();

#if !defined(__AVR_ATmega8__)
    sbi(TCCR0A, WGM00);
    sbi(TCCR0A, WGM01);
#endif  

#if defined(__AVR_ATmega128__)
    sbi(TCCR0, CS02);
#elif defined(TCCR0) && defined(CS01) && defined(CS00)
    TCCR0 |= 0x03 << CS01;
#elif defined(TCCR0B) && defined(CS01) && defined(CS00)
	TCCR0B |= 0x03 << CS00;
#elif defined(TCCR0A) && defined(CS01) && defined(CS00)
	TCCR0A |= 0x03 << CS00;
#else
        #error Timer 0 prescale factor 64 not set correctly
#endif

    // enable timer 0 overflow interrupt
#if defined(__AVR_ATmega8__)
	sbi(TIMSK, TOIE0);
#else
	sbi(TIMSK0, TOIE0);
#endif

    // set timer 1 prescale factor to 64
#if defined(TCCR1B) && defined(CS11) && defined(CS10)
	TCCR1B = 0x03 << CS10;
#elif defined(TCCR1) && defined(CS11) && defined(CS10)
	TCCR1B = 0x00;
	TCCR1 |= 0x03 << CS10;
#endif
    // put timer 1 in 8-bit phase correct pwm mode
    sbi(TCCR1A, WGM10);


        // set timer 2 prescale factor to 64
#if defined(TCCR2) && defined(CS22)
        sbi(TCCR2, CS22);
#elif defined(TCCR2B) && defined(CS22)
        sbi(TCCR2B, CS22);
#else
        #warning Timer 2 not finished (may not be present on this CPU)
#endif

    // configure timer 2 for phase correct pwm (8-bit)
#if defined(__AVR_ATmega8__)
	sbi(TCCR2, WGM20);
#else
	sbi(TCCR2A, WGM20);
#endif

#if defined(__AVR_ATmega1280__)
    // set timer 3, 4, 5 prescale factor to 64
    TCCR3B |= 0x03 << CS30;
    TCCR4B |= 0x03 << CS40;
    TCCR5B |= 0x03 << CS50;
    // put timer 3, 4, 5 in 8-bit phase correct pwm mode
    sbi(TCCR3A, WGM30);
    sbi(TCCR4A, WGM40);
    sbi(TCCR5A, WGM50);
#endif

    // set a2d prescale factor to 128
    // 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
    // XXX: this will not work properly for other clock speeds, and
    // this code should use F_CPU to determine the prescale factor.
	ADCSRA |= (7 << ADPS0) | (1 << ADEN);

    // the bootloader connects pins 0 and 1 to the USART; disconnect them
    // here so they can be used as normal digital i/o; they will be
    // reconnected in Serial.begin()
#if defined(__AVR_ATmega8__)
    UCSRB = 0;
#else
    UCSR0B = 0;
#endif

 	setup();
 	for (;;) loop();

 	return 0;
 }
