/*
  HardwareSerial.cpp - Hardware UART library for TOARU-duino
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
#include <HardwareSerial.h>

Uart0 Serial;

ISR(USART_RX_vect)
{
	Serial.rx_buf.emit(UDR0);
}

ISR(USART_UDRE_vect)
{
	if (Serial.tx_buf.isEmpty())
		UCSR0B &= ~(1 << UDRIE0);

	UDR = Serial.tx_buf.pull();
}


