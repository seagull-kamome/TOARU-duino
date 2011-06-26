/*
  counter.pde - Counter example for PWMMatrix library.
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

PWMMatrix<false, 8, HIGH, 5, LOW, PinMapper<6, A1, A0, 3, 12, 4, 8, 9, 2, 7, 10, 5, A3> > myMatrix;

void setup()
{
	myMatrix.begin();
	myMatrix.clear();
}


int count = 0;

void loop()
{
	myMatrix.writeHexDigit(3, count & 0x0f );
	myMatrix.writeHexDigit(2, (count >> 4) & 0x0f);
	myMatrix.writeHexDigit(1, (count >> 8) & 0x0f);
	myMatrix.writeHexDigit(0, (count >> 12) & 0x0f);
	count++;
	myMatrix.updateWithDelay(100);
}

