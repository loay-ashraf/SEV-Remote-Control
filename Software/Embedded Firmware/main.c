/** @mainpage About
 *  @author Loay Ashraf <loay.ashraf.96@gmail.com>
 *  @version 1.0.0
 *
 *  This program is implemented on ATmega16/32 running at 16 MHZ
 *	CPU clock, it basically receives commands from Android companion
 *	application via Bluetooth using the widely common HC-05 Bluetooth 
 *	Module, it acts upon the command received by -for example- turning 
 *	ON a relay which activates certain feature of the vehicle; means of
 *	feedback include battery voltage and speed of the motor. 
 *  
 *	@pinassignments
 *
 *	- PB0-7: DAC output signal
 *	- PC0-7, PD6-7: Relay output signal
 *	- PD0: USART RX signal
 *	- PD1: USART TX signal
 *
 *	@see main.c
 */

/** @file main.c
 *  @brief includes the main function
 *  
 *	the main function periodically checks for incoming commands
 *	via USART interface, if one is received it starts processing 
 *	the command and takes the action that corresponds to the 
 *	command received. 
 *
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include USART.h, string.h and set F_CPU in micro_config.h
 *  @bug no known bugs
 *  @see micro_config.h
 */

//------------INCLUDE DEVICE DRIVERS------------//

#include "ADC.h"
#include "USART.h"

//------------DECLARE GLOBAL VARIABLES------------//
/** @var char* received_ptr
 *  @brief Stores memory address of received command
 */

/** @var char* received_str
 *  @brief Stores data of received command
 */

/** @var char* res_str
 *  @brief Stores result of float conversion
 */

/** @var char* ON_flag
 *  @brief Stores ON status of the all features (0 for OFF, 1 for ON)
 */

/** @var float Volt
 *  @brief Current battery voltage value
 */

/** @var float Speed
 *  @brief Current motor speed value
 */

/** @var uint8 i
 *  @brief Counter that iterates through received command
 */

/** @var uint8 thumb_position
 *  @brief Stores current slider position in the android app
 */

char * received_ptr;
char received_str[10], res_str[7], ON_flag[7];
float Volt, Speed; 
uint8 i, thumb_position;

/** @brief the main function
 *
 *  see main.c description
 *
 *  @return exit status (1 or 0)
 */
int main(void){
	
	//------------INITIALIZE I/O PORTS------------//
	
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xC0;
	
	//------------INITIALIZE ADC INTERFACE------------//
	
	ADC_Init(AVCC, RIGHT_ADJ, 128);	// AVCC as Vref, right adjusted result (10-bit), 128 prescaler
	ADC_Read(ADC0);
	ADC_Read(ADC1);
	
	//------------INITIALIZE USART INTERFACE------------//
	
	USART_init(AS, NORMAL_SPEED, 8, NO_PARITY, 1);	// asynchronous, 8-bit data frame, no parity check, one stop bit
	USART_set_baudrate(38400);						// 38400 bits/sec baud rate
	USART_enable();									// enable USART interface

	while (1){
		
		//------------RECIEVE NEW COMMAND AND PROCESS IT INTO A STRING------------//
		
		received_ptr = USART_Receive_str();		// receive new command
		for(i=0;i<10;i++)
		received_str[i]=*received_ptr++;		// store received command data in array
		
		//------------CHECK RECEIVED COMMAND AND TAKE CORRESPONDING ACTION------------//
		
		if(!strcmp(received_str,"Voltage")){
			
			Volt = ((ADC_Read(ADC0)/1023.0)*60);
			dtostrf(Volt, 6, 2,res_str);
			USART_Transmit_str(res_str);

		}else if (!strcmp(received_str,"RPM")){
			
			Speed = ((ADC_Read(ADC1)/1023.0)*4900);
			dtostrf(Speed, 6, 2,res_str);
			USART_Transmit_str(res_str);
			
		}else if (!strcmp(received_str,"one")){
			
			// forward action
			PORTC |= 0x01;

		}else if (!strcmp(received_str,"two")){
			
			// reverse action
			PORTC |= 0x02;
			_delay_ms(1);
			PORTC |= 0x01;
	
		}else if (!strcmp(received_str,"three")){
			
			// brake action
			PORTC |= 0x04;
			_delay_ms(1);
			PORTC &= ~0x03;
			_delay_ms(10);
			PORTC &= ~0x04;

		}else if (!strcmp(received_str,"four")){
			
			// low headlights
			if(!ON_flag[0]){
			PORTC |= 0x08;
			ON_flag[0] = 1;
			}else{
			PORTC &= ~0x08;	
			ON_flag[0] = 0;	
			}

		}else if (!strcmp(received_str,"five")){
			
			// high headlights
			if(!ON_flag[1]){
			PORTC |= 0x10;
			ON_flag[1] = 1;
			}else{
			PORTC &= ~0x10;
			ON_flag[1] = 0;
			}

		}else if (!strcmp(received_str,"six")){
			
			// right signs
			if(!ON_flag[2]){
			PORTC |= 0x20;
			ON_flag[2] = 1;
			}else{
			PORTC &= ~0x20;
			ON_flag[2] = 0;
			}

		}else if (!strcmp(received_str,"seven")){
			
			// left signs
			if(!ON_flag[3]){
			PORTC |= 0x40;
			ON_flag[3] = 1;
			}else{
			PORTC &= ~0x40;
			ON_flag[3] = 0;
			}

		}else if (!strcmp(received_str,"eight")){
			
			// waiting signs
			if(!ON_flag[4]){
			PORTC |= 0x80;
			ON_flag[4] = 1;
			}else{
			PORTC &= ~0x80;
			ON_flag[4] = 0;
			}

		}else if (!strcmp(received_str,"nine")){
			
			// lower car roof
			if(!ON_flag[5]){
			PORTD |= 0x40;
			ON_flag[5] = 1;
			}else{
			PORTD &= ~0x40;
			ON_flag[5] = 0;
			}

		}else if (!strcmp(received_str,"ten")){
			
			// raise car roof
			if(!ON_flag[6]){
			PORTD |= 0x80;
			ON_flag[6] = 1;
			}else{
			PORTD &= ~0x80;
			ON_flag[6] = 0;
			}

		}else{
			
			// receive slider position in the app and set speed
			thumb_position = atoi(received_str);
			PORTB = (255-thumb_position);
			
		}}
	}