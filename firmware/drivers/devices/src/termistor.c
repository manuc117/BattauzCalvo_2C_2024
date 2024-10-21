/**
 * @file termistor.c
 * @authors Antonella Battauz Baron y Manuela Calvo(manuela.calvo@ingenieria.uner.edu.ar)
 * @brief 
 * @version 0.1
 * @date 2024-10-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*/==================[inclusions]=============================================*/
#include "termistor.h"
#include "analog_io_mcu.h"
#include "timer_mcu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void TermistorInit(){

    analog_input_config_t termistorInput = {			
	    .input = CH1,			
	    .mode =	ADC_SINGLE,	
	    .func_p =  NULL, 			
	    .param_p = NULL,		
	    .sample_frec = NULL
    }; 	

    AnalogInputInit(&termistorInput);
}

void medir(uint16_t *tension)
{
    AnalogInputReadSingle(CH1, &tension);
}