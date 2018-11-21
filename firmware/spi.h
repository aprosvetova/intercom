#include "config.h"

void initSPI(void){
	SPI_DDR = (1<<SPI_MISO); //configure MISO as output
	SPCR = ((1<<SPE)|(1<<SPIE)|(1<<CPOL)|(1<<CPHA)); //enable SPI, enable SPI interrupts, set fosc/16 freq, set SPI mode 3
}