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
#define CONFIG_MEASURE 100000
#define MINUTO 60000000
uint16_t tempAmbiente = 0;
uint16_t frecResp = 0;
/*==================[internal data definition]===============================*/
TaskHandle_t registrarTension_task_handle = NULL;
/*==================[internal functions declaration]=========================*/
void CalibrarTempAmbiente(){
	medir(&tempAmbiente);
}

void FuncTimerA(void* param)
{
	vTaskNotifyGiveFromISR(registrarTension_task_handle, pdFALSE);
}

void medirFrecResp(uint16_t cuentas){
	uint16_t duracion = cuentas * CONFIG_MEASURE;
	frecResp = MINUTO/duracion;
	UartSendString(UART_PC, ",");
	UartSendString(UART_PC, " \r\n");
	UartSendString(UART_PC, (const char*)UartItoa(frecResp,10));
	
}

static void registrarTensionTask(void *pvParameter){

	uint16_t tempActual = 0;
	uint16_t cuentas = 0;
	uint16_t tempAnterior = 0;

	while(true)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		tempAnterior = tempActual;
		medir(&tempActual);
		cuentas++;
		//UartSendString(UART_PC, (const char*)UartItoa(tempActual,10));
		//UartSendString(UART_PC, ",");
		//UartSendString(UART_PC, (const char*)UartItoa(tempAnterior,10));
		//UartSendString(UART_PC, ",");
		//UartSendString(UART_PC, (const char*)UartItoa(tempAmbiente,10));
		//UartSendString(UART_PC, " \r\n");
	
		//Esto es provisorio para ver si vemos lo que deberíamos 
		//UartSendString(UART_PC, (const char*)UartItoa(tension_temperatura,10));
		//UartSendString(UART_PC, " \r\n");
		
		if(tempAnterior < tempAmbiente && tempActual >= tempAmbiente){
			medirFrecResp(cuentas);
			cuentas = 0;
		}
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
	xTaskCreate(&registrarTensionTask, "Medir tension", 512, NULL, 5, &registrarTension_task_handle);
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