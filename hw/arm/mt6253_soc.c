/*
 * MT6253 SoC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qemu/osdep.h"
#include "hw/arm/arm.h"
#include "target/arm/cpu.h"
#include "exec/address-spaces.h"
#include "hw/arm/mt6253_soc.h"
#include "hw/loader.h"
#include "qemu/error-report.h"
#include "qapi/error.h"

#define MT_SOC_ERR_DEBUG

#ifdef MT_SOC_ERR_DEBUG
#define DPRINTF(fmt, ...) \
do { fprintf(stderr, "%s: " fmt, __func__, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) \
do {} while (0)
#endif

static char system_rom_prog[] = {
	0x04, 0xF0, 0x1F, 0xE5, // ldr pc, 4
	0x00, 0x00, 0x00, 0x00  // 0x00000000
};

static const uint32_t uart_addr[MT_NUM_UARTS] = { 0x81030000, 0x81040000, 0x81050000 };

static void mt6253_soc_initfn(Object *obj)
{
    MT6253State *s = MT6253_SOC(obj);
    int i;

	DPRINTF("init cfg\n");
	
    object_initialize(&s->cfg, sizeof(s->cfg), TYPE_MT6253_CFG);
    qdev_set_parent_bus(DEVICE(&s->cfg), sysbus_get_default());
	
	DPRINTF("init emi\n");
	
    object_initialize(&s->emi, sizeof(s->emi), TYPE_MT6253_EMI);
    qdev_set_parent_bus(DEVICE(&s->emi), sysbus_get_default());

	DPRINTF("init uarts\n");
	
    for (i = 0; i < MT_NUM_UARTS; i++) {
        object_initialize(&s->uart[i], sizeof(s->uart[i]),
                          TYPE_MT6253_UART);
        qdev_set_parent_bus(DEVICE(&s->uart[i]), sysbus_get_default());
    }
}

static void mt6253_soc_realize(DeviceState *dev_soc, Error **errp)
{
	MT6253State *s = MT6253_SOC(dev_soc);
    DeviceState *cfgdev, *uartdev, *emidev;
    SysBusDevice *cfgbusdev, *uartbusdev, *emibusdev;
	ARMCPU *cpu;
	// qemu_irq *pic;
    Error *err = NULL;
    int i;
	
	MemoryRegion *system_memory = get_system_memory();
    MemoryRegion *systemram = g_new(MemoryRegion, 1);
	MemoryRegion *systemrom = g_new(MemoryRegion, 1);
	
	DPRINTF("system ram\n");
	
    memory_region_init_ram(systemram, NULL, "MT6253.systemram", SYSTEMRAM_SIZE,
                           &err);
    vmstate_register_ram_global(systemram);
    memory_region_add_subregion(system_memory, SYSTEMRAM_BASE_ADDRESS, systemram);
	
	DPRINTF("system rom\n");
	
	memory_region_init_ram_ptr(systemrom, NULL, "MT6253.systemrom", SYSTEMROM_SIZE,
                           system_rom_prog);
    vmstate_register_ram_global(systemrom);
    memory_region_add_subregion(system_memory, SYSTEMROM_BASE_ADDRESS, systemrom);
	
	DPRINTF("cpu\n");
	
	cpu = cpu_arm_init(s->cpu_model);
    if (cpu == NULL) {
        fprintf(stderr, "Unable to find CPU definition\n");
        exit(1);
    }
	
	DPRINTF("cfg regs\n");
	cfgdev = DEVICE(&s->cfg);
    object_property_set_bool(OBJECT(&s->cfg), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    cfgbusdev = SYS_BUS_DEVICE(cfgdev);
    sysbus_mmio_map(cfgbusdev, 0, 0x80010000);
	
	DPRINTF("emi\n");
	emidev = DEVICE(&s->emi);
	qdev_prop_set_string(emidev, "kernel-filename", s->kernel_filename);
    object_property_set_bool(OBJECT(&s->emi), true, "realized", &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    emibusdev = SYS_BUS_DEVICE(emidev);
    sysbus_mmio_map(emibusdev, 0, 0x81000000);
	
	// Attach UART
	DPRINTF("attach uart\n");
    for (i = 0; i < MT_NUM_UARTS; i++) {
        uartdev = DEVICE(&(s->uart[i]));
        object_property_set_bool(OBJECT(&s->uart[i]), true, "realized", &err);
        if (err != NULL) {
            error_propagate(errp, err);
            return;
        }
        uartbusdev = SYS_BUS_DEVICE(uartdev);
        sysbus_mmio_map(uartbusdev, 0, uart_addr[i]);
    }
}

static Property mt6253_soc_properties[] = {
    DEFINE_PROP_STRING("kernel-filename", MT6253State, kernel_filename),
    DEFINE_PROP_STRING("cpu-model", MT6253State, cpu_model),
    DEFINE_PROP_END_OF_LIST(),
};

static void mt6253_soc_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->realize = mt6253_soc_realize;
    dc->props = mt6253_soc_properties;
}

static const TypeInfo mt6253_soc_info = {
    .name          = TYPE_MT6253_SOC,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(MT6253State),
    .instance_init = mt6253_soc_initfn,
    .class_init    = mt6253_soc_class_init,
};

static void mt6253_soc_types(void)
{
    type_register_static(&mt6253_soc_info);
}

type_init(mt6253_soc_types)
