/*! @mainpage Proyecto_Final
 *
 * @section genDesc General Description
 * El programa permite levantar la curva respiratoria con un sensor de temperatura y medir la frecuencia respiratoria 
 * a partir de la misma; la frecuencia respiratoria se muestra por la UART.
 *
 * @section hardConn Hardware Connection
 *
 * |     ESP32      |     PERIFERICO    |
 * |:--------------:|:------------------|
 * | 	 CH1	 	|    Sensor temp    |
 * | 	 GND	 	|  GND Sensor temp  |
 * 
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 18/10/2024 | Document creation		                         |
 *
 * @authors Antonella Battauz Baron (antobattauzbaron.abb@gmail.com), Manuela Calvo (manuela.calvo@ingenieria.uner.ar)
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
#include "switch.h"

/*==================[macros and definitions]=================================*/
/**
 * @def CONFIG_MEASURE
 * @brief Periodo del temporizador en microsegundos para notificar la tarea que registra la tensión.
 */
#define CONFIG_MEASURE 100000
/**
 * @def FACTOR_CONVERSION
 * @brief Factor para calcular la frecuencia respiratoria que viene dado por 60000000/100000, donde 
 * 60000000 es 1 minuto en microsegundos y 100000 es CONFIG_MESURE.
 */
#define FACTOR_CONVERSION 600

/**
 * @brief Variable donde se almacena el valor de tensión de la temperatura ambiente calibrada.
 */
uint16_t tempAmbiente = 0;

/**
 * @brief Variable donde se almacena el valor de frecuencia respiratoria.
 */
uint16_t frecResp = 0;

/*==================[internal data definition]===============================*/
/**
 * @brief Handle para la tarea que registra la tensión del sensor de temperatura.
 */
TaskHandle_t registrarTension_task_handle = NULL;

/*==================[internal functions declaration]=========================*/
/**
 * @fn void CalibrarTempAmbiente()
 * 
 * @brief Función que calibra la temperatura ambiente.
 */
void CalibrarTempAmbiente(){
	medir(&tempAmbiente);
}

/**
 * @fn void FuncTimerA(void* param)
 * 
 * @brief Función del temporizador que notifica a la tarea `registrarTensionTask` para su ejecución.
 * 
 * @param param Parámetro opcional que se puede pasar a la función. No se utiliza en esta implementación.
 */
void FuncTimerA(void* param)
{
	vTaskNotifyGiveFromISR(registrarTension_task_handle, pdFALSE);
}

/**
 * @fn void medirFrecResp(uint16_t cuentas)
 * 
 * @brief Función que calcula la frecuencia respiratoria y la informa por la UART.
 * 
 * @param cuentas cantidad de ticks entre cada inspiración y espiración (entre cada cruce por la referencia).
 */
void medirFrecResp(uint16_t cuentas){

	frecResp = FACTOR_CONVERSION/cuentas;	

	UartSendString(UART_PC, "Frecuencia respiratoria: ");
	UartSendString(UART_PC, (const char*)UartItoa(frecResp,10));
	UartSendString(UART_PC, "\r\n");
}
/**
 * @fn static void registrarTensionTask(void* pvParameter)
 * 
 * @brief Función que levanta la curva respiratoria.
 * 
 * @param pvParameter Parámetro opcional que se puede pasar a la función. No se utiliza en esta implementación.
 */
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
		//UartSendString(UART_PC, (const char*)UartItoa(tempAmbiente,10));
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

	timer_config_t timerA = {
        .timer = TIMER_A,
        .period = CONFIG_MEASURE,
        .func_p = FuncTimerA,
        .param_p = NULL
    };

	TimerInit(&timerA);
	xTaskCreate(&registrarTensionTask, "Medir tension", 512, NULL, 5, &registrarTension_task_handle);
	TimerStart(timerA.timer);

	SwitchActivInt(SWITCH_1, &CalibrarTempAmbiente, NULL);

	serial_config_t puertoSerie = {
		.port = UART_PC,
		.baud_rate = 115200,
		.func_p = NULL,
		.param_p = NULL
	};
	UartInit(&puertoSerie);

}
/*==================[end of file]============================================*/