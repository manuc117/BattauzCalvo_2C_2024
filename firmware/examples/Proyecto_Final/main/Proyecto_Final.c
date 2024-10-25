/*! @mainpage Proyecto_Final
 *
 * @section genDesc General Description
 *
 * 
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 18/10/2024 | Document creation		                         |
 *
 * @author Antonella Battauz Baron (antobattauzbaron.abb@gmail.com), Manuela Calvo (manuela.calvo@ingenieria.uner.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "termistor.h"
#include "timer_mcu.h"
#include "uart_mcu.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_MEASURE 769000
uint16_t tempAmbiente = 0;
/*==================[internal data definition]===============================*/
TaskHandle_t medirTension_task_handle = NULL;
/*==================[internal functions declaration]=========================*/
void CalibrarTempAmbiente(){
	medir(&tempAmbiente);
}

void FuncTimerA(void* param)
{
	vTaskNotifyGiveFromISR(medirTension_task_handle, pdFALSE);
}

static void medirTensionTask(void *pvParameter){

	uint16_t tension_temperatura = 0;

	while(true)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		medir(&tension_temperatura);
		//Esto es provisorio para ver si vemos lo que deberíamos 
		UartSendString(UART_PC, (const char*)UartItoa(voltaje,10));
		UartSendString(UART_PC, " \r\n");
	}

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
	//Esto es provisorio a ver si funciona correctamente
	serial_config_t puertoSerie = {
		.port = UART_PC,
		.baud_rate = 115200,
		.func_p = NULL,
		.param_p = NULL
	};
	UartInit(&puertoSerie);
}
/*==================[end of file]============================================*/