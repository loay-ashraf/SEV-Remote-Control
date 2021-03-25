/** @file micro_config.h
 *  @brief includes the MCU configuration parameters and required header files
 *  @author Hussam Mostafa
 *	@version 1.0.0
 *  @bug no known bugs
 */


#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_

//------------DEFINE CPU CLOCK FREQUENCY------------//

#ifndef F_CPU
#define F_CPU 12000000UL //12MHz Clock frequency
#endif

//------------INCLUDE ALL REQUIRED HEADER FILES------------//

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <util/delay.h>
#include "std_types.h"

#endif /* MICRO_CONFIG_H_ */