/*
  analog.cpp - Analog I/O library for TOARU-duino
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
#include <cores/arduino/analog.h>
#include <cores/arduino/digital.h>

uint16_t analogRead(uint8_t pin)
{
	analogSetChannel(pin);
	analogStartConversion();
	analogWaitForConversion();
	return analogResult();
}




void analogWrite_noconst(uint8_t pin, uint8_t val)
{
	pinMode_const(pin, OUTPUT);

	switch (pin)
	{
	case 3:		// OC2B
		TCCR2A |= 1 << COM2B1;
		OCR2B = val;
		break;

	case 5:		// OC0B
		TCCR0A |= 1 << COM0B1;
		OCR0B = val;
		break;

	case 6:		// OC0A
		if (val == 0)
			digitalWrite(pin, LOW);
		else {
			TCCR0A |= 1 << COM0A1;
			OCR0A = val;
		}
		break;

	case 9:		// OC1A
		TCCR1A |= 1 << COM1A1;
		OCR1A = val;
		break;

	case 10:	// OC1B
		TCCR1A |= 1 << COM1B1;
		OCR1B = val;
		break;
		
	case 11:	// OC2A
		TCCR2A |= 1 << COM2A1;
		OCR2A = val;
		break;

	default:
		digitalWrite(pin, val<128? LOW : HIGH);
		break;
	}
}
