/**
 * Código de teste dos sensores 
 * 
 * Desenvolvimento atual por Kalidsa Buzzatti de Oliveira
 *
 
 *
 */
 
#include <atmel_start.h>
#include <stdio.h>
#include <IO1_drivers.h> // Inclui os arquivos de função dos sensores da placa IO1X Plained

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	IO_SENSOR_INIT();
	
	char message[15]; // Mensagem a ser enviada pela serial para o terminal
	
	while (1) {
				
			//Liga LED da placa de expansão
			IO1X_LED_ON();
			
			 // Leitura e envio da iluminância calculada
			float voltageSensor = readVoltageSensor();
			char voltage_str[20];
			floatToString(voltageSensor, voltage_str, 4);
			sprintf(message, "Tensão do sensor: %s V\r\n",voltage_str);
			printf(message);
			
			float currentSensor = readCurrentSensor(voltageSensor);
			char current_str[20];
			floatToString(currentSensor, current_str, 8);
			sprintf(message, "Corrente no sensor: %s âmpere\r\n", current_str);
			printf(message);
			
			float light_sensor = readLightSensor(currentSensor); // Iluminância medida pelo sensor de luz
			delay_ms(100);
			char iluminance_str[20];
			floatToString(light_sensor, iluminance_str, 4);
			
			sprintf(message, "Iluminancia: %s lux\r\n\n", iluminance_str);
			printf(message);
			
			// Leitura e envio da temperatura
			uint16_t temperature = readTemperatureSensor(); // Temperatura medida pelo sensor
			sprintf(message, "Temperatura: %d C\r\n", temperature);
			printf(message);
		
			
			
	
	}
}




