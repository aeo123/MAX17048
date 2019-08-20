# MAX17048电池监测芯片驱动

目录：/packages/peripherals/max17048

依赖：自行添加I2C驱动

## 使用示例

``` c
static rt_device_t i2c_dev = RT_NULL;
static uint8_t i2c_write(unsigned char addr, unsigned char *buf, unsigned char len)
{
    int ret = 0;
    ret = rt_device_write(i2c_dev, addr, buf, len);
    return ret;
}

static uint8_t i2c_read(unsigned char addr, unsigned char *buf, unsigned char len)
{
    int ret = 0;
    ret = rt_device_read(i2c_dev, addr, buf, len);
    return ret;
}

static void bat_mx17048_init()
{
    i2c_dev = rt_device_find("i2c1");
    if (i2c_dev == RT_NULL)
    {
        LOG_E("i2c1 open failed1");
    }
    if (rt_device_open(i2c_dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        LOG_E("i2c1 open failed2");
        return;
    }
    rt_thread_delay(100);
    
    //初始化max17048
    max17048_init(i2c_read, i2c_write);
    
    uint16_t ver = 0;
    //读取芯片版本
    if (max17048_get_verison(&ver))
    {
        LOG_D("bat version %X", ver);
    }
    else
    {
        LOG_D("read bat version failed");
    }
    
    float bat = 0;
    //读取电池电压mv
    if(max17048_get_millivolt(&bat)){
        LOG_D("volt %d mv", (int)bat);
    }

    //剩余电量百分比
    if(max17048_get_permille(&bat)){
        LOG_D("bat %d %%", (int)bat);
    }

	uint16_t config;
    //读取配置寄存器
    if(max17048_get_config(&config)){
        LOG_D("config %X ", (int)config);
    }

    //设置寄存器
    //uint8_t max_setRegister(max_register_t reg, uint16_t value);
}
```

