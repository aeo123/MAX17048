#ifndef MAX17048_H
#define MAX17048_H

#include <rtdevice.h>
#include <board.h>

typedef uint8_t (*Ops_With_Addr)(unsigned char addr, unsigned char *buf, unsigned char len);

typedef struct
{
    Ops_With_Addr ReadWithAddr;
    Ops_With_Addr WriteWithAddr;
} MAX_Ops;

typedef enum
{
    REGISTER_VCELL = 0x02,
    REGISTER_SOC = 0x04,
    REGISTER_MODE = 0x06,
    REGISTER_VERSION = 0x08,
    REGISTER_HIBRT = 0x0A,
    REGISTER_CONFIG = 0x0C,
    REGISTER_VALRT = 0x14,
    REGISTER_CRATE = 0x16,
    REGISTER_VRESET_ID = 0x18,
    REGISTER_STATUS = 0x1A,
    REGISTER_TABLE_START = 0x40,
    REGISTER_CMD = 0xFE
} max_register_t;

void max17048_init(Ops_With_Addr read_ops, Ops_With_Addr write_ops);
uint8_t max17048_get_permille(float *per);
uint8_t max17048_get_millivolt(float *volt);
uint8_t max17048_get_verison(uint16_t *ver);
uint8_t max17048_get_config(uint16_t *config);
uint8_t max_setRegister(max_register_t reg, uint16_t value);
uint8_t max_getRegister(max_register_t reg, uint16_t *value);

#endif
