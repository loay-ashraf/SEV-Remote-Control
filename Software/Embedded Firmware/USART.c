/** @file USART.c
 *  @brief includes function definition
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include USART.h
 *  @bug no known bugs
 *  @see USART.h
 */

//------------INCLUDE DRIVER HEADER FILE------------//

 #include "USART.h"

//------------DECLARE LOCAL VARIABLES------------//

/** @var uint8 mode_flag
 *  @brief indicates the current mode (Asynchronous or Synchronous)
 */

/** @var uint8 u2x_flag
 *  @brief indicates the current speed (Normal speed or Double speed)
 */

/** @var uint8 ninth_bit_flag
 *  @brief indicates if there's a ninth bit in the data frame
 */

 static uint8 mode_flag = 0;
 static uint8 u2x_flag = 0;
 static uint8 ninth_bit_flag = 0;

 /** @brief initializes the USART interface
  *  @param umode communication mode (Asynchronous or Synchronous).
  *  @param u2x speed mode (Normal speed or Double speed).
  *	 @param ch_size number of bits in the data frame.
  *	 @param parity_bit parity check mode (Parity or No parity).
  *	 @param stop_bit number of stop bits (one bit or two bits).
  *  @return none.
  */
 void USART_init(uint8 umode, uint8 u2x, uint8 ch_size, uint8 parity_bit, uint8 stop_bit){
    
	mode_flag = umode; 
	u2x_flag = u2x;
    
	if(umode == AS)

	UCSRA |= (u2x<<U2X);							// Asynchronous mode

	else if (umode == S)

	UCSRC |= ((1<<URSEL) | (1<<UMSEL));				// Synchronous mode
	
	if(ch_size != 9 && ch_size >= 5){
	
	ninth_bit_flag = 0; 
	UCSRC |= ((1<<URSEL) | ((ch_size-5)<<UCSZ0));	// set number of bits in the data frame

	}else if (ch_size == 9){

	ninth_bit_flag = 1;
	// set number of bits to 9 in the data frame
	UCSRB |= (1<<UCSZ2);							
	UCSRC |= ((1<<URSEL) | (3<<UCSZ0));	

	}
    
	if(parity_bit > 0 && stop_bit > 0)
	// set parity mode and number of stop bits
	UCSRC |= ((1<<URSEL) | (parity_bit<<UPM0) | ((stop_bit-1)<<USBS));	

 }

/** @brief sets the baud rate for the USART interface
 *  @param baudrate set baud rate in bits/sec.
 *  @return none.
 */
 void USART_set_baudrate(uint16 baudrate){
   
   uint16 UB_Baud = 0;

   UCSRB &= ~((1<<RXEN) | (1<<TXEN));		// disable receiver and transmitter buffers

   if(mode_flag == AS){

   if (u2x_flag == 0)

   UB_Baud = ((F_CPU)/(16.0*baudrate))-1;	// calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet

   else if(u2x_flag == 1)

   UB_Baud = ((F_CPU)/(8.0*baudrate))-1;	// calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet

   }else if(mode_flag == S){
    
   UB_Baud = ((F_CPU)/(2.0*baudrate))-1;	// calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet
   }
   // set the baud rate
   UBRRH = (uint8) (UB_Baud >> 8);	
   UBRRL = (uint8) UB_Baud;	

 }
 
 /** @brief enables the USART interface
  *  @return none.
  */
 void USART_enable(void){

  UCSRB |= ((1<<TXEN) | (1<<RXEN));		// enable receiver and transmitter buffers

 }

 /** @brief transmits 8-bit data 
  *	 @param data 8-bit data to be transmitted.
  *  @return none.
  */
 void USART_Transmit_chr(char data){

	while(!(UCSRA & (1<<UDRE)));	// wait for empty transmit buffer
	
	if(!ninth_bit_flag){

	  UDR = data;					// start transmittion

	}else{
	  
	  UCSRB &= ~(1<<TXB8);			// clear ninth data bit

	  if(data & 0x0100)

	  UCSRB |= (1<<TXB8);			// set ninth data bit if it's set in the data
	  
	  UDR = data;					// start transmittion

	  }
 }

 /** @brief transmits a string of 8-bit data 
  *	 @param data pointer to the fist element of the string.
  *  @return none.
  */
 void USART_Transmit_str(const char * data){

   while(*data != '\0'){
      
	  USART_Transmit_chr(*data++);	// iterate through the array till the NULL terminator

    }
 }

 /** @brief receives 8-bit data 
  *  @return received 8-bit data.
  */
 char USART_Receive_chr(void){
    
	while(!(UCSRA & (1<<RXC)));		// Wait for incoming data
   
	if (!ninth_bit_flag)
	{
	  return UDR;					// return UDR register if there's no ninth bit

	}else{

	uint8 resh,resl;

	resh = UCSRB;
	resl = UDR;

	resh = (resh >> 1) & 0x01;		// process the ninth bit

	return ((resh << 8) | resl);	// return the combined 9-bit data
	} 
 }
 
 /** @brief receives a string of 8-bit data 
  *  @return pointer to the fist element of the received string.
  */
 char * USART_Receive_str(void){
	 
	 static char input[10];
	 uint8 tmp, i = 0;
	 
	 while ((tmp=USART_Receive_chr()) != '#'){
		input[i++] = tmp;	// iterate through the array till the hash character is received
		if(i==9)
			break;			// break loop if buffer size limit is reached
	 }
	 input[i] = '\0';		// terminate the array with NULL terminator
	 return input;			// return pointer to the first element of the array
 }
 
 
 
 