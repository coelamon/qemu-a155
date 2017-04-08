
#include "qemu/osdep.h"
#include "hw/misc/mt6253_emi.h"
#include "exec/address-spaces.h"
#include "hw/loader.h"

#define MT_EMI_ERR_DEBUG

#ifdef MT_EMI_ERR_DEBUG
#define DPRINTF(fmt, ...) \
do { fprintf(stderr, "%s: " fmt, __func__, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) \
do {} while (0)
#endif

static char boot_code_bin[] =
{
	0, 0, 0, 0,
	0x04, 0xF0, 0x1F, 0xE5, // ldr pc, 4
	0x00, 0x00, 0x00, 0x48  // 0x48000000
};

typedef struct Mt6253RegInfo {
	const hwaddr addr;
	const char* name;
} Mt6253RegInfo;

static Mt6253RegInfo mt6253_emi_regs[] = {
	{.addr = EMI_CONA,         .name = "EMI_CONA"},
	{.addr = EMI_CONB,         .name = "EMI_CONB"},
	{.addr = EMI_CONC,         .name = "EMI_CONC"},
	{.addr = EMI_COND,         .name = "EMI_COND"},
	{.addr = EMI_REMAP,        .name = "EMI_REMAP"},
	{.addr = EMI_GEN,          .name = "EMI_GEN"},
	{.addr = EMI_CLOCKDEL,     .name = "EMI_CLOCKDEL"},
	{.addr = EMI_PREFETCH_CON, .name = "EMI_PREFETCH_CON"},
	{.addr = EMI_ADMUX,        .name = "EMI_ADMUX"},
	{.addr = EMI_EXTCON,       .name = "EMI_EXTCON"},
	{.addr = EMI_EXTCON2,      .name = "EMI_EXTCON2"},
	{.addr = EMI_GEN2,         .name = "EMI_GEN2"},
	{.name = 0}
};

static void mt6253_emi_remap_ctl_changed(MT6253EmiState *obj)
{
	MT6253EmiState *s = obj;
	
	MemoryRegion *system_memory = get_system_memory();
	
	if (s->boot_code_is_mapped)
	{
		memory_region_del_subregion(system_memory, &s->boot_code);
		s->boot_code_is_mapped = 0;
	}
	
	if (s->bank0_is_mapped)
	{
		memory_region_del_subregion(system_memory, &s->bank0);
		s->bank0_is_mapped = 0;
	}
	
	if (s->bank1_is_mapped)
	{
		memory_region_del_subregion(system_memory, &s->bank1);
		s->bank1_is_mapped = 0;
	}
	
	if (s->remap_ctl == EMI_REMAP_0_BC_BANK1)
	{
		memory_region_add_subregion(system_memory, EMI_ADDRESS_0, &s->boot_code);
		memory_region_add_subregion(system_memory, EMI_ADDRESS_1, &s->bank1);
		s->boot_code_is_mapped = 1;
		s->bank1_is_mapped = 1;
		DPRINTF("EMI: Boot Code => 0x00000000\n");
		DPRINTF("EMI: BANK1     => 0x08000000\n");
	}
	
	if (s->remap_ctl == EMI_REMAP_1_BANK1_BANK0 || s->remap_ctl == EMI_REMAP_3_BANK1_BANK0)
	{
		memory_region_add_subregion(system_memory, EMI_ADDRESS_0, &s->bank1);
		memory_region_add_subregion(system_memory, EMI_ADDRESS_1, &s->bank0);
		s->bank0_is_mapped = 1;
		s->bank1_is_mapped = 1;
		DPRINTF("EMI: BANK1 => 0x00000000\n");
		DPRINTF("EMI: BANK0 => 0x08000000\n");
	}
	
	if (s->remap_ctl == EMI_REMAP_2_BANK0_BANK1)
	{
		memory_region_add_subregion(system_memory, EMI_ADDRESS_0, &s->bank0);
		memory_region_add_subregion(system_memory, EMI_ADDRESS_1, &s->bank1);
		s->bank0_is_mapped = 1;
		s->bank1_is_mapped = 1;
		DPRINTF("EMI: BANK0 => 0x00000000\n");
		DPRINTF("EMI: BANK1 => 0x08000000\n");
	}
}

static void mt6253_emi_reset(DeviceState *dev)
{
    MT6253EmiState *s = MT6253_EMI(dev);

    s->remap_ctl = EMI_REMAP_0_BC_BANK1;
	mt6253_emi_remap_ctl_changed(s);
}

static uint64_t mt6253_emi_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    //MT6253EmiState *s = opaque;

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
    MT6253EmiState *s = opaque;
    uint32_t value = val64;

	switch (addr) {
		case EMI_REMAP:
			s->remap_ctl = value;
			mt6253_emi_remap_ctl_changed(s);
			break;
	}
	
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
	
	Error *err = NULL;
	
	s->boot_code_is_mapped = 0;
	s->bank0_is_mapped = 0;
	s->bank1_is_mapped = 0;
	
	memory_region_init_ram_ptr(&s->boot_code, NULL, "MT6253.EMI.BOOTCODE", sizeof(boot_code_bin),
                           boot_code_bin);
	vmstate_register_ram_global(&s->boot_code);
	
	memory_region_init_ram(&s->bank0, NULL, "MT6253.EMI.BANK0", BANK0_SIZE,
                           &err);
	vmstate_register_ram_global(&s->bank0);
	
	memory_region_init_ram(&s->bank1, NULL, "MT6253.EMI.BANK1", BANK1_SIZE,
                           &err);
	vmstate_register_ram_global(&s->bank1);
	
	s->remap_ctl = EMI_REMAP_0_BC_BANK1;
	mt6253_emi_remap_ctl_changed(s);

    memory_region_init_io(&s->mmio, obj, &mt6253_emi_ops, s,
                          TYPE_MT6253_EMI, 0x10000);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void mt6253_emi_realize(DeviceState *dev_emi, Error **errp)
{
	MT6253EmiState *s = MT6253_EMI(dev_emi);
	
	load_image_mr(s->kernel_filename, &s->bank0);
}

static Property mt6253_emi_properties[] = {
    DEFINE_PROP_STRING("kernel-filename", MT6253EmiState, kernel_filename),
    DEFINE_PROP_END_OF_LIST(),
};

static void mt6253_emi_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = mt6253_emi_reset;
	dc->realize = mt6253_emi_realize;
	dc->props = mt6253_emi_properties;
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
