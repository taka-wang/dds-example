// generated code

#include "dds.h"

#ifndef _DDSL_MB_H_
#define _DDSL_MB_H_

typedef struct Modbus_led
{
  int16_t id;
  bool on;
} Modbus_led;

extern const dds_topic_descriptor_t Modbus_led_desc;

#define Modbus_led__alloc() \
((Modbus_led*) dds_alloc (sizeof (Modbus_led)));

#define Modbus_led_free(d,o) \
dds_sample_free ((d), &Modbus_led_desc, (o))


typedef struct Modbus_voltage
{
  int16_t id;
  float val;
} Modbus_voltage;

extern const dds_topic_descriptor_t Modbus_voltage_desc;

#define Modbus_voltage__alloc() \
((Modbus_voltage*) dds_alloc (sizeof (Modbus_voltage)));

#define Modbus_voltage_free(d,o) \
dds_sample_free ((d), &Modbus_voltage_desc, (o))


#endif /* _DDSL_MB_H_ */
