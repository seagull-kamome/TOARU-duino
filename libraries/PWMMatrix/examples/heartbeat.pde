/*** Flip Animation
 * by arms22 <http://arms22.blog91.fc2.com/>
 * 
 * Demonstrates the use of the Dots library
 *
 * 2011/06/25 HATTORI,Hiroki Modified for TOARU-duino
 */

#include <PWMMatrix.h>
#include <avr/pgmspace.h>

PWMMatrix<false, 8, HIGH, 8, LOW, PinMapper<6, A1, A0, 3, 12, 4, 8, 9, 2, 7, 10, 5, A3, 11, A2, 13> > myMatrix;

const byte PROGMEM one[] = {
  B00000000,
  B01100110,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
};

const byte PROGMEM two[] = {
  B00000000,
  B00000000,
  B00100100,
  B01111110,
  B01111110,
  B00111100,
  B00011000,
  B00000000,
};

void setup()
{
	myMatrix.begin();
	myMatrix.clear();
}

uint8_t r = 0;
uint8_t c = 0;

void loop()
{
	myMatrix.write_P(0, one, 8);
	myMatrix.updateWithDelay(750);
 
	myMatrix.write_P(0, two, 8);
	myMatrix.updateWithDelay(250);
}

