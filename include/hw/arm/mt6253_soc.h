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

#ifndef HW_ARM_MT6253_H
#define HW_ARM_MT6253_H

#include "hw/misc/mt6253_cfg.h"
#include "hw/misc/mt6253_emi.h"
#include "hw/char/mt6253_uart.h"

#define TYPE_MT6253_SOC "mt6253-soc"
#define MT6253_SOC(obj) \
    OBJECT_CHECK(MT6253State, (obj), TYPE_MT6253_SOC)

#define MT_NUM_UARTS 3

#define SYSTEMRAM_BASE_ADDRESS 0x40000000
#define SYSTEMRAM_SIZE (144 * 1024)

#define SYSTEMROM_BASE_ADDRESS 0x48000000
#define SYSTEMROM_SIZE (20 * 1024)

typedef struct MT6253State {
    /*< private >*/
    SysBusDevice parent_obj;
    /*< public >*/

    char *kernel_filename;
    char *cpu_model;

    MT6253CfgState cfg;
	MT6253EmiState emi;
    MT6253UartState uart[MT_NUM_UARTS];
} MT6253State;

#endif