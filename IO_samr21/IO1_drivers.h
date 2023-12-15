#ifndef _IO1_drivers
#define _IO1_drivers

#include "driver_init.h"
#include <stdio.h>
#include <at30tse75x.h>
#include <temperature_sensor.h>
#include <at30tse75x_config.h>

#define VCC_TARGET 3.3 // Tensão VCC da placa de R21 usada como referencia
#define CONF_AT30TSE75X_RESOLUTION 2

struct temperature_sensor *AT30TSE75X;
struct io_descriptor* TARGETIO_DEBUG;

static struct at30tse75x AT30TSE75X_descr;

void IO_SENSOR_INIT(void);
void sendByteToUART(uint8_t byte_to_send);
float readVoltageSensor(void);
float readCurrentSensor(float voltage);
float readLightSensor(float current);
void IO1X_LED_ON(void);
void IO1X_LED_OFF(void);
void floatToString(float num, char* str, int precision);


#endif

