#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pins.h"
#include "spi.h"
#include "const.h"

unsigned char command = 0;
long lastCall = 0;
bool shouldOpenDoor = false;
bool shouldHangUp = false;

void init(void) {
	initPins(); //configure pins
	setWaitMode(true); //enable wait mode
	initSPI(); //configure SPI
	sei(); //allow global interrupts
}

ISR(SPI_STC_vect) {
	switch (SPDR) {
		case CALLING_ADDRESS:
			SPDR = isCalling() ? CALLING_RESPONSE : NOT_CALLING_RESPONSE;
			break;
		case HOOK_MODE_ADDRESS:
			SPDR = isHooked() ? HOOKED_RESPONSE : NOT_HOOKED_RESPONSE;
			break;
		case OPEN_DOOR:
			shouldOpenDoor = true;
			SPDR = ACK;
			break;
		case HANG_UP:
			shouldHangUp = true;
			SPDR = ACK;
			break;
		case HOOK:
			setHookMode(true);
			SPDR = ACK;
			break;
		case UNHOOK:
			setHookMode(false);
			SPDR = ACK;
			break;
		case ACK:
			SPDR = ACK;
			break;
		default:
			SPDR = WTF;
			break;
	}
};

void pickupPhone(void) {
	setOpen(false);
	_delay_ms(20);
	setWaitMode(false);
}

void putPhone(void) {
	setWaitMode(true);
	setOpen(true);
}

void acceptCall(bool open) {
	if (!isCalling()) {
		return;
	}
	pickupPhone();
	_delay_ms(100);
	bool wasHooked = isHooked();
	if (!wasHooked) {
		setHookMode(true);
		_delay_ms(10);
	}
	if (open) {
		setOpen(true);
	}
	_delay_ms(1000);
	if (!wasHooked) {
		setHookMode(false);
	}
	putPhone();	
}

int main(void)
{
	init();
	while(1) {
		if(shouldOpenDoor) {
			acceptCall(true);
			shouldOpenDoor = false;
			continue;
		}
		if(shouldHangUp) {
			acceptCall(false);
			shouldHangUp = false;
			continue;
		}
	}
}