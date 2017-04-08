
#include "qemu/osdep.h"
#include "hw/misc/mt6253_gpio.h"

#define MT_GPIO_ERR_DEBUG

#ifdef MT_GPIO_ERR_DEBUG
#define DPRINTF(fmt, ...) \
do { fprintf(stderr, "%s: " fmt, __func__, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) \
do {} while (0)
#endif

static void mt6253_gpio_reset(DeviceState *dev)
{
    //MT6253CfgState *s = MT6253_CFG(dev);

    // TODO
}

static uint64_t mt6253_gpio_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    //MT6253CfgState *s = opaque;

	DPRINTF("read 0x%"HWADDR_PRIx"\n", addr);
	
    return 0;
}

static void mt6253_gpio_write(void *opaque, hwaddr addr,
                       uint64_t val64, unsigned int size)
{
    //MT6253CfgState *s = opaque;
    uint32_t value = val64;

	DPRINTF("write 0x%"HWADDR_PRIx": 0x%x\n", addr, value);
}

static const MemoryRegionOps mt6253_gpio_ops = {
    .read = mt6253_gpio_read,
    .write = mt6253_gpio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void mt6253_gpio_init(Object *obj)
{
    MT6253GpioState *s = MT6253_GPIO(obj);

    memory_region_init_io(&s->mmio, obj, &mt6253_gpio_ops, s,
                          TYPE_MT6253_GPIO, 0x10000);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void mt6253_gpio_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = mt6253_gpio_reset;
}

static const TypeInfo mt6253_gpio_info = {
    .name          = TYPE_MT6253_GPIO,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(MT6253GpioState),
    .instance_init = mt6253_gpio_init,
    .class_init    = mt6253_gpio_class_init,
};

static void mt6253_gpio_register_types(void)
{
    type_register_static(&mt6253_gpio_info);
}

type_init(mt6253_gpio_register_types)
