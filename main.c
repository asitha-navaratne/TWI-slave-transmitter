#define F_CPU 8000000UL

#include <avr/io.h>

void AWAIT_CALL(unsigned char address);
void SLAVE_SEND(char *data);

int main(void){
	AWAIT_CALL(0xA0);
	SLAVE_SEND("Hello World!");
}

/*!
 *	@brief Wait and listen for call from master device.
 *	@param Device's address (unsigned char).
 */

void AWAIT_CALL(unsigned char address){
	TWAR = address; 							///< Load device's address onto TWAR register.
	TWCR = (1<<TWEN)|(1<<TWEA);						///< Enable TWI and set TWINT bit to 1. Generate an ACK bit on the TWI bus if the address is received.
	while(!(TWCR & (1<<TWINT)));						///< Wait while TWI completes current action.
	while((TWSR & 0xF8) != 0xA8);						///< While address has not been received or ACK has not been sent.
}

/*!
 *	@brief Transmit data to master device.
 *	@param Data to be transmitted (char).
 */

void SLAVE_SEND(char *data){
	uint8_t i = 0;
	while((TWSR & 0xF8) != 0xC0){
		TWDR = data[i]; 						///< Load data to be transmitted to TWDR register.
		TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA);				///< Enable TWI and set TWINT bit to 1. Write 1 to TWEA to continue transmission.		
		while(!(TWCR & (1<<TWINT)));					///< Wait while TWI completes current action. 
		i++;
	}
}
