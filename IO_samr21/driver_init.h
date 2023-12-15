#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>
#include <hal_adc_async.h>
#include <hal_usart_sync.h>
#include <hal_i2c_m_sync.h>

extern struct adc_async_descriptor IO_SENSOR_ADC;
extern struct usart_sync_descriptor TARGETIO;
extern struct i2c_m_sync_desc I2C_INSTANCE;

void IO_SENSOR_ADC_INIT(void);
void TARGETIO_PORT_INIT(void);
void TARGETIO_CLOCK_INIT(void);
void TARGETIO_init(void);
void I2C_INSTANCE_CLOCK_INSTANCE(void);
void I2C_INSTANCE_INIT(void);
void I2C_INSTANCE_PORT_INIT(void);
void delay_driver_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif

#endif // DRIVER_INIT_INCLUDED
