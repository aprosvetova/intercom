#include "config.h"
#include <stdbool.h>

bool hooked = false;

void initPins(void) {
	DDR = (1<<HOOK_MODE_PIN) | (1<<WAIT_MODE_PIN) | (1<<OPEN_PIN); //enable output mode on corresponding pins
	PORT = (1<<INCOMING_CALL_PIN); //enable pull-up on input pin
}

void setPin(bool enabled, int pin) {
	if (enabled) {
		PORT |= (1 << pin);
		} else {
		PORT &= ~(1 << pin);
	}
}

bool isCalling(void) {
	return !(PIN & (1<<INCOMING_CALL_PIN)); //true for low because pin has pull-up resistor
}

bool isHooked(void) {
	return hooked;
}

void setHookMode(bool enabled) {
	hooked = enabled;
	setPin(enabled, HOOK_MODE_PIN);
}

void setWaitMode(bool enabled) {
	setPin(enabled, WAIT_MODE_PIN);
}

void setOpen(bool enabled) {
	setPin(!enabled, OPEN_PIN);
}