//
//
//
#include "SerialBUS.h"


void SPIMaster::begin()
{
	super::begin();

	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);
	
	SPCR |= 1 << MSTR;
}


void SPIMaster::set_divider(int div)
{
	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (div & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((div >> 2) & SPI_2XCLOCK_MASK);
}



uint8_t SPIMaster::recv()
{
	send(0xff);
	return wait_for_complete();
}

void SPIMaster::wait_and_send(uint8_t value)
{
	wait_for_complete();
	send(value);
}


uint8_t SPIMaster::wait_and_recv()
{
	wait_for_complete();
	return recv();
}

uint8_t SPIMaster::wait_for_complete()
{
	while (is_running())
 		;
 	SPSR |= 1 << SPIF;
 	return SPDR;
}


void SPIMaster::dummy_cycle(uint8_t n, uint8_t x)
{
	if (n == 0) return ;
	send(x);
	while (--n) wait_and_send(x);
}

void SPIMaster::drop_until(uint8_t x)
{
	do {
		send(0xff);
	} while (wait_for_complete() != x) ;
}

void SPIMaster::drop_until_with_timeout(uint8_t timeout, uint8_t x)
{
	while (timeout-- && recv() != x) ;
}

uint8_t SPIMaster::drop_while(uint8_t x)
{
	uint8_t r;
	do { r = recv(); } while (r == x);
	return r;
}

uint8_t SPIMaster::drop_while_with_timeout(uint8_t timeout, uint8_t x)
{
	uint8_t r;
	while (timeout--) {
		r = recv();
		if (r != x) break;
	}
	return r;
}
