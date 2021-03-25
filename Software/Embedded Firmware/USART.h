/** @file USART.h
 *  @brief This driver supports interfacing any device that
 *	supports serial communication via USART interface; baud 
 *	rate, data frame, parity check and number of stop bits
 *	can be set according to each system
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @bug no known bugs
 *  @see micro_config.h
 */

#ifndef USART_H_
#define USART_H_

//------------INCLUDE MCU CONFIGURATION------------//

#include "micro_config.h"

//------------DEFINE CONFIGURATION PARAMETERS ABBREVIATIONS------------//

#define AS 0
#define S 1
#define NORMAL_SPEED 0
#define DOUBLE_SPEED 1
#define NO_PARITY 0
#define EVEN_PARITY 2
#define ODD_PARITY 3

//------------FUNCTION PROTOTYPES------------//

void USART_init(uint8 umode, uint8 u2x, uint8 ch_size, uint8 parity_bit, uint8 stop_bit);
void USART_set_baudrate(uint16 baudrate);
void USART_enable(void);
void USART_Transmit_chr(char data);
void USART_Transmit_str(const char * data);
char USART_Receive_chr(void);
char * USART_Receive_str(void);

#endif /* USART_H_ */