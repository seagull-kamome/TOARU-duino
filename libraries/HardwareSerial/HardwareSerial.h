/*
  HardwareSerial.h - Hardware UART library for TOARU-duino
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
	  HardwareSerial.h - Hardware serial library for Wiring
	  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

	  Modified 28 September 2010 by Mark Sproul
 */
#if !defined(HardwareSerial_H)
#  define HardwareSerial_H

#  include <cores/arduino/WProgram.h>
#  include <cores/arduino/Print.h>
#  include <cores/arduino/RingBuffer.h>


class Uart : public Print
{
public:
	enum { TX_PIN = 0, RX_PIN = 1, RX_BUFFER_SIZE=128 };


	static __inline__ bool calcBaud(unsigned long baud, uint16_t* baud_setting)
	{
		bool use_u2x = true;
#  if F_CPU == 16000000L
        if (baud == 57600) use_u2x = false;
#  endif
		
		if (use_u2x)
			*baud_setting = (F_CPU / 4 / baud - 1) / 2;
		else
			*baud_setting = (F_CPU / 8 / baud - 1) / 2;
		return use_u2x;

	}

	uint8_t available()
	{
		return (RX_BUFFER_SIZE + rx_buf.w_ - rx_buf.r_) % RX_BUFFER_SIZE;
	}

	void flush() { rx_buf.clear(); }

	int read()
	{
		if (rx_buf.isEmpty()) return -1;
		return (int)rx_buf.pull();
	}

protected:
	RingBuffer<RX_BUFFER_SIZE> rx_buf;
};


extern "C" void USART_RX_vect();

class Uart0 : public Uart
{
	friend void USART_RX_vect();

public:
	void __inline__ begin(unsigned long baud)
	{
		uint16_t baud_setting;
		UCSR0A = ((calcBaud(baud, &baud_setting)?1:0) << U2X0);
		UBRR0H = baud_setting >> 8;
		UBRR0L = baud_setting;
		UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	}

	void __inline__ end()
	{
		UCSR0B &= ~((1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0));
	}

	virtual void write(uint8_t x)
	{
		while (!(UCSR0A & (1 << UDRE0))) ;
		UDR0 = x;
	}
};

extern Uart0 Serial;

#endif

