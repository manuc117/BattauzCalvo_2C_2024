#ifndef TERMISTOR_H
#define TERMISTOR_H

/** \addtogroup Drivers_Programable Drivers Programable
 ** @{ */
/** \addtogroup Drivers_Devices Drivers devices
 ** @{ */
/** \addtogroup Termistor
 ** @{ */

/** \brief Termistor driver for the ESP-EDU Board.
 * 
 * @note 
 *
 * @authors Antonella Battauz Baron y Manuela Calvo
 * 
 *
 * @section changelog
 *
 * |   Date	    | Description                                    						|
 * |:----------:|:----------------------------------------------------------------------|
 * | 04/10/2024 | Document creation		                         						|
 * 
 **/
/*/==================[inclusions]=============================================*/
#include "analog_io_mcu.h"
#include <stdbool.h>
#include <stdint.h>
/*==================[macros]=================================================/
/==================[typedef]================================================/
/==================[external data declaration]==============================/
/==================[external functions declaration]=========================*/
void TermistorInit();
void medir(uint16_t *tension);
/*/==================[end of file]============================================*/
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif