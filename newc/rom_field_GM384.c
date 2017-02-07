#include "arch.h"
#include "fp_GM384.h"

/* Curve NIST384 */

#if CHUNK==32
const BIG_384 Modulus_GM384= {0xFFFFFFF,0xF,0x0,0xFFFF000,0xFFEFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFFFF,0xFFFFF};
const chunk MConst_GM384=0x1;
#endif

#if CHUNK==64
const BIG_384 Modulus_GM384= {0xFFFFFFFF,0xFFFF0000000000,0xFFFFFFFFFEFFFF,0xFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFF,0xFFFFFFFFFFFF};
const chunk MConst_GM384=0x100000001;
#endif


