/*
 * MT6253 CFG
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

#ifndef HW_MT6253_EMI_H
#define HW_MT6253_EMI_H

#include "hw/sysbus.h"
#include "hw/hw.h"

#define EMI_CONA          0x00
#define EMI_CONB          0x04
#define EMI_CONC          0x08
#define EMI_COND          0x0C
#define EMI_GEN           0x44
#define EMI_CLOCKDEL      0x48
#define EMI_PREFETCH_CON  0x70
#define EMI_ADMUX         0x74
#define EMI_EXTCON        0x78
#define EMI_EXTCON2       0x7C
#define EMI_GEN2          0x80

#define TYPE_MT6253_EMI "mt6253-emi"
#define MT6253_EMI(obj) \
    OBJECT_CHECK(MT6253EmiState, (obj), TYPE_MT6253_EMI)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

} MT6253EmiState;

#endif /* HW_MT6253_EMI_H */
