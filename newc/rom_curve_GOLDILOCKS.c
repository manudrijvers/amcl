#include "arch.h"
#include "ecp_GOLDILOCKS.h"

/* Curve GOLDILOCKS */

#if CHUNK==32
const int CURVE_A_GOLDILOCKS=1;
const BIG_448 CURVE_Order_GOLDILOCKS= {0xB5844F3,0x1BC61495,0x1163D548,0x1984E51B,0x3690216,0xDA4D76B,0xFA7113B,0x1FEF9944,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x7FF};
const BIG_448 CURVE_B_GOLDILOCKS= {0x1FFF6756,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FDFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFF};
const BIG_448 CURVE_Gx_GOLDILOCKS= {0x15555555,0xAAAAAAA,0x15555555,0xAAAAAAA,0x15555555,0xAAAAAAA,0x15555555,0x152AAAAA,0xAAAAAAA,0x15555555,0xAAAAAAA,0x15555555,0xAAAAAAA,0x15555555,0xAAAAAAA,0x1555};
const BIG_448 CURVE_Gy_GOLDILOCKS= {0xA9386ED,0x1757DE6F,0x13681AF6,0x19657DA3,0x3098BBB,0x12C19D15,0x12E03595,0xE515B18,0x17B7E36D,0x1AC426E,0xDBB5E8,0x10D8560,0x159D6205,0xB8246D9,0x17A58D2B,0x15C0};
#endif

#if CHUNK==64
const int CURVE_A_GOLDILOCKS=1;
const BIG_448 CURVE_Order_GOLDILOCKS= {0x378C292AB5844F3,0x6CC2728DC58F552,0xEDB49AED6369021,0xFFFF7CCA23E9C44,0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0x3FFFFFF};
const BIG_448 CURVE_B_GOLDILOCKS= {0xFFFFFFFFFFF6756,0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFEFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFFFFFFFFFFFFF,0xFFFFFFF};
const BIG_448 CURVE_Gx_GOLDILOCKS= {0x555555555555555,0x555555555555555,0x555555555555555,0xAAA955555555555,0xAAAAAAAAAAAAAAA,0xAAAAAAAAAAAAAAA,0xAAAAAAAAAAAAAAA,0xAAAAAAA};
const BIG_448 CURVE_Gy_GOLDILOCKS= {0xAEAFBCDEA9386ED,0xBCB2BED1CDA06BD,0x565833A2A3098BB,0x6D728AD8C4B80D6,0x7A035884DD7B7E3,0x205086C2B0036ED,0x34AD7048DB359D6,0xAE05E96};
#endif


