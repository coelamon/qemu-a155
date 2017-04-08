
#include "qemu/osdep.h"
#include "hw/misc/mt6253_lcd.h"

#define MT_LCD_ERR_DEBUG

#ifdef MT_LCD_ERR_DEBUG
#define DPRINTF(fmt, ...) \
do { fprintf(stderr, "%s: " fmt, __func__, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) \
do {} while (0)
#endif

static void mt6253_lcd_reset(DeviceState *dev)
{
    //MT6253CfgState *s = MT6253_CFG(dev);

    // TODO
}

static uint64_t mt6253_lcd_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    //MT6253CfgState *s = opaque;

    return 0;
}

static void mt6253_lcd_write(void *opaque, hwaddr addr,
                       uint64_t val64, unsigned int size)
{
    //MT6253CfgState *s = opaque;
    uint32_t value = val64;

	DPRINTF("write 0x%"HWADDR_PRIx": 0x%x\n", addr, value);
}

static const MemoryRegionOps mt6253_lcd_ops = {
    .read = mt6253_lcd_read,
    .write = mt6253_lcd_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void mt6253_lcd_init(Object *obj)
{
    MT6253LcdState *s = MT6253_LCD(obj);

    memory_region_init_io(&s->mmio, obj, &mt6253_lcd_ops, s,
                          TYPE_MT6253_LCD, 0x10000);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void mt6253_lcd_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = mt6253_lcd_reset;
}

static const TypeInfo mt6253_lcd_info = {
    .name          = TYPE_MT6253_LCD,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(MT6253LcdState),
    .instance_init = mt6253_lcd_init,
    .class_init    = mt6253_lcd_class_init,
};

static void mt6253_lcd_register_types(void)
{
    type_register_static(&mt6253_lcd_info);
}

type_init(mt6253_lcd_register_types)
