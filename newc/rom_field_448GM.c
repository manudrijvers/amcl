#include "arch.h"
#include "fp_448GM.h"

/* Curve GOLDILOCKS */

#if CHUNK==32

const BIG_448 Modulus_448GM= {0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FDFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFF};
const chunk MConst_448GM=0x1;
#endif

#if CHUNK==64
const BIG_448 Modulus_448GM= {0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFEFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFFFFF};
const chunk MConst_448GM=0x1;
#endif


