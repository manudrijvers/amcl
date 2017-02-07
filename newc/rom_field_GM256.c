#include "arch.h"
#include "fp_GM256.h"

/* Curve 25519 */

#if CHUNK==16
const chunk MConst_GM256=1;
const BIG_256 Modulus_GM256= {0x1FFF,0x1FFF,0x1FFF,0x1FFF,0x1FFF,0x1FFF,0x1FFF,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x400,0x0,0x0,0x1FF8,0x1FFF,0x1FF};
#endif

#if CHUNK==32
const chunk MConst_GM256=1;
const BIG_256 Modulus_GM256= {0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FF,0x0,0x0,0x40000,0x1FE00000,0xFFFFFF};
#endif

#if CHUNK==64
const chunk MConst_GM256=1;
const BIG_256 Modulus_GM256= {0xFFFFFFFFFFFFFF,0xFFFFFFFFFF,0x0,0x1000000,0xFFFFFFFF};
#endif

