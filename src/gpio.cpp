#include "wss.h"

void gpio_begin(void) {
	pinMode(PINLED0, OUTPUT);
	pinMode(PINLED1, OUTPUT);
	digitalWrite(PINLED0, HIGH);
	digitalWrite(PINLED1, HIGH);
}
