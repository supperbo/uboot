  /*
 * uart.c
 *
 *  Created on: 06.10.2011
 *      Author: IBezhenar
 */

#include <avr/io.h>              // Most basic include files
#include <avr/interrupt.h>       // Add the necessary ones here
#include "uart.h"
#include <string.h>
#include <util/delay.h>
#include <stdio.h>


#define HEAD		3
#define CRC			2
#define MAX_DATA_SIZE 128

//volatile char received_packet_indicator;
volatile unsigned char 	buffer_index = 0;
volatile unsigned char 	received_packet_indicator = 0;
volatile unsigned char 	received_data_size = MAX_DATA_SIZE;
volatile unsigned char 	received_packet_number;


volatile unsigned char 	receive_buffer[HEAD+MAX_DATA_SIZE+CRC];

volatile union  {
		unsigned volatile  int checksum;
		unsigned volatile  char val[2];
} crc;


// USART Receiver interrupt service routine
ISR(USART_RXC_vect) {
//SIGNAL (SIG_UART_RECV) {
	char data;
	data=UDR;

	//If received_packet_indicator = '$' that means - we receiving the packet
	if (received_packet_indicator == '$') {
		//Received any byte
		if ( buffer_index == 2) {
			//Receive packet size
			receive_buffer[buffer_index] = data;
			received_data_size = data;
			buffer_index++;
		}
		else if ( buffer_index > HEAD + CRC  && buffer_index < received_data_size + HEAD + CRC ) {
			//Receive general byte
			receive_buffer[buffer_index] = data;
			crc.checksum = crc.checksum + data;
			buffer_index++;
		}
		else if ( buffer_index < HEAD + CRC && buffer_index != 2 ) {
			//Receive header except packet size
			receive_buffer[buffer_index] = data;
			buffer_index++;
		}
		else if ( buffer_index > received_data_size + HEAD + CRC ) {
			//Last byte of packet was received

			//check CRC, if OK send to UART number of received packet
			if ( crc.val[0] == receive_buffer[3] && crc.val[1] == receive_buffer[4] ) {
				//Succesfully received
				//Send to UART number of received packet


				/*
				if ( received_data_size != MAX_PACKET_SIZE) {
				LAST PACKET WERE RECEIVED
				RUN_MAIN_PROGRAM
				}
				*/

			}
			crc.checksum = 0;
			buffer_index = 0;
			received_packet_indicator = 0x00;

		}
	}//if (received_packet_indicator == '$') {

	if(data =='$' && buffer_index == 0) {
		//If we received "$" shows that packet started to be received
		received_packet_indicator = '$';
	}
}//ISR(USART_RXC_vect)





int main(void) {

	crc.checksum = 0;
	//UART Port speed 115200  for the crystal freq. 7.3MHz
	USART_init ();


	//TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode

	//TIMSK |= (1 << OCIE1A); // Enable CTC interrupt
	//TCCR1B |= ((1 << CS10) | (1 << CS11)); // Set up timer at Fcpu/64

	//OCR1A   = 7000; // Set CTC compare value
	//TCCR1B |= ((1 << CS10) | (1 << CS11)); // Start timer at Fcpu/64

	USART_transmit('c');// Change the directory on
	USART_transmit('d');//ARM to home. Cannot run
	USART_transmit(' ');///home/status STATUS__
	USART_transmit('/');//need to cd to /home
	USART_transmit('h');
	USART_transmit('o');
	USART_transmit('m');
	USART_transmit('e');
	USART_transmit(0x0A);

	USART_transmit('.');// Change the directory on
	USART_transmit('/');
	USART_transmit('a');//ARM to home. Cannot run
	USART_transmit('v');///home/status STATUS__
	USART_transmit('r');//need to cd to /home
	USART_transmit('c');
	USART_transmit('a');
	USART_transmit('t');
	USART_transmit(' ');
	USART_transmit('c');// Change the directory on
	USART_transmit('o');//ARM to home. Cannot run
	USART_transmit('n');///home/status STATUS__
	USART_transmit('s');//need to cd to /home
	USART_transmit('o');
	USART_transmit('l');
	USART_transmit('e');
	USART_transmit(' ');
	USART_transmit('0');
	USART_transmit(0x0A);

	while(1) {
	}

}


/*
ISR(TIMER1_COMPA_vect)
{
	//This enterrupt allows to skip
	//all the odd data on com port, so we can ignore it in the beginning
	//and start to receive only required data
	//received_packet_indicator = 'g';
	//puts("C\r");
	//USART_transmit('C');
	//USART_transmit(0x0A);

   //TIMSK &= ~(1 << OCIE1A); // Disable CTC interrupt
}
*/




















