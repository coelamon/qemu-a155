/*
 * A155 Machine Model
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
#include "hw/boards.h"
#include "qemu/error-report.h"
#include "hw/arm/mt6253_soc.h"
#include "qemu/log.h"
#include "qemu/error-report.h"
#include "qapi/error.h"

#ifndef A155_ERR_DEBUG
#define A155_ERR_DEBUG 1
#endif

#define DB_PRINT_L(lvl, fmt, args...) do { \
    if (A155_ERR_DEBUG >= lvl) { \
        qemu_log("%s: " fmt, __func__, ## args); \
    } \
} while (0);

#define DB_PRINT(fmt, args...) DB_PRINT_L(1, fmt, ## args)

static void a155_init(MachineState *machine)
{
    DeviceState *dev;
    Error *err = NULL;

	qemu_log("test qemu_log");
	
	DB_PRINT("create soc");
	
    dev = qdev_create(NULL, TYPE_MT6253_SOC);
    if (machine->kernel_filename) {
        qdev_prop_set_string(dev, "kernel-filename", machine->kernel_filename);
    }
    qdev_prop_set_string(dev, "cpu-model", "arm926");
    object_property_set_bool(OBJECT(dev), true, "realized", &err);
    if (err != NULL) {
        error_report("%s", error_get_pretty(err));
        exit(1);
    }
	
	DB_PRINT("return");
}

static void a155_machine_init(MachineClass *mc)
{
    mc->desc = "A155 Machine";
    mc->init = a155_init;
}

DEFINE_MACHINE("a155", a155_machine_init)
