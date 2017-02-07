#include "arch.h"
#include "ecp_MF256E.h"

/* MF256 NUMS Curve - EDWARDS */

/* Montgomery-Friendly NUMS curves http://eprint.iacr.org/2014/130 */

/* p=k.2^n-1 where n is multiple of BASEBITS - but k can be adjusted. */
/* Here p=2^240(2^14-127)-1 = 2^232(2^22-256.127)-1 where 8*29=232 and (2^22-256.127)=0x3F8100 */
/* or = 2^224(2^30-65536.127)-1 where 4*56=224 and (2^30-65536.127) = 0x3F810000 */



/* Montgomery-Friendly NUMS curves http://eprint.iacr.org/2014/130 */

#if CHUNK==32
const int CURVE_A_MF256E= -1;
const BIG_256 CURVE_Order_MF256E= {0x18EC7BAB,0x16C976F6,0x19CCF259,0x9775F70,0x1FFFFB15,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x3FE9FF};
const BIG_256 CURVE_B_MF256E= {0x350A};
const BIG_256 CURVE_Gx_MF256E= {0x1};
const BIG_256 CURVE_Gy_MF256E= {0x12F3C908,0xF553917,0x1FA9A35F,0xBCC91B,0x1AACA0C,0x1779ED96,0x156BABAF,0x1F1F1989,0xDAD8D4};
#endif

#if CHUNK==64
const int CURVE_A_MF256E= -1;
const BIG_256 CURVE_Order_MF256E= {0xD92EDED8EC7BAB,0xBBAFB86733C966,0xFFFFFFFFFFB154,0xFFFFFFFFFFFFFF,0x3FE9FFFF};
const BIG_256 CURVE_B_MF256E= {0x350A};
const BIG_256 CURVE_Gx_MF256E= {0x1};
const BIG_256 CURVE_Gy_MF256E= {0xEAA722F2F3C908,0x5E648DFEA68D7D,0xF3DB2C1AACA0C0,0xF8CC4D5AEAEBEE,0xDAD8D4F8};
#endif
