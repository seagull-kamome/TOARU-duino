/*
  RTCRead.pde - Hardware UART library for TOARU-duino
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
#include <RTC/RTC4543.h>

RTC4543<A1,  A0, SPIMaster> rtc;

void setup()
{
	SPIMaster::begin();
	rtc.begin();
	Serial.begin(9600);
}


void loop()
{
	DateTime_t tm;
	char buf[14];
Serial.println(Serial.available(), DEC);
	if (Serial.available()) {
		for (int i = 0; i < 14; i++) {
			while (! Serial.available()) ;
			buf[i] = Serial.read();
		}
		tm.parse(buf);
		rtc.write(tm);
	}

	rtc.read(&tm);
	tm.format(buf);
	Serial.println(buf);
			
	delay(1000);
}