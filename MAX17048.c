#include "MAX17048.h"

#define MAX17048_ADDRESS_HARD (0x6C)
#define MAX17048_ADDRESS_SOFT (0x6C >> 1)

#define MAX17048_ADDRESS MAX17048_ADDRESS_HARD

static rt_device_t i2c_dev = RT_NULL;

//使用了DMA了可以访问的内存
static __attribute__((section(".RAM_D1"))) uint8_t memoryRead[3];
static __attribute__((section(".RAM_D1"))) uint8_t memoryWrite[3];

static uint8_t read_with_address(uint8_t addr, uint8_t *buf, uint8_t len)
{
    int ret = 0;
    ret = rt_device_read(i2c_dev, addr, buf, len);
    return ret;
}
static uint8_t write_with_address(uint8_t addr, uint8_t *buf, uint8_t len)
{
    int ret = 0;
    ret = rt_device_write(i2c_dev, addr, buf, len);
    return ret;
}

void max17048_init(const char *bus_name)
{
    i2c_dev = rt_device_find(bus_name);
    if (i2c_dev == RT_NULL)
    {
        rt_kprintf("max17048 i2c bus can not be find");
    }
    if (rt_device_open(i2c_dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("max17048 i2c bus open failed");
        return;
    }
    rt_thread_delay(20);
}

/**
 * @brief  读取剩余电量
 * @note   
 * @retval 
 */
uint8_t max17048_get_permille(float *per)
{
    
		memoryWrite[0] = REGISTER_SOC;
	
    if (write_with_address(MAX17048_ADDRESS, memoryWrite, 1) != 1)
        return 0;
    if (read_with_address(MAX17048_ADDRESS, memoryRead, 2) != 2)
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


		memoryWrite[0] = REGISTER_VCELL;
	
    if (write_with_address(MAX17048_ADDRESS, memoryWrite, 1) != 1)
        return 0;
    if (read_with_address(MAX17048_ADDRESS, memoryRead, 2) != 2)
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
  

    memoryWrite[0] = reg;
    memoryWrite[1] = value >> 8;
    memoryWrite[2] = value;

    if (write_with_address(MAX17048_ADDRESS, memoryWrite, 3) != 3)
        return 0;

    return 1;
}

uint8_t max_getRegister(max_register_t reg, uint16_t *value)
{

		memoryWrite[0] = reg;
	
    if (write_with_address(MAX17048_ADDRESS, memoryWrite, 1) != 1)
        return 0;
    if (read_with_address(MAX17048_ADDRESS, memoryRead, 2) != 2)
        return 0;

    *value = ((uint16_t)memoryRead[0] << 8) | memoryRead[1];

    return 1;
}