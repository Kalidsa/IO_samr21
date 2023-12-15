#include <IO1_drivers.h> 

/** 
 * Faz a convers�o anal�gico digital do sensor de luz e avisa quando terminar 
 */
volatile bool conversion_done = false;

static void lightSensor_ADC_conversion_callback(const struct adc_async_descriptor *const descr, const uint8_t channel) {
	conversion_done = true;
}
void IO_SENSOR_INIT(void){
   /* Inicializa��o par�metros sensor de luz */
   adc_async_register_callback(&IO_SENSOR_ADC, 0, ADC_ASYNC_CONVERT_CB, lightSensor_ADC_conversion_callback);
   adc_async_enable_channel(&IO_SENSOR_ADC, 0);
   adc_async_start_conversion(&IO_SENSOR_ADC);
   
   /* Inicializa��o par�metros USART */
   usart_sync_get_io_descriptor(&TARGETIO, &TARGETIO_DEBUG);
   
   /* Inicializa��o par�metros do sensor de temperatura */
   i2c_m_sync_enable(&I2C_INSTANCE);
   AT30TSE75X = at30tse75x_construct(&AT30TSE75X_descr.parent, &I2C_INSTANCE, CONF_AT30TSE75X_RESOLUTION);
}

/**
 * Escreve 1 byte na UART de debug
 */
void sendByteToUART(uint8_t byte_to_send){
	io_write(TARGETIO_DEBUG, &byte_to_send, 1);
}

/**
 * Le o valor digital do sensor de luz ap�s passar pelo ADC e calcula e entrega a iluminancia com base nas caracteristicas el�tricas do sensor
 */
float readVoltageSensor(void){
	uint8_t IO_SENSOR_VALUE;  //Armazena o valor lido do sensor de luz
	float IO_SENSOR_VOLTAGE; //Armazena a tens�o medida pelo sensor
	/* Faz a convers�o AD do sensor de luz*/
	adc_async_start_conversion(&IO_SENSOR_ADC);
	while(!conversion_done){}
	adc_async_read_channel(&IO_SENSOR_ADC, 0, &IO_SENSOR_VALUE, 1);
	
	/* Faz a defini��o dos valores de tens�o lidos do sensor a partir dos dados quantizados do ADC*/
	IO_SENSOR_VOLTAGE = IO_SENSOR_VALUE * VCC_TARGET / 255;
	return IO_SENSOR_VOLTAGE;
}
float readCurrentSensor(float voltage){
	float IO_SENSOR_CURRENT;
	/* Calcula a corrente com base na diferen�a entre a tens�o de refer�ncia e a tens�o medida,
	 considerando a rela��o entre a corrente do fototransistor e a ilumin�ncia*/
	IO_SENSOR_CURRENT = (VCC_TARGET - voltage)/100000; 
	return IO_SENSOR_CURRENT ;
}
float readLightSensor(float current){
	float IO_ILUMINANCE;    //Representa a ilumin�ncia calculada com base nos valores medidos
	
	IO_ILUMINANCE = (current * 2 *10)/0.000001;
	return IO_ILUMINANCE;
}

uint16_t readTemperatureSensor(void){
	return (uint16_t)temperature_sensor_read(AT30TSE75X);
}

/**
 * Liga o LED da placa de expans�o IO1X
 */
void IO1X_LED_ON(void){
	gpio_set_pin_level(LED, false);
}

/**
 * Converte um n�mero float em string com a precis�o informada
 * � usada para poder printar valor float em tela usando o sprintf
 * O compilador usado n�o aceita float no sprintf
 */
void floatToString(float num, char* str, int precision) {
	int i = 0;
	int integralPart = (int)num;
	
	/* Converte a parte inteira para string */
	do {
		str[i++] = integralPart % 10 + '0';
		integralPart /= 10;
	} while (integralPart > 0);
	
	/* Inverte a string da parte inteira */
	int j;
	int len = i;
	for (j = 0; j < len / 2; j++) {
		char temp = str[j];
		str[j] = str[len - j - 1];
		str[len - j - 1] = temp;
	}
	
	/* Adiciona ponto decimal */
	str[i++] = '.';
	
	/* Converte a parte fracion�ria para string */
	float fractionalPart = num - (int)num;
	int k;
	for (k = 0; k < precision; k++) {
		fractionalPart *= 10;
		int digit = (int)fractionalPart;
		str[i++] = digit + '0';
		fractionalPart -= digit;
	}
	
	/* Adiciona caractere de t�rmino */
	str[i] = '\0';
}
