/**
  fade.pde - Fadeing example for PWMMatrix library.
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
#include <PWMMatrix.h>


PWMMatrix<true, 3, HIGH, 5, LOW, PinMapper<9, 10, 11, 4, 5, 6, 7, 8> > myMatrix;


void setup()
{
	myMatrix.begin();
	myMatrix.clear();
}

static uint8_t b = 0;
static uint8_t c = 0;

void loop()
{
	b++;
	c++;

	for (int i = 0; i < 5; i++) {
		myMatrix.setBrightness(i, 0, ((i + c + 1) & 1) * b);
		myMatrix.setBrightness(i, 1, (((i + c +1) >> 1) & 1) * b);
		myMatrix.setBrightness(i, 2, (((i + c + 1) >> 2) & 1) * b);
	}

	myMatrix.updateWithDelay(200);
}

