/*
 * MT6253 UART
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
#include "hw/char/mt6253_uart.h"

#define MT_UART_ERR_DEBUG

#ifdef MT_UART_ERR_DEBUG
#define DPRINTF(fmt, ...) \
do { fprintf(stderr, "%s: " fmt, __func__, ## __VA_ARGS__); } while (0)
#else
#define DPRINTF(fmt, ...) \
do {} while (0)
#endif

static void mt6253_uart_reset(DeviceState *dev)
{
    // MT6253UartState *s = MT6253_UART(dev);

    // TODO
}

static uint64_t mt6253_uart_read(void *opaque, hwaddr addr,
                                       unsigned int size)
{
    //MT6253UartState *s = opaque;
    //uint64_t retvalue;

	switch (addr)
	{
		case UART_LSR:
			return UART_LSR_THRE;
		default:
			DPRINTF("Read 0x%"HWADDR_PRIx"\n", addr);
			break;
	}
	
	return 0;
}

static void mt6253_uart_write(void *opaque, hwaddr addr,
                                  uint64_t val64, unsigned int size)
{
    //MT6253UartState *s = opaque;
    uint32_t value = val64;

	switch (addr)
	{
		case UART_THR:
			fprintf(stderr, "%c", value);
			break;
		default:
			DPRINTF("Write 0x%" PRIx32 ", 0x%"HWADDR_PRIx"\n", value, addr);
			break;
	}
}

static const MemoryRegionOps mt6253_uart_ops = {
    .read = mt6253_uart_read,
    .write = mt6253_uart_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void mt6253_uart_init(Object *obj)
{
    MT6253UartState *s = MT6253_UART(obj);

    memory_region_init_io(&s->mmio, obj, &mt6253_uart_ops, s,
                          TYPE_MT6253_UART, 0x100);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void mt6253_uart_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = mt6253_uart_reset;
}

static const TypeInfo mt6253_uart_info = {
    .name          = TYPE_MT6253_UART,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(MT6253UartState),
    .instance_init = mt6253_uart_init,
    .class_init    = mt6253_uart_class_init,
};

static void mt6253_uart_register_types(void)
{
    type_register_static(&mt6253_uart_info);
}

type_init(mt6253_uart_register_types)