//Cabeçalhos necessários para inicialização de drivers, configuração de clock, funções utilitárias e 
//inicialização de hardware específico do microcontrolador.
#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>
#include <hpl_adc_base.h>

// Define parâmetros para configuração do ADC, como quantidade de canais e tamanho do buffer.
#define IO_SENSOR_ADC_CH_AMOUNT 1
#define IO_SENSOR_ADC_BUFFER_SIZE 16
#define IO_SENSOR_ADC_CH_MAX 0

// Declaração de estruturas para os controladores ADC, USART e I2C.
struct adc_async_descriptor IO_SENSOR_ADC;
struct adc_async_channel_descriptor IO_SENSOR_ADC_ch[IO_SENSOR_ADC_CH_AMOUNT];
static uint8_t IO_SENSOR_ADC_buffer[IO_SENSOR_ADC_BUFFER_SIZE];
static uint8_t IO_SENSOR_ADC_map[IO_SENSOR_ADC_CH_MAX + 1];
struct usart_sync_descriptor TARGETIO;
struct i2c_m_sync_desc I2C_INSTANCE;

//  Inicializa o controlador ADC e configura o pino PA06 para uso com o ADC
void IO_SENSOR_ADC_INIT(void) {
	_pm_enable_bus_clock(PM_BUS_APBC, ADC);
	_gclk_enable_channel(ADC_GCLK_ID, CONF_GCLK_ADC_SRC);
	adc_async_init(&IO_SENSOR_ADC, ADC, IO_SENSOR_ADC_map, IO_SENSOR_ADC_CH_MAX,
	IO_SENSOR_ADC_CH_AMOUNT, &IO_SENSOR_ADC_ch[0], (void *)NULL);
	adc_async_register_channel_buffer(&IO_SENSOR_ADC, 0, IO_SENSOR_ADC_buffer,
	IO_SENSOR_ADC_BUFFER_SIZE);
	gpio_set_pin_direction(PA06, GPIO_DIRECTION_OFF);
	gpio_set_pin_function(PA06, PINMUX_PA06B_ADC_AIN6);
}

//  Inicializa o controlador USART (configura os pinos PA04 e PA05 para uso com USART)
void TARGETIO_PORT_INIT(void) {
	gpio_set_pin_function(PA04, PINMUX_PA04D_SERCOM0_PAD0);
	gpio_set_pin_function(PA05, PINMUX_PA05D_SERCOM0_PAD1);
}

//é responsável por inicializar o clock para o módulo SERCOM0, que geralmente é usado para comunicação serial no microcontrolador
void TARGETIO_CLOCK_INIT(void) {
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
}

// é responsável por inicializar o módulo de comunicação USART (Universal Synchronous and Asynchronous Receiver-Transmitter) usando o SERCOM0
void TARGETIO_INIT(void) {
	TARGETIO_CLOCK_INIT();
	usart_sync_init(&TARGETIO, SERCOM0, (void *)NULL);
	TARGETIO_PORT_INIT();
}

// Inicializa o controlador I2C (configura os pinos PA16 e PA17 para uso com I2C)
void I2C_INSTANCE_PORT_INIT(void) {
	gpio_set_pin_pull_mode(PA16, GPIO_PULL_OFF);
	gpio_set_pin_function(PA16, PINMUX_PA16C_SERCOM1_PAD0);
	gpio_set_pin_pull_mode(PA17, GPIO_PULL_OFF);
	gpio_set_pin_function(PA17, PINMUX_PA17C_SERCOM1_PAD1);
}

//Essas funções estão relacionadas à inicialização e configuração de um 
//controlador de comunicação I2C (Inter-Integrated Circuit) usando o módulo SERCOM1
void I2C_INSTANCE_CLOCK_INIT(void) {
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM1);
	_gclk_enable_channel(SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC);
	_gclk_enable_channel(SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC);
}


void I2C_INSTANCE_INIT(void) {
	I2C_INSTANCE_CLOCK_INIT();
	i2c_m_sync_init(&I2C_INSTANCE, SERCOM1);
	I2C_INSTANCE_PORT_INIT();
}

//  Inicializa o driver de atraso usando SysTick
void delay_driver_init(void) {
	delay_init(SysTick);
}

void system_init(void) {
	init_mcu(); // Inicializa o microcontrolador
	//Configura o pino do LED e o coloca em nível baixo.
	gpio_set_pin_level(LED, false);
	gpio_set_pin_direction(LED, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(LED, GPIO_PIN_FUNCTION_OFF);
	//Chama as funções de inicialização dos controladores
	IO_SENSOR_ADC_INIT();
	TARGETIO_INIT();
	I2C_INSTANCE_INIT();
	delay_driver_init();
}