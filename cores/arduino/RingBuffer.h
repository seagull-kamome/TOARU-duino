/*
  RingBuffer.h - RingBuffer library for TOARU-duino
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
 #if !defined(ARDUINO__CORE__RINGBUFFER_H)
 #  define ARDUINO__CORE__RINGBUFFER_H
 
template <int N>
class RingBuffer
{
protected: 
 	uint8_t w_;
	uint8_t r_;
 	uint8_t xs_[N];

public: 
 	enum { BUFFER_SIZE = N };

 	RingBuffer() { clear(); }


 	void emit(uint8_t x)
 	{
		xs_[w_] = x;
 		w_ = (w_ + 1) % N;
 	}
 
	uint8_t available()
	{
		uint8_t oldsreg = SREG;
		nointerrupt();
		uint8_t x = (w_ > r_)? w_ -r_ : r_ - w_;
		SREG = oldsreg;
		return x;
	}


	uint8_t pull()
	{
		uint8_t x = xs_[r_];
		r_ = (r_ + 1) % N;
		return x;
	}

	uint8_t peek()
	{
		return xs_[r_];
	}

	void clear() { w_ = r_ = 0; }
	bool isEmpty() const { return r_ == w_; }
	bool isFull() const { return (_w + 1) % N == _r; }
 };
 
 
 #endif
