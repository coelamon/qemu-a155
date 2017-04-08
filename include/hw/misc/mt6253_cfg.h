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

#ifndef HW_MT6253_CFG_H
#define HW_MT6253_CFG_H

#include "hw/sysbus.h"
#include "hw/hw.h"

#define CFG_HW_VER      0x00
#define CFG_SW_VER      0x04
#define CFG_HW_CODE     0x08
#define CFG_MCUCLK_CON  0x118
#define CFG_DSPCLK_CON  0x11C
#define CFG_PDN_CLR2    0x328

#define TYPE_MT6253_CFG "mt6253-cfg"
#define MT6253_CFG(obj) \
    OBJECT_CHECK(MT6253CfgState, (obj), TYPE_MT6253_CFG)

typedef struct {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

} MT6253CfgState;

#endif /* HW_MT6253_CFG_H */
