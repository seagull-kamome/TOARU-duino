/*
  WProgram.cpp - Wiring compatibility library for TOARU-duino
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
#ifndef WPROGRAM_H
#  define WPROGRAM_H

#  define clockCyclesPerMicrosecond()		( F_CPU / 1000000L )
#  define clockCyclesToMicroseconds(x)		( (x) / clockCyclesPerMicrosecond() )
#  define microsecondsToClockCycles(x)	( (x) * clockCyclesPerMicrosecond() )

#  define PI 3.1415926535897932384626433832795
#  define HALF_PI 1.5707963267948966192313216916398
#  define TWO_PI 6.283185307179586476925286766559
#  define DEG_TO_RAD 0.017453292519943295769236907684886
#  define RAD_TO_DEG 57.295779513082320876798154814105

enum { SERIAL = 0, DISPLAY=1 };
enum { LSBFIRST = 0, MSBFIRST = 1 };
enum { CHANGE = 1, FALLING = 2, RISING = 3 };


template <typename T> static inline T min(T const& a, T const& b) { return a < b? a : b; }
template <typename T> static inline T max(T const& a, T const& b) { return a > b? a : b; }
template <typename T> static inline T abs(T const& a) { return a > 0? a : -a; }
template <typename T> static inline T constraint(T const& x, T const& l, T const& h)
{ return (x < l)? l : (x > h)? h : x; }
static inline double round(double const& x) { return (x >= 0)? (long)(x + 0.5) : (long)(x - 0.5); }
static inline float round(float const& x) { return (x >= 0)? (long)(x + 0.5) : (long)(x - 0.5); }
template <typename T> static inline T radians(T const& deg) { return deg * DEG_TO_RAD; }
template <typename T> static inline T degrees(T const& rad) { return rad * RAD_TO_DEG; }
template <typename T> static inline T sq(T const& x) { return x * x; }

#  include <avr/interrupt.h>
static inline void interrupts() { sei(); }
static inline void nointerrupts() { cli(); }

void __inline__ setup();
void __inline__ loop();

unsigned long millis();
unsigned long micros();
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);




#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
const static uint8_t A0 = 54;
const static uint8_t A1 = 55;
const static uint8_t A2 = 56;
const static uint8_t A3 = 57;
const static uint8_t A4 = 58;
const static uint8_t A5 = 59;
const static uint8_t A6 = 60;
const static uint8_t A7 = 61;
const static uint8_t A8 = 62;
const static uint8_t A9 = 63;
const static uint8_t A10 = 64;
const static uint8_t A11 = 65;
const static uint8_t A12 = 66;
const static uint8_t A13 = 67;
const static uint8_t A14 = 68;
const static uint8_t A15 = 69;
#else
const static uint8_t A0 = 14;
const static uint8_t A1 = 15;
const static uint8_t A2 = 16;
const static uint8_t A3 = 17;
const static uint8_t A4 = 18;
const static uint8_t A5 = 19;
const static uint8_t A6 = 20;
const static uint8_t A7 = 21;
#endif


#  include <stdlib.h>
void __inline__ randomSeed(unsigned int seed) { ::srandom(seed); }
long __inline__ random(long x) { return x == 0? 0 : (random() % x); }
long __inline__ random(long x, long y)
{
	if (x >= y) return x;
	return random(y - x) + x;
}

template <typename T>
T __inline__ map(T x, T n, T m, T nn, T mm)
{
	return (x - n) * (mm - nn) / (m - n + nn);
}

#  include <cores/arduino/digital.h>
#  include <cores/arduino/analog.h>
#  include <cores/arduino/UART.h>

#endif

