/*
  digital.cpp - Digital I/O library for TOARU-duino
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

void pinMode_noconst(uint8_t pin, uint8_t mode)
{
	switch (mode)
	{
	case OUTPUT:
		pinno2ddrreg(pin) |= 1 << pinno2bit(pin);
		break;
	case INPUT:
		pinno2ddrreg(pin) &= ~(1 << pinno2bit(pin));
		break;
	}
}



void turnOffPWM_noconst(uint8_t pin)
{

	switch (pin)
	{
	case 3:		// OC2B
		TCCR2A &= ~(1 << COM2B1);
		break;

	case 5:		// OC0B
		TCCR0A &= ~(1 << COM0B1);
		break;

	case 6:		// OC0A
		TCCR0A &= ~(1 << COM0A1);
		break;

	case 9:		// OC1A
		TCCR1A &= ~(1 << COM1A1);
		break;

	case 10:	// OC1B
		TCCR1A &= ~(1 << COM1B1);
		break;

	case 11:	// OC2A
		TCCR2A &= ~(1 << COM2A1);
		break;

	default:
		break;
	}
}

bool digitalRead_noconst(uint8_t pin)
{
	turnOffPWM_noconst(pin);
	return pinno2pinreg(pin) & (1 << pinno2bit(pin));
}



void digitalWrite_noconst(uint8_t pin, uint8_t val)
{
	turnOffPWM_noconst(pin);

	if (val)
		pinno2portreg(pin) |= 1 << pinno2bit(pin);
	else
		pinno2portreg(pin) &= ~(1 << pinno2bit(pin));
}


unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
	uint8_t volatile& pinreg = pinno2pinreg(pin);
	uint8_t msk = 1 << pinno2bit(pin);
	uint8_t stm = state? msk : 0;

	unsigned long w = 0;
	unsigned long n = 0;
	unsigned long m = microsecondsToClockCycles(timeout) / 16;

	while ((pinreg & msk) == stm)
		if (n++ == m) return 0;
	while ((pinreg & msk) != stm)
		if (n++ == m) return 0;
	while ((pinreg & msk) == stm)
		w++;

	return clockCyclesToMicroseconds(w * 10 + 16); 
}



void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	for (int i = 0; i < 8; i++) {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, (val & (1 << i)));
		else
			digitalWrite(dataPin, (val & (1 << (7 - i))));
		
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}

