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
 * @authors Antonella Battauz Baron, Manuela Calvo
 * 
 *
 * @section changelog
 *
 * |   Date	    | Description                                    						|
 * |:----------:|:----------------------------------------------------------------------|
 * | 04/10/2024 | Document creation		                         						|
 * 
 **/
/*/==================[inclusions]=============================================/
/==================[macros]=================================================/
/==================[typedef]================================================/
/==================[external data declaration]==============================/
/==================[external functions declaration]=========================*/
void TermistorInit();
uint16_t medir();
/*/==================[end of file]============================================*/
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
#endif