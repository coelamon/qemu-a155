
#include "qemu/osdep.h"
#include "hw/misc/mt6253_cfg.h"

#define MT_CFG_ERR_DEBUG

#ifdef MT_CFG_ERR_DEBUG
#define DPRINTF(fmt, ...) \
do { fprintf(stderr, "%s: " fmt, __func__, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) \
do {} while (0)
#endif

typedef struct Mt6253RegInfo {
	const hwaddr addr;
	const char* name;
} Mt6253RegInfo;

static Mt6253RegInfo mt6253_cfg_regs[] = {
	{.addr = CFG_HW_CODE,    .name = "CFG_HW_CODE"},
	{.addr = CFG_HW_VER,     .name = "CFG_HW_VER"},
	{.addr = CFG_MCUCLK_CON, .name = "CFG_MCUCLK_CON"},
	{.addr = CFG_DSPCLK_CON, .name = "CFG_DSPCLK_CON"},
	{.addr = CFG_PDN_CLR2,   .name = "CFG_PDN_CLR2"},
	{.name = 0}
};

static void mt6253_cfg_reset(DeviceState *dev)
{
    //MT6253CfgState *s = MT6253_CFG(dev);

    // TODO
}

static uint64_t mt6253_cfg_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    //MT6253CfgState *s = opaque;

	int reg_info_found = 0;
	Mt6253RegInfo *info = mt6253_cfg_regs;
	while (info->name != 0) {
		if (info->addr == addr) {
			DPRINTF("read %s\n", info->name);
			reg_info_found = 1;
			break;
		}
		info++;
	}
	if (!reg_info_found) {
		DPRINTF("read: 0x%"HWADDR_PRIx"\n", addr);
	}
	
    switch (addr) {
    case CFG_HW_CODE:
        return 0x6253;
	case CFG_HW_VER:
		return 0x8B03; // what to return?
    }

    return 0;
}

static void mt6253_cfg_write(void *opaque, hwaddr addr,
                       uint64_t val64, unsigned int size)
{
    //MT6253CfgState *s = opaque;
    uint32_t value = val64;

	int reg_info_found = 0;
    Mt6253RegInfo *info = mt6253_cfg_regs;
	while (info->name != 0) {
		if (info->addr == addr) {
			DPRINTF("write %s: 0x%x\n", info->name, value);
			reg_info_found = 1;
			break;
		}
		info++;
	}
	if (!reg_info_found) {
		DPRINTF("write 0x%"HWADDR_PRIx": 0x%x\n", addr, value);
	}
}

static const MemoryRegionOps mt6253_cfg_ops = {
    .read = mt6253_cfg_read,
    .write = mt6253_cfg_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void mt6253_cfg_init(Object *obj)
{
    MT6253CfgState *s = MT6253_CFG(obj);

    memory_region_init_io(&s->mmio, obj, &mt6253_cfg_ops, s,
                          TYPE_MT6253_CFG, 0x1000);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void mt6253_cfg_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = mt6253_cfg_reset;
}

static const TypeInfo mt6253_cfg_info = {
    .name          = TYPE_MT6253_CFG,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(MT6253CfgState),
    .instance_init = mt6253_cfg_init,
    .class_init    = mt6253_cfg_class_init,
};

static void mt6253_cfg_register_types(void)
{
    type_register_static(&mt6253_cfg_info);
}

type_init(mt6253_cfg_register_types)
