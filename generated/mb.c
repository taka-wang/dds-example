// generated code

#include "mb.h"


static const dds_key_descriptor_t Modbus_led_keys[1] =
{
  { "id", 0 }
};

static const uint32_t Modbus_led_ops [] =
{
  DDS_OP_ADR | DDS_OP_TYPE_2BY | DDS_OP_FLAG_KEY, offsetof (Modbus_led, id),
  DDS_OP_ADR | DDS_OP_TYPE_BOO, offsetof (Modbus_led, on),
  DDS_OP_RTS
};

const dds_topic_descriptor_t Modbus_led_desc =
{
  sizeof (Modbus_led),
  (sizeof(bool)>2u)?sizeof(bool):2u,
  DDS_TOPIC_FIXED_KEY,
  1u,
  "Modbus::led",
  Modbus_led_keys,
  Modbus_led_ops,
  "<MetaData version=\"1.0.0\"><Module name=\"Modbus\"><Struct name=\"led\"><Member name=\"id\"><Short/></Member><Member name=\"on\"><Boolean/></Member></Struct></Module></MetaData>"
};


static const dds_key_descriptor_t Modbus_voltage_keys[1] =
{
  { "id", 0 }
};

static const uint32_t Modbus_voltage_ops [] =
{
  DDS_OP_ADR | DDS_OP_TYPE_2BY | DDS_OP_FLAG_KEY, offsetof (Modbus_voltage, id),
  DDS_OP_ADR | DDS_OP_TYPE_4BY, offsetof (Modbus_voltage, val),
  DDS_OP_RTS
};

const dds_topic_descriptor_t Modbus_voltage_desc =
{
  sizeof (Modbus_voltage),
  4u,
  DDS_TOPIC_FIXED_KEY,
  1u,
  "Modbus::voltage",
  Modbus_voltage_keys,
  Modbus_voltage_ops,
  "<MetaData version=\"1.0.0\"><Module name=\"Modbus\"><Struct name=\"voltage\"><Member name=\"id\"><Short/></Member><Member name=\"val\"><Float/></Member></Struct></Module></MetaData>"
};
