
#include "qemu/osdep.h"
#include "hw/misc/mt6253_emi.h"

#define MT_EMI_ERR_DEBUG

#ifdef MT_EMI_ERR_DEBUG
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

static Mt6253RegInfo mt6253_emi_regs[] = {
	{.addr = EMI_CONA,         .name = "EMI_CONA"},
	{.addr = EMI_CONB,         .name = "EMI_CONB"},
	{.addr = EMI_CONC,         .name = "EMI_CONC"},
	{.addr = EMI_COND,         .name = "EMI_COND"},
	{.addr = EMI_GEN,          .name = "EMI_GEN"},
	{.addr = EMI_CLOCKDEL,     .name = "EMI_CLOCKDEL"},
	{.addr = EMI_PREFETCH_CON, .name = "EMI_PREFETCH_CON"},
	{.addr = EMI_ADMUX,        .name = "EMI_ADMUX"},
	{.addr = EMI_EXTCON,       .name = "EMI_EXTCON"},
	{.addr = EMI_EXTCON2,      .name = "EMI_EXTCON2"},
	{.addr = EMI_GEN2,         .name = "EMI_GEN2"},
	{.name = 0}
};

static void mt6253_emi_reset(DeviceState *dev)
{
    //MT6253CfgState *s = MT6253_CFG(dev);

    // TODO
}

static uint64_t mt6253_emi_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    //MT6253CfgState *s = opaque;

    int reg_info_found = 0;
	Mt6253RegInfo *info = mt6253_emi_regs;
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

    return 0;
}

static void mt6253_emi_write(void *opaque, hwaddr addr,
                       uint64_t val64, unsigned int size)
{
    //MT6253CfgState *s = opaque;
    uint32_t value = val64;

	int reg_info_found = 0;
	Mt6253RegInfo *info = mt6253_emi_regs;
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

static const MemoryRegionOps mt6253_emi_ops = {
    .read = mt6253_emi_read,
    .write = mt6253_emi_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void mt6253_emi_init(Object *obj)
{
    MT6253EmiState *s = MT6253_EMI(obj);

    memory_region_init_io(&s->mmio, obj, &mt6253_emi_ops, s,
                          TYPE_MT6253_EMI, 0x10000);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void mt6253_emi_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = mt6253_emi_reset;
}

static const TypeInfo mt6253_emi_info = {
    .name          = TYPE_MT6253_EMI,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(MT6253EmiState),
    .instance_init = mt6253_emi_init,
    .class_init    = mt6253_emi_class_init,
};

static void mt6253_emi_register_types(void)
{
    type_register_static(&mt6253_emi_info);
}

type_init(mt6253_emi_register_types)
