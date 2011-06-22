/*
  wiring.cpp - Wiring compatibility library for TOARU-duino
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

  This library based on Arduino core library.
	  wiring.h - Partial implementation of the Wiring API for the ATmega8.
  	  Part of Arduino - http://www.arduino.cc/
	  Copyright (c) 2005-2006 David A. Mellis
 */
#include <cores/arduino/WProgram.h>

#include <util/delay.h>

#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

static unsigned long volatile timer0_overflow_count = 0;
static unsigned long volatile timer0_millis = 0;
static uint8_t volatile timer0_fract = 0;

SIGNAL(TIMER0_OVF_vect)
{
	unsigned long m = timer0_millis;
	uint8_t f = timer0_fract;
	
	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) { f -= FRACT_MAX; m += 1; }
	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}

unsigned long millis()
{
	uint8_t old_sreg = SREG;
	cli();
	unsigned long m = timer0_millis;
	SREG = old_sreg;
	return m;
}


unsigned long micros()
{
	uint8_t old_sreg = SREG;
	cli();
	unsigned long m = timer0_overflow_count;
	uint8_t t = TCNT0;
	
#ifdef TIFR0
	if ((TIFR0 & (1 << TOV0)) && t < 255) m++;
#else
	if ((TIFR & (1 << TOV0)) && t < 255) m++;
#endif
	SREG = old_sreg;
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}


void delay(unsigned long ms)
{
        uint16_t start = (uint16_t)micros();
        while (ms > 0) {
                if (((uint16_t)micros() - start) >= 1000) {
                        ms--;
                        start += 1000;
                }
        }
}

void delayMicroseconds(unsigned int us)
{
#if F_CPU >= 16000000L
        if (--us == 0)
                return;
        us <<= 2;
        us -= 2;
#else
        if (--us == 0)
                return;
        if (--us == 0)
                return;
        us <<= 1;
        us--;
#endif
        __asm__ __volatile__ (
                "1: sbiw %0,1" "\n\t" // 2 cycles
                "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
        );
}

