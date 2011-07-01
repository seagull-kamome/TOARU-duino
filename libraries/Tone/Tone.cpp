/* Tone.cpp
*/


#include <avr/interrupt.h>
#include <cores/arduino/WProgram.h>

#include <Tone.h>

static uint8_t current_pin_ = 255;
static unsigned long toggle_count_;


void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
	if (current_pin_ == 255 && pin < 20) {
		TIMSK2 &= ~(1 << OCIE2A);	// Avoid race condition

		pinMode(pin, OUTPUT);
		current_pin_ = pin;
		toggle_count_ = 2 * frequency * duration / 1000;

		if ((uint16_t)(F_CPU / 2 / 1 / 256) < frequency) {
			TCCR2B = 0x01;
			OCR2A = F_CPU / 2 / frequency - 1;
		} else if ((uint16_t)(F_CPU / 2 / 8 / 256) < frequency) {
			TCCR2B = 0x02;
			OCR2A = F_CPU / 2 / 8 /frequency - 1;
		} else if ((uint16_t)(F_CPU / 2 / 32 / 256) < frequency) {
			TCCR2B = 0x03;
			OCR2A = F_CPU / 2 / 32 / frequency - 1;
		} else if ((uint16_t)(F_CPU / 2 / 64 / 256) < frequency) {
			TCCR2B = 0x04;
			OCR2A = F_CPU / 2 / 64 / frequency - 1;
		} else if ((uint16_t)(F_CPU / 2 / 128 / 256) < frequency) {
			TCCR2B = 0x05;
			OCR2A = F_CPU / 2 / 128 / frequency - 1;
		} else if ((uint16_t)(F_CPU / 2 / 256 / 256) < frequency) {
			TCCR2B = 0x06;
			OCR2A = F_CPU / 2 / 256 / frequency - 1;
		} else if ((uint16_t)(F_CPU / 2 / 1024 / 256) < frequency) {
			TCCR2B = 0x07;
			OCR2A = F_CPU / 2 / 1024 / frequency - 1;
		}
		TCCR2A = 1 << WGM21;
//		TCCR2B |= 1 << CS20;
		TIMSK2 |= 1 << OCIE2A;
	}
}


void noTone(uint8_t pin)
{
	if (pin == current_pin_) {
		TIMSK2 &= ~(1 << OCIE2A);
		current_pin_ = 255;
		digitalWrite(pin, 0);
	}
}


ISR(TIMER2_COMPA_vect)
{
	pinno2pinreg(current_pin_) |= 1 << pinno2bit(current_pin_);

	if (toggle_count_ != 0) {
	    if (--toggle_count_ == 0) {
	    	TIMSK2 &= ~(1 << OCIE2A);
	    	digitalWrite(current_pin_, LOW);
	    }
	}
}


