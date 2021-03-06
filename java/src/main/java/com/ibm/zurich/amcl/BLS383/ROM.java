/*
	Licensed to the Apache Software Foundation (ASF) under one
	or more contributor license agreements.  See the NOTICE file
	distributed with this work for additional information
	regarding copyright ownership.  The ASF licenses this file
	to you under the Apache License, Version 2.0 (the
	"License"); you may not use this file except in compliance
	with the License.  You may obtain a copy of the License at
	
	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing,
	software distributed under the License is distributed on an
	"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
	KIND, either express or implied.  See the License for the
	specific language governing permissions and limitations
	under the License.
*/

/* Fixed Data in ROM - Field and Curve parameters */


package com.ibm.zurich.amcl.BLS383;

public class ROM
{

// Base Bits= 58
public static final long[] Modulus= {0x2ACAAB52AAD556BL,0x6EC051D7DD75EL,0x16CF73083D5D752L,0xC814C6083E67ACL,0x3852C01355A68EAL,0x27DD718417154A9L,0x7AC52080AL};
public static final long[] R2modp= {0x21D559796C5EBFBL,0x8FDEC2DAAFDED6L,0x104E5B315F6AF66L,0x34B0FB71D7537D3L,0xE63A3C6309B4FCL,0x3CC89B133BB5EFAL,0x1D66B7E04L};
public static final long MConst= 0xA59AB3B123D0BDL;

public static final int CURVE_A= 0;
public static final int CURVE_B_I= 9;
public static final long[] CURVE_B= {0x9L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L};
public static final long[] CURVE_Order= {0xFFF80000FFF001L,0x32FF7801C3F9E00L,0x3A3000049C5EDF1L,0x13310001FE44001L,0x1464100L,0x0L,0x0L};
public static final long[] CURVE_Gx= {0xD59B348D10786BL,0x2CD1DF038FD52B4L,0x25BF25B734578B9L,0x2713DAB001EEDBDL,0x2EFD5830FF57EL,0x3262B6E7E23EDBBL,0xB08CEE4BL};
public static final long[] CURVE_Gy= {0x5DA023D145DDBL,0x3C4FD46317FBDF3L,0x1F56EC3462B2A66L,0xFA5BCC0671EA49L,0x259906104798122L,0x19C412042B63D6FL,0x1F3909337L};

public static final long[] CURVE_Bnx= {0x1000000040L,0x44L,0x0L,0x0L,0x0L,0x0L,0x0L};
public static final long[] CURVE_Cof= {0x2A00000052BL,0x155582AAAAACB28L,0x605L,0x0L,0x0L,0x0L,0x0L};
public static final long[] CURVE_Cru= {0x1A3AAC4EDA155A9L,0x177CBFA1D87978FL,0x26BCDFAADE63262L,0x20D448C4A34C0D6L,0x190DBF3A2BBEAD6L,0x27DD717EAC81090L,0x7AC52080AL};
public static final long[] Fra= {0x32BA59A92B4508BL,0x118F6DE81BBBCD0L,0x1340341CB1DFBC7L,0x35058E7A74CB557L,0x398B19B3F05CC36L,0x19CBCC8FB9361AAL,0x5A5FB198L};
public static final long[] Frb= {0x381051A97F904E0L,0x2EDF5269BC21A8DL,0x38F3EEB8B7DB8AL,0x177BBDE60F1B255L,0x3EC7A65F6549CB3L,0xE11A4F45DDF2FEL,0x751F25672L};
public static final long[] CURVE_Pxa= {0x16059885BAC9472L,0x1DF134C778B70DBL,0x3CBDC90C308C88AL,0x19CA7C065C71A23L,0xBF3693539C43F1L,0x1AFF607969587AEL,0x4D50722B5L};
public static final long[] CURVE_Pxb= {0x9B4BD7A272AB23L,0x11EBC6753D1373AL,0x3B3F6F7B93206A3L,0x3B95C774F8AA067L,0x1263A2BA3B635D7L,0x396EB0A31E03068L,0xEE3617C3L};
public static final long[] CURVE_Pya= {0x281D230977BD4FDL,0x302D981C837F7F1L,0x3A41FC9590C89A0L,0x194B87EF3E1E0A1L,0x25E011C23014EEEL,0x170A21E205AECCL,0x8F40859AL};
public static final long[] CURVE_Pyb= {0xA5E20A252C4CE6L,0xD641E9D2BFD032L,0x1941760A42448EFL,0x127FFBE0AC3F686L,0x17BA0F29A18D4EAL,0x304AB1FDEE1B926L,0x1DCABBA88L};
public static final long[][] CURVE_W= {{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L}};
public static final long[][][] CURVE_SB= {{{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L}},{{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L}}};
public static final long[][] CURVE_WB= {{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L}};
public static final long[][][] CURVE_BB= {{{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L}},{{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L}},{{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L}},{{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L},{0x0L,0x0L,0x0L,0x0L,0x0L,0x0L,0x0L}}};

}

