/**



*/
#include <PWMMatrix.h>


PWMMatrix<5, LOW, 3, HIGH, 4, 5, 6, 7, 8, 9, 10, 11> myMatrix;


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
		myMatrix.setBrightness(0, i, ((i + c + 1) & 1) * b);
		myMatrix.setBrightness(1, i, (((i + c +1) >> 1) & 1) * b);
		myMatrix.setBrightness(2, i, (((i + c + 1) >> 2) & 1) * b);
	}

	myMatrix.update();
	delay(10);
}

