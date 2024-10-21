/*! @mainpage Proyecto_final
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 21/10/2024 | Document creation		                         |
 *
 * @authors Antonella Battauz Baron (antobattauzbaron.abb@gmail.com) y Manuela Calvo (manuela.calvo@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "termistor.h"
#include "timer_mcu.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_MESURE 769000
uint_16t tempAmbiente = 0;
/*==================[internal data definition]===============================*/
TaskHandle_t medirTension_task_handle = NULL;
/*==================[internal functions declaration]=========================*/
void FuncTimerA(void* param)
{
	vTaskNotifyGiveFromISR(medirTension_task_handle, pdFALSE);
}

void CalibrarTempAmbiente(){
	medir(&tempAmbiente);
}
/*==================[external functions definition]==========================*/
void app_main(void){
	TermistorInit();
	CalibrarTempAmbiente();
	
	timer_config_t timer = {
        .timer = TIMER_A,
        .period = CONFIG_MEASURE,
        .func_p = FuncTimerA,
        .param_p = NULL
    };

	TimerInit(&timer);

	xTaskCreate(&medirTensionTask, "Medir tension", 512, NULL, 5, &medirTension_task_handle);
	TimerStart(timer.timer);
}
/*==================[end of file]============================================*/