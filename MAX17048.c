#include "MAX17048.h"

#define MAX17048_ADDRESS (0x6C >> 1)

static MAX_Ops max_ops;

void max17048_init(Ops_With_Addr read_ops, Ops_With_Addr write_ops)
{
    max_ops.ReadWithAddr = read_ops;
    max_ops.WriteWithAddr = write_ops;
}

/**
 * @brief  读取剩余电量
 * @note   
 * @retval 
 */
uint8_t max17048_get_permille(float *per)
{
    uint8_t memoryRead[3];
    uint8_t cmd = REGISTER_SOC;

    if (max_ops.WriteWithAddr(MAX17048_ADDRESS, &cmd, 1) != 1)
        return 0;
    if (max_ops.ReadWithAddr(MAX17048_ADDRESS, memoryRead, 2) != 2)
        return 0;

    uint16_t value = ((uint16_t)memoryRead[0] << 8) | memoryRead[1];
    // remove rounding error when converting percent to per mille
    if (value > 100 * 256)
    {
        value = 100 * 256;
    }
    *per = ((float)value / 256.0f);
    return 1;
}

/**
 * @brief  读取电池电压。单位MV
 * @note   
 * @param  callback: 
 * @retval None
 */
uint8_t max17048_get_millivolt(float *volt)
{
    uint8_t memoryRead[3];

    uint8_t cmd = REGISTER_VCELL;

    if (max_ops.WriteWithAddr(MAX17048_ADDRESS, &cmd, 1) != 1)
        return 0;
    if (max_ops.ReadWithAddr(MAX17048_ADDRESS, memoryRead, 2) != 2)
        return 0;

    uint16_t value = ((uint16_t)memoryRead[0] << 8) | memoryRead[1];

    *volt = (((float)value) * 78.125f / 1000.0f);
    return 1;
}

/**
 * @brief  获取芯片版本
 * @note   
 * @retval 
 */
uint8_t max17048_get_verison(uint16_t *ver)
{
    return max_getRegister(REGISTER_VERSION, ver);
}

/**
 * @brief  读取配置寄存器
 * @note   
 * @param  *config: 
 * @retval 
 */
uint8_t max17048_get_config(uint16_t *config)
{
    return max_getRegister(REGISTER_CONFIG, config);
}

/**
 * @brief  
 * @note   
 * @param  *config: 
 * @retval 
 */
uint8_t max17048_get_valrt(uint16_t *val)
{
    return max_getRegister(REGISTER_VALRT, val);
}

/**
 * @brief  
 * @note   
 * @param  *val: 
 * @retval 
 */
uint8_t max17048_get_status(uint16_t *val)
{
    return max_getRegister(REGISTER_STATUS, val);
}

/**
 * @brief  设置寄存器
 * @note   
 * @param  reg: 
 * @param  value: 
 * @retval None
 */
uint8_t max_setRegister(max_register_t reg, uint16_t value)
{
    uint8_t memoryWrite[3];

    memoryWrite[0] = reg;
    memoryWrite[1] = value >> 8;
    memoryWrite[2] = value;

    if (max_ops.WriteWithAddr(MAX17048_ADDRESS, memoryWrite, 3) != 3)
        return 0;

    return 1;
}

uint8_t max_getRegister(max_register_t reg, uint16_t *value)
{
    uint8_t memoryRead[3];

    if (max_ops.WriteWithAddr(MAX17048_ADDRESS, &reg, 1) != 1)
        return 0;
    if (max_ops.ReadWithAddr(MAX17048_ADDRESS, memoryRead, 2) != 2)
        return 0;

    *value = ((uint16_t)memoryRead[0] << 8) | memoryRead[1];

    return 1;
}