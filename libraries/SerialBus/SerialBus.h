//
//
//
#ifndef SerialBUS_H
#  define SerialBUS_H

#  include <WProgram.h>


struct MSBFirst;
struct LSBFirst
{
public:
	enum { mask = 0x01 };
	static inline uint8_t shift(uint8_t x) { return x >> 1; }

	typedef MSBFirst Reverse;
};

struct MSBFirst
{
public:
	enum { mask = 0x80 };
	static inline uint8_t shift(uint8_t x) { return x << 1; }
	
	typedef LSBFirst Reverse;
};

	
struct InputFirst { public: enum { cpha = 0x00 }; };
struct OutputFirst { public: enum { cpha = 0x01 }; };





template <int clk, int in, int out>
class SerialBUS
{
protected:
	SerialBUS() {}
	~SerialBUS() {}

public:

	enum { pinCLK = clk, pinIn = in, pinOut = out };

	static inline void begin()
	{
		pinMode(in, INPUT);
		digitalWrite(in, HIGH);
		
		pinMode(out, OUTPUT);
		digitalWrite(out, LOW);

		pinMode(clk, OUTPUT);
		digitalWrite(pinCLK, LOW);
	}

	static inline void bitOut(uint8_t b)	{ digitalWrite(out, b != 0? HIGH : LOW); }
	static inline bool bitIn()				{ return digitalRead(in); }
};




template <
	typename BUS,
	uint8_t clkPol,			// アイドル時のクロック極性
	typename clkPhase,		// 書き出し側のクロック極性
	typename bitorder >
class SerialDevice
{
protected:
	inline SerialDevice()	{}
	inline ~SerialDevice()	{}


public:
	static inline void begin()		{}
	static inline void end()		{}

	static inline void pulse()
	{
		digitalWrite(BUS::pinCLK, clkPol == LOW? HIGH : LOW);
		digitalWrite(BUS::pinCLK, clkPol);
	}

	static void shift_out(uint8_t send, uint8_t bits)
	{
		while (bits--) {
			if (clkPhase::cpha != 0x00)
				BUS::bitOut(send & bitorder::mask);

			digitalWrite(BUS::pinClk, clkPol::inactive);

			if (clkPhase::cpha != clkPol::active)
				BUS::bitOut(send & bitorder::mask);
				
			digitalWrite(BUS::pinCLK, clkPol);
			send = bitorder::shift(send);
		}
	}

	static uint8_t shift_in(uint8_t bits)
	{
		uint8_t recv = 0x00;
		while (bits--) {
			recv = bitorder::shift(recv);

			if (clkPhase::cpha == 0x00 && BUS::bitIn())
				recv |= bitorder::Reverse::mask;

			digitalWrite(BUS::pinClk, clkPol == LOW? HIGH : LOW);

			if (clkPhase::cpha != clkPol::active && BUS::bitIn())
				recv |= bitorder::Reverse::mask;

			digitalWrite(BUS::pinCLK, clkPol);
		}

		return recv;
	}


	static uint8_t shift_io(uint8_t send, uint8_t bits)
	{
		uint8_t recv = 0x00;
		while (bits--) {
			recv = bitorder::shift(recv);

			if (clkPhase::cpha != 0x00)
				BUS::bitOut(send & bitorder::mask);
			else if (BUS::bitIn())
				recv |= bitorder::Reverse::mask;

			digitalWrite(BUS::pinClk, clkPol == LOW? HIGH : LOW);

			if (clkPhase::cpha != clkPol::active)
				BUS::bitOut(send & bitorder::mask);
			else if (BUS::bitIn())
				recv |= bitorder::Reverse::mask;
				
			digitalWrite(BUS::pinCLK, clkPol);
			send = bitorder::shift(send);
		}
		
		return recv;
	}

};





class SPIMaster : public SerialBUS<13, 12, 11>
{
protected:
	inline SPIMaster() {}
	inline ~SPIMaster() {}

	typedef SerialBUS<13, 12, 11> super;

	enum { SPI_CLOCK_MASK = 0x03, SPI_2XCLOCK_MASK = 0x01, SPI_MODE_MASK = 0x0C };
	enum { MODE0 = 0x00, MODE1 = 0x04, MODE2 = 0x08, MODE3 = 0x0C };

public:
	enum { pinClk = 13 };
	enum { DIV2 = 0x04, DIV4 = 0x00, DIV8 = 0x05, DIV16 = 0x02, DIV32 = 0x06, DIV64 = 0x07, DIV128 = 0x03 };

	static void begin()
	{
		super::begin();

		pinMode(10, OUTPUT);
		digitalWrite(10, HIGH);
	
		SPCR |= 1 << MSTR;
	}


	static inline void enable() { SPCR |= 1 << SPE; }
	static inline void disable() { SPCR &= ~(1 << SPE); }

	static void set_divider(int div)
	{
		SPCR = (SPCR & ~SPI_CLOCK_MASK) | (div & SPI_CLOCK_MASK);
		SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((div >> 2) & SPI_2XCLOCK_MASK);
	}
	

	template <typename clkPol, typename clkPhase>
	static void set_data_mode()
	{
		uint8_t datamode = (clkPol::active << CPOL) | (clkPhase::cpha << CPHA);
		SPCR = (SPCR & ~SPI_MODE_MASK) | datamode;
	}

	template <typename bitorder>
	static void set_bit_order()
	{
		if (bitorder::mask == 0x01)		SPCR |= 1 << DORD;
		else							SPCR &= ~(1 << DORD);
	}


	static void send(uint8_t value) { SPDR = value; }
	static uint8_t recv()			{ send(0xff); return wait_for_complete(); }

	static void wait_and_send(uint8_t value)	{ wait_for_complete(); send(value); }
	static uint8_t wait_and_recv()				{ wait_for_complete(); return recv(); }

	static bool is_running() { return (SPSR & (1 << SPIF)) == 0x00? true : false; }
	static uint8_t wait_for_complete()
	{
		while (is_running())
 			;
	 	SPSR |= 1 << SPIF;
	 	return SPDR;
	}
	
	static void dummy_cycle(uint8_t n, uint8_t x)
	{
		if (n == 0) return ;
		send(x);
		while (--n) wait_and_send(x);
	}

	static void drop_until(uint8_t x)
	{
		do { send(0xff); } while (wait_for_complete() != x) ;
	}

	static void drop_until_with_timeout(uint8_t timeout, uint8_t x) { while (timeout-- && recv() != x) ; }
	static uint8_t drop_while(uint8_t x)
	{
		uint8_t r;
		do { r = recv(); } while (r == x);
		return r;
	}

	static uint8_t drop_while_with_timeout(uint8_t timeout, uint8_t x)
	{
		uint8_t r;
		while (timeout--) {
			r = recv();
			if (r != x) break;
		}
		return r;
	}
};





template <
	typename clkPol,		// アイドル時のクロック極性
	typename clkPhase,		// 書き出し側のクロック極性
	typename bitorder,
	int clock_divider,
	uint8_t ss>
class SPIDevice
{
public:
	inline SPIDevice() {}
	inline ~SPIDevice() {}

	void begin()
	{
		pinMode(ss, OUTPUT);
		disable();
	}

	void enable()
	{
		SPIMaster::set_divider(clock_divider);
		SPIMaster::set_bit_order<bitorder>();
		SPIMaster::set_data_mode<clkPol, clkPhase>();
		SPIMaster::enable();
	}
	inline void disable() { SPIMaster::disable(); }

	inline void select() { digitalWrite(this->ss, LOW); }
	inline void deselect() { digitalWrite(this->ss, HIGH); }
};



#endif
