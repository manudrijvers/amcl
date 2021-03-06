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

/* MPIN Functions */

/* Version 3.0 - supports Time Permits */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpin_ZZZ.h"

#define ROUNDUP(a,b) ((a)-1)/(b)+1

/* Special mpin hashing */
static void mpin_hash(int sha,FP4_YYY *f, ECP_ZZZ *P,octet *w)
{
    int i;
    BIG_XXX x,y;
    char h[64];
    hash256 sha256;
    hash512 sha512;
    char t[6*MODBYTES_XXX];  // to hold 6 BIGs
    int hlen=sha;
    BIG_XXX_copy(x,f->a.a);
    FP_YYY_redc(x);
    BIG_XXX_toBytes(&t[0],x);
    BIG_XXX_copy(x,f->a.b);
    FP_YYY_redc(x);
    BIG_XXX_toBytes(&t[MODBYTES_XXX],x);
    BIG_XXX_copy(x,f->b.a);
    FP_YYY_redc(x);
    BIG_XXX_toBytes(&t[2*MODBYTES_XXX],x);
    BIG_XXX_copy(x,f->b.b);
    FP_YYY_redc(x);
    BIG_XXX_toBytes(&t[3*MODBYTES_XXX],x);
    ECP_ZZZ_get(x,y,P);
    BIG_XXX_toBytes(&t[4*MODBYTES_XXX],x);
    BIG_XXX_toBytes(&t[5*MODBYTES_XXX],y);

    OCT_empty(w);
    switch (sha)
    {
    case SHA256:
        HASH256_init(&sha256);
        for (i=0; i<6*MODBYTES_XXX; i++) HASH256_process(&sha256,t[i]);
        HASH256_hash(&sha256,h);
        break;
    case SHA384:
        HASH384_init(&sha512);
        for (i=0; i<6*MODBYTES_XXX; i++) HASH384_process(&sha512,t[i]);
        HASH384_hash(&sha512,h);
        break;
    case SHA512:
        HASH512_init(&sha512);
        for (i=0; i<6*MODBYTES_XXX; i++) HASH512_process(&sha512,t[i]);
        HASH512_hash(&sha512,h);
        break;
    }

    OCT_jbytes(w,h,PAS_ZZZ);
    for (i=0; i<hlen; i++) h[i]=0;
}

/* these next two functions help to implement elligator squared - http://eprint.iacr.org/2014/043 */
/* maps a random u to a point on the curve */
static void map(ECP_ZZZ *P,BIG_XXX u,int cb)
{
    BIG_XXX x,q;

    BIG_XXX_rcopy(q,Modulus_YYY);
    BIG_XXX_copy(x,u);
    BIG_XXX_mod(x,q);

    while (!ECP_ZZZ_setx(P,x,cb))
        BIG_XXX_inc(x,1);
}

/* returns u derived from P. Random value in range 1 to return value should then be added to u */
static int unmap(BIG_XXX u,int *cb,ECP_ZZZ *P)
{
    int s,r=0;
    BIG_XXX x;

    s=ECP_ZZZ_get(x,x,P);
    BIG_XXX_copy(u,x);
    do
    {
        BIG_XXX_dec(u,1);
        r++;
    }
    while (!ECP_ZZZ_setx(P,u,s));
    ECP_ZZZ_setx(P,x,s);

    *cb=s;

    return r;
}

/* map octet string containing hash to point on curve of correct order */
static void mapit(octet *h,ECP_ZZZ *P)
{
    BIG_XXX q,x,c;
    BIG_XXX_fromBytes(x,h->val);
    BIG_XXX_rcopy(q,Modulus_YYY);
    BIG_XXX_mod(x,q);

    while (!ECP_ZZZ_setx(P,x,0))
        BIG_XXX_inc(x,1);

    BIG_XXX_rcopy(c,CURVE_Cof_ZZZ);
    ECP_ZZZ_mul(P,c);
}

/* needed for SOK */
/* static void mapit2(octet *h,ECP2_ZZZ *Q) */
/* { */
/* 	BIG_XXX q,one,Fx,Fy,x,hv; */
/* 	FP2_YYY X; */
/* 	ECP2_ZZZ T,K; */
/* 	BIG_XXX_fromBytes(hv,h->val); */
/* 	BIG_XXX_rcopy(q,Modulus_YYY); */
/* 	BIG_XXX_one(one); */
/* 	BIG_XXX_mod(hv,q); */

/* 	for (;;) */
/* 	{ */
/* 		FP2_YYY_from_BIGs(&X,one,hv); */
/* 		if (ECP2_ZZZ_setx(Q,&X)) break; */
/* 		BIG_XXX_inc(hv,1);  */
/* 	} */

/* /\* Fast Hashing to G2 - Fuentes-Castaneda, Knapp and Rodriguez-Henriquez *\/ */
/* 	BIG_XXX_rcopy(Fx,CURVE_Fra_ZZZ); */
/* 	BIG_XXX_rcopy(Fy,CURVE_Frb_ZZZ); */
/* 	FP2_YYY_from_BIGs(&X,Fx,Fy); */
/* 	BIG_XXX_rcopy(x,CURVE_Bnx_ZZZ); */

/* 	ECP2_ZZZ_copy(&T,Q); */
/* 	ECP2_ZZZ_mul(&T,x); */
/* 	ECP2_ZZZ_neg(&T);  /\* our x is negative *\/ */
/* 	ECP2_ZZZ_copy(&K,&T); */
/* 	ECP2_ZZZ_dbl(&K); */
/* 	ECP2_ZZZ_add(&K,&T); */
/* 	ECP2_ZZZ_affine(&K); */

/* 	ECP2_ZZZ_frob(&K,&X); */
/* 	ECP2_ZZZ_frob(Q,&X); ECP2_ZZZ_frob(Q,&X); ECP2_ZZZ_frob(Q,&X);  */
/* 	ECP2_ZZZ_add(Q,&T); */
/* 	ECP2_ZZZ_add(Q,&K); */
/* 	ECP2_ZZZ_frob(&T,&X); ECP2_ZZZ_frob(&T,&X); */
/* 	ECP2_ZZZ_add(Q,&T); */
/* 	ECP2_ZZZ_affine(Q); */
/* } */


/* these next two functions implement elligator squared - http://eprint.iacr.org/2014/043 */
/* Elliptic curve point E in format (0x04,x,y} is converted to form {0x0-,u,v} */
/* Note that u and v are indistinguisible from random strings */
int MPIN_ZZZ_ENCODING(csprng *RNG,octet *E)
{
    int rn,m,su,sv,res=0;

    BIG_XXX q,u,v;
    ECP_ZZZ P,W;

    if (!ECP_ZZZ_fromOctet(&P,E)) res=MPIN_INVALID_POINT;

    if (res==0)
    {
        BIG_XXX_rcopy(q,Modulus_YYY);

        BIG_XXX_randomnum(u,q,RNG);

        su=RAND_byte(RNG);
        if (su<0) su=-su;
        su%=2;
        map(&W,u,su);
        ECP_ZZZ_sub(&P,&W);

        rn=unmap(v,&sv,&P);
        m=RAND_byte(RNG);
        if (m<0) m=-m;
        m%=rn;
        BIG_XXX_inc(v,m+1);
        E->val[0]=su+2*sv;
        BIG_XXX_toBytes(&(E->val[1]),u);
        BIG_XXX_toBytes(&(E->val[PFS_ZZZ+1]),v);
    }

    return res;
}

int MPIN_ZZZ_DECODING(octet *D)
{
    int su,sv;
    BIG_XXX u,v;
    ECP_ZZZ P,W;
    int res=0;

    if ((D->val[0]&0x04)!=0) res=MPIN_INVALID_POINT;
    if (res==0)
    {

        BIG_XXX_fromBytes(u,&(D->val[1]));
        BIG_XXX_fromBytes(v,&(D->val[PFS_ZZZ+1]));

        su=D->val[0]&1;
        sv=(D->val[0]>>1)&1;

        map(&W,u,su);
        map(&P,v,sv);

        ECP_ZZZ_add(&P,&W);
        ECP_ZZZ_toOctet(D,&P);
    }
    return res;
}

/* R=R1+R2 in group G1 */
int MPIN_ZZZ_RECOMBINE_G1(octet *R1,octet *R2,octet *R)
{
    ECP_ZZZ P,T;
    int res=0;
    if (res==0)
    {
        if (!ECP_ZZZ_fromOctet(&P,R1)) res=MPIN_INVALID_POINT;
        if (!ECP_ZZZ_fromOctet(&T,R2)) res=MPIN_INVALID_POINT;
    }
    if (res==0)
    {
        ECP_ZZZ_add(&P,&T);
        ECP_ZZZ_toOctet(R,&P);
    }
    return res;
}

/* W=W1+W2 in group G2 */
int MPIN_ZZZ_RECOMBINE_G2(octet *W1,octet *W2,octet *W)
{
    ECP2_ZZZ Q,T;
    int res=0;
    if (!ECP2_ZZZ_fromOctet(&Q,W1)) res=MPIN_INVALID_POINT;
    if (!ECP2_ZZZ_fromOctet(&T,W2)) res=MPIN_INVALID_POINT;
    if (res==0)
    {
        ECP2_ZZZ_add(&Q,&T);
        ECP2_ZZZ_toOctet(W,&Q);
    }
    return res;
}

/* create random secret S */
int MPIN_ZZZ_RANDOM_GENERATE(csprng *RNG,octet* S)
{
    BIG_XXX r,s;

    BIG_XXX_rcopy(r,CURVE_Order_ZZZ);
    BIG_XXX_randomnum(s,r,RNG);
#ifdef AES_S
    BIG_XXX_mod2m(s,2*AES_S);
#endif
    BIG_XXX_toBytes(S->val,s);
    S->len=MODBYTES_XXX;
    return 0;
}

/* Extract PIN from TOKEN for identity CID */
int MPIN_ZZZ_EXTRACT_PIN(int sha,octet *CID,int pin,octet *TOKEN)
{
    ECP_ZZZ P,R;
    int res=0;
    char h[MODBYTES_XXX];
    octet H= {0,sizeof(h),h};

    if (!ECP_ZZZ_fromOctet(&P,TOKEN))  res=MPIN_INVALID_POINT;
    if (res==0)
    {
        mhashit(sha,-1,CID,&H);
        mapit(&H,&R);

        pin%=MAXPIN_ZZZ;

        ECP_ZZZ_pinmul(&R,pin,PBLEN_ZZZ);
        ECP_ZZZ_sub(&P,&R);

        ECP_ZZZ_toOctet(TOKEN,&P);
    }
    return res;
}

/* Implement step 2 on client side of MPin protocol - SEC=-(x+y)*SEC */
int MPIN_ZZZ_CLIENT_2(octet *X,octet *Y,octet *SEC)
{
    BIG_XXX px,py,r;
    ECP_ZZZ P;
    int res=0;
    BIG_XXX_rcopy(r,CURVE_Order_ZZZ);
    if (!ECP_ZZZ_fromOctet(&P,SEC)) res=MPIN_INVALID_POINT;
    if (res==0)
    {
        BIG_XXX_fromBytes(px,X->val);
        BIG_XXX_fromBytes(py,Y->val);
        BIG_XXX_add(px,px,py);
        BIG_XXX_mod(px,r);
        //	BIG_XXX_sub(px,r,px);
        PAIR_ZZZ_G1mul(&P,px);
        ECP_ZZZ_neg(&P);
        ECP_ZZZ_toOctet(SEC,&P);
    }
    return res;
}

/*
 W=x*H(G);
 if RNG == NULL then X is passed in
 if RNG != NULL the X is passed out
 if type=0 W=x*G where G is point on the curve, else W=x*M(G), where M(G) is mapping of octet G to point on the curve
*/

int MPIN_ZZZ_GET_G1_MULTIPLE(csprng *RNG,int type,octet *X,octet *G,octet *W)
{
    ECP_ZZZ P;
    BIG_XXX r,x;
    int res=0;
    if (RNG!=NULL)
    {
        BIG_XXX_rcopy(r,CURVE_Order_ZZZ);
        BIG_XXX_randomnum(x,r,RNG);
#ifdef AES_S
        BIG_XXX_mod2m(x,2*AES_S);
#endif
        X->len=MODBYTES_XXX;
        BIG_XXX_toBytes(X->val,x);
    }
    else
        BIG_XXX_fromBytes(x,X->val);

    if (type==0)
    {
        if (!ECP_ZZZ_fromOctet(&P,G)) res=MPIN_INVALID_POINT;
    }
    else mapit(G,&P);

    if (res==0)
    {
        PAIR_ZZZ_G1mul(&P,x);
        ECP_ZZZ_toOctet(W,&P);
    }
    return res;
}

/*
 if RNG == NULL then X is passed in
 if RNG != NULL the X is passed out
 W=x*G where G is point on the curve
 if type==1 W=(x^-1)G
*/

int MPIN_ZZZ_GET_G2_MULTIPLE(csprng *RNG,int type,octet *X,octet *G,octet *W)
{
    ECP2_ZZZ P;
    BIG_XXX r,x;
    int res=0;
    BIG_XXX_rcopy(r,CURVE_Order_ZZZ);
    if (RNG!=NULL)
    {
        BIG_XXX_randomnum(x,r,RNG);
#ifdef AES_S
        BIG_XXX_mod2m(x,2*AES_S);
#endif
        X->len=MODBYTES_XXX;
        BIG_XXX_toBytes(X->val,x);
    }
    else
    {
        BIG_XXX_fromBytes(x,X->val);
        if (type==1) BIG_XXX_invmodp(x,x,r);
    }

    if (!ECP2_ZZZ_fromOctet(&P,G)) res=MPIN_INVALID_POINT;

    if (res==0)
    {
        PAIR_ZZZ_G2mul(&P,x);
        ECP2_ZZZ_toOctet(W,&P);
    }
    return res;
}



/* Client secret CST=s*H(CID) where CID is client ID and s is master secret */
/* CID is hashed externally */
int MPIN_ZZZ_GET_CLIENT_SECRET(octet *S,octet *CID,octet *CST)
{
    return MPIN_ZZZ_GET_G1_MULTIPLE(NULL,1,S,CID,CST);
}

/* Implement step 1 on client side of MPin protocol */
int MPIN_ZZZ_CLIENT_1(int sha,int date,octet *CLIENT_ID,csprng *RNG,octet *X,int pin,octet *TOKEN,octet *SEC,octet *xID,octet *xCID,octet *PERMIT)
{
    BIG_XXX r,x;
    ECP_ZZZ P,T,W;
    int res=0;
    char h[MODBYTES_XXX];
    octet H= {0,sizeof(h),h};

    BIG_XXX_rcopy(r,CURVE_Order_ZZZ);
    if (RNG!=NULL)
    {
        BIG_XXX_randomnum(x,r,RNG);
#ifdef AES_S
        BIG_XXX_mod2m(x,2*AES_S);
#endif
        X->len=MODBYTES_XXX;
        BIG_XXX_toBytes(X->val,x);
    }
    else
        BIG_XXX_fromBytes(x,X->val);

    mhashit(sha,-1,CLIENT_ID,&H);
    mapit(&H,&P);

    if (!ECP_ZZZ_fromOctet(&T,TOKEN)) res=MPIN_INVALID_POINT;

    if (res==0)
    {
        pin%=MAXPIN_ZZZ;

        ECP_ZZZ_copy(&W,&P);				// W=H(ID)
        ECP_ZZZ_pinmul(&W,pin,PBLEN_ZZZ);			// W=alpha.H(ID)
        ECP_ZZZ_add(&T,&W);					// T=Token+alpha.H(ID) = s.H(ID)

        if (date)
        {
            if (PERMIT!=NULL)
            {
                if (!ECP_ZZZ_fromOctet(&W,PERMIT)) res=MPIN_INVALID_POINT;
                ECP_ZZZ_add(&T,&W);					// SEC=s.H(ID)+s.H(T|ID)
            }
            mhashit(sha,date,&H,&H);
            mapit(&H,&W);
            if (xID!=NULL)
            {
                PAIR_ZZZ_G1mul(&P,x);				// P=x.H(ID)
                ECP_ZZZ_toOctet(xID,&P);  // xID
                PAIR_ZZZ_G1mul(&W,x);               // W=x.H(T|ID)
                ECP_ZZZ_add(&P,&W);
            }
            else
            {
                ECP_ZZZ_add(&P,&W);
                PAIR_ZZZ_G1mul(&P,x);
            }
            if (xCID!=NULL) ECP_ZZZ_toOctet(xCID,&P);  // U
        }
        else
        {
            if (xID!=NULL)
            {
                PAIR_ZZZ_G1mul(&P,x);				// P=x.H(ID)
                ECP_ZZZ_toOctet(xID,&P);  // xID
            }
        }
    }

    if (res==0)
        ECP_ZZZ_toOctet(SEC,&T);  // V

    return res;
}

/* Extract Server Secret SST=S*Q where Q is fixed generator in G2 and S is master secret */
int MPIN_ZZZ_GET_SERVER_SECRET(octet *S,octet *SST)
{
    BIG_XXX r,s;
    FP2_YYY qx,qy;
    ECP2_ZZZ Q;
    int res=0;

    BIG_XXX_rcopy(r,CURVE_Order_ZZZ);
    BIG_XXX_rcopy(qx.a,CURVE_Pxa_ZZZ);
    FP_YYY_nres(qx.a);
    BIG_XXX_rcopy(qx.b,CURVE_Pxb_ZZZ);
    FP_YYY_nres(qx.b);
    BIG_XXX_rcopy(qy.a,CURVE_Pya_ZZZ);
    FP_YYY_nres(qy.a);
    BIG_XXX_rcopy(qy.b,CURVE_Pyb_ZZZ);
    FP_YYY_nres(qy.b);
    ECP2_ZZZ_set(&Q,&qx,&qy);

    if (res==0)
    {
        BIG_XXX_fromBytes(s,S->val);
        PAIR_ZZZ_G2mul(&Q,s);
        ECP2_ZZZ_toOctet(SST,&Q);
    }

    return res;
}


/* Time Permit CTT=s*H(date|H(CID)) where s is master secret */
int MPIN_ZZZ_GET_CLIENT_PERMIT(int sha,int date,octet *S,octet *CID,octet *CTT)
{
    BIG_XXX s;
    ECP_ZZZ P;
    char h[MODBYTES_XXX];
    octet H= {0,sizeof(h),h};

    mhashit(sha,date,CID,&H);

    mapit(&H,&P);
    BIG_XXX_fromBytes(s,S->val);
    PAIR_ZZZ_G1mul(&P,s);

    ECP_ZZZ_toOctet(CTT,&P);
    return 0;
}

// if date=0 only use HID, set HCID=NULL
// if date and PE, use HID and HCID

/* Outputs H(CID) and H(CID)+H(T|H(CID)) for time permits. If no time permits set HTID=NULL */
void MPIN_ZZZ_SERVER_1(int sha,int date,octet *CID,octet *HID,octet *HTID)
{
    char h[MODBYTES_XXX];
    octet H= {0,sizeof(h),h};
    ECP_ZZZ P,R;

#ifdef USE_ANONYMOUS
    mapit(CID,&P);
#else
    mhashit(sha,-1,CID,&H);
    mapit(&H,&P);
#endif

    ECP_ZZZ_toOctet(HID,&P);  // new

    if (date)
    {
        //	if (HID!=NULL) ECP_ZZZ_toOctet(HID,&P);
#ifdef USE_ANONYMOUS
        mhashit(sha,date,CID,&H);
#else
        mhashit(sha,date,&H,&H);
#endif
        mapit(&H,&R);
        ECP_ZZZ_add(&P,&R);
        ECP_ZZZ_toOctet(HTID,&P);
    }
    //else ECP_ZZZ_toOctet(HID,&P);

}

/* Implement M-Pin on server side */
int MPIN_ZZZ_SERVER_2(int date,octet *HID,octet *HTID,octet *Y,octet *SST,octet *xID,octet *xCID,octet *mSEC,octet *E,octet *F)
{
    BIG_XXX px,py,y;
    FP2_YYY qx,qy;
    FP12_YYY g;
    ECP2_ZZZ Q,sQ;
    ECP_ZZZ P,R;
    int res=0;

    BIG_XXX_rcopy(qx.a,CURVE_Pxa_ZZZ);
    FP_YYY_nres(qx.a);
    BIG_XXX_rcopy(qx.b,CURVE_Pxb_ZZZ);
    FP_YYY_nres(qx.b);
    BIG_XXX_rcopy(qy.a,CURVE_Pya_ZZZ);
    FP_YYY_nres(qy.a);
    BIG_XXX_rcopy(qy.b,CURVE_Pyb_ZZZ);
    FP_YYY_nres(qy.b);

    if (!ECP2_ZZZ_set(&Q,&qx,&qy)) res=MPIN_INVALID_POINT;

    if (res==0)
    {
        if (!ECP2_ZZZ_fromOctet(&sQ,SST)) res=MPIN_INVALID_POINT;
    }

    if (res==0)
    {
        if (date)
        {
            BIG_XXX_fromBytes(px,&(xCID->val[1]));
            BIG_XXX_fromBytes(py,&(xCID->val[PFS_ZZZ+1]));
        }
        else
        {
            BIG_XXX_fromBytes(px,&(xID->val[1]));
            BIG_XXX_fromBytes(py,&(xID->val[PFS_ZZZ+1]));
        }
        if (!ECP_ZZZ_set(&R,px,py)) res=MPIN_INVALID_POINT; // x(A+AT)
    }
    if (res==0)
    {
        BIG_XXX_fromBytes(y,Y->val);
        if (date)
        {
            if (!ECP_ZZZ_fromOctet(&P,HTID))  res=MPIN_INVALID_POINT;
        }
        else
        {
            if (!ECP_ZZZ_fromOctet(&P,HID))  res=MPIN_INVALID_POINT;
        }
    }
    if (res==0)
    {
        PAIR_ZZZ_G1mul(&P,y);  // y(A+AT)
        ECP_ZZZ_add(&P,&R); // x(A+AT)+y(A+T)
        if (!ECP_ZZZ_fromOctet(&R,mSEC))  res=MPIN_INVALID_POINT; // V
    }
    if (res==0)
    {
        PAIR_ZZZ_double_ate(&g,&Q,&R,&sQ,&P);
        PAIR_ZZZ_fexp(&g);

        if (!FP12_YYY_isunity(&g))
        {
            if (HID!=NULL && xID!=NULL && E!=NULL && F !=NULL)
            {
                /* xID is set to NULL if there is no way to calculate PIN error */
                FP12_YYY_toOctet(E,&g);

                /* Note error is in the PIN, not in the time permit! Hence the need to exclude Time Permit from this check */

                if (date)
                {
                    if (!ECP_ZZZ_fromOctet(&P,HID)) res=MPIN_INVALID_POINT;
                    if (!ECP_ZZZ_fromOctet(&R,xID)) res=MPIN_INVALID_POINT; // U

                    if (res==0)
                    {
                        PAIR_ZZZ_G1mul(&P,y);  // yA
                        ECP_ZZZ_add(&P,&R); // yA+xA
                    }
                }
                if (res==0)
                {
                    PAIR_ZZZ_ate(&g,&Q,&P);
                    PAIR_ZZZ_fexp(&g);
                    FP12_YYY_toOctet(F,&g);
                }
            }
            res=MPIN_BAD_PIN;
        }
    }

    return res;
}

#if MAXPIN_ZZZ==10000
#define MR_TS 10  /* 2^10/10 approx = sqrt(MAXPIN) */
#define TRAP 200  /* 2*sqrt(MAXPIN) */
#endif

#if MAXPIN_ZZZ==1000000
#define MR_TS 14
#define TRAP 2000
#endif

/* Pollards kangaroos used to return PIN error */
int MPIN_ZZZ_KANGAROO(octet *E,octet *F)
{
    int i,j,m,s,dn,dm,steps;
    int distance[MR_TS];
    FP12_YYY ge,gf,t,table[MR_TS];
    int res=0;
    // BIG_XXX w;

    FP12_YYY_fromOctet(&ge,E);
    FP12_YYY_fromOctet(&gf,F);

    FP12_YYY_copy(&t,&gf);

    for (s=1,m=0; m<MR_TS; m++)
    {
        distance[m]=s;
        FP12_YYY_copy(&table[m],&t);
        s*=2;
        FP12_YYY_usqr(&t,&t);
        FP12_YYY_reduce(&t);
    }

    FP12_YYY_one(&t);

    for (dn=0,j=0; j<TRAP; j++)
    {

        //BIG_XXX_copy(w,t.a.a.a);
        //FP_YYY_redc(w);
        //i=BIG_XXX_lastbits(w,20)%MR_TS;

        i=t.a.a.a[0]%MR_TS;

        FP12_YYY_mul(&t,&table[i]);
        FP12_YYY_reduce(&t);
        dn+=distance[i];
    }

    FP12_YYY_conj(&gf,&t);
    steps=0;
    dm=0;
    while (dm-dn<MAXPIN_ZZZ)
    {
        steps++;
        if (steps>4*TRAP) break;

        //BIG_XXX_copy(w,ge.a.a.a);
        //FP_YYY_redc(w);
        //i=BIG_XXX_lastbits(w,20)%MR_TS;

        i=ge.a.a.a[0]%MR_TS;

        FP12_YYY_mul(&ge,&table[i]);
        FP12_YYY_reduce(&ge);
        dm+=distance[i];
        if (FP12_YYY_equals(&ge,&t))
        {
            res=dm-dn;
            break;
        }
        if (FP12_YYY_equals(&ge,&gf))
        {
            res=dn-dm;
            break;
        }
    }
    if (steps>4*TRAP || dm-dn>=MAXPIN_ZZZ)
    {
        res=0;    /* Trap Failed  - probable invalid token */
    }

    return res;
}

/* Functions to support M-Pin Full */

int MPIN_ZZZ_PRECOMPUTE(octet *TOKEN,octet *CID,octet *CP,octet *G1,octet *G2)
{
    ECP_ZZZ P,T;
    ECP2_ZZZ Q;
    FP2_YYY qx,qy;
    FP12_YYY g;
    int res=0;

    if (!ECP_ZZZ_fromOctet(&T,TOKEN)) res=MPIN_INVALID_POINT;

    if (res==0)
    {
        mapit(CID,&P);
        if (CP!=NULL)
        {
            if (!ECP2_ZZZ_fromOctet(&Q,CP)) res=MPIN_INVALID_POINT;
        }
        else
        {
            BIG_XXX_rcopy(qx.a,CURVE_Pxa_ZZZ);
            FP_YYY_nres(qx.a);
            BIG_XXX_rcopy(qx.b,CURVE_Pxb_ZZZ);
            FP_YYY_nres(qx.b);
            BIG_XXX_rcopy(qy.a,CURVE_Pya_ZZZ);
            FP_YYY_nres(qy.a);
            BIG_XXX_rcopy(qy.b,CURVE_Pyb_ZZZ);
            FP_YYY_nres(qy.b);
            if (!ECP2_ZZZ_set(&Q,&qx,&qy)) res=MPIN_INVALID_POINT;
        }
    }
    if (res==0)
    {
        PAIR_ZZZ_ate(&g,&Q,&T);
        PAIR_ZZZ_fexp(&g);

        FP12_YYY_toOctet(G1,&g);
        if (G2!=NULL)
        {
            PAIR_ZZZ_ate(&g,&Q,&P);
            PAIR_ZZZ_fexp(&g);
            FP12_YYY_toOctet(G2,&g);
        }
    }
    return res;
}

/* calculate common key on client side */
/* wCID = w.(A+AT) */
int MPIN_ZZZ_CLIENT_KEY(int sha,octet *G1,octet *G2,int pin,octet *R,octet *X,octet *H,octet *wCID,octet *CK)
{
    FP12_YYY g1,g2;
    FP4_YYY c,cp,cpm1,cpm2;
    FP2_YYY f;
    ECP_ZZZ W;
    int res=0;
    BIG_XXX r,z,x,q,m,a,b,h;

    FP12_YYY_fromOctet(&g1,G1);
    FP12_YYY_fromOctet(&g2,G2);
    BIG_XXX_fromBytes(z,R->val);
    BIG_XXX_fromBytes(x,X->val);
    BIG_XXX_fromBytes(h,H->val);

    if (!ECP_ZZZ_fromOctet(&W,wCID)) res=MPIN_INVALID_POINT;

    if (res==0)
    {
        BIG_XXX_rcopy(r,CURVE_Order_ZZZ);
        BIG_XXX_add(z,z,h);    // new
        BIG_XXX_mod(z,r);

        PAIR_ZZZ_G1mul(&W,x);

        BIG_XXX_rcopy(a,CURVE_Fra_ZZZ);
        BIG_XXX_rcopy(b,CURVE_Frb_ZZZ);
        FP2_YYY_from_BIGs(&f,a,b);

        BIG_XXX_rcopy(q,Modulus_YYY);
        BIG_XXX_copy(m,q);
        BIG_XXX_mod(m,r);

        BIG_XXX_copy(a,z);
        BIG_XXX_mod(a,m);

        BIG_XXX_copy(b,z);
        BIG_XXX_sdiv(b,m);

        FP12_YYY_pinpow(&g2,pin,PBLEN_ZZZ);
        FP12_YYY_mul(&g1,&g2);

        FP12_YYY_trace(&c,&g1);

        FP12_YYY_copy(&g2,&g1);
        FP12_YYY_frob(&g2,&f);
        FP12_YYY_trace(&cp,&g2);

        FP12_YYY_conj(&g1,&g1);
        FP12_YYY_mul(&g2,&g1);
        FP12_YYY_trace(&cpm1,&g2);
        FP12_YYY_mul(&g2,&g1);
        FP12_YYY_trace(&cpm2,&g2);

        FP4_YYY_xtr_pow2(&c,&cp,&c,&cpm1,&cpm2,a,b);
        mpin_hash(sha,&c,&W,CK);

    }
    return res;
}

/* calculate common key on server side */
/* Z=r.A - no time permits involved */

int MPIN_ZZZ_SERVER_KEY(int sha,octet *Z,octet *SST,octet *W,octet *H,octet *HID,octet *xID,octet *xCID,octet *SK)
{
    int res=0;
    FP12_YYY g;
    FP4_YYY c;
    ECP_ZZZ R,U,A;
    ECP2_ZZZ sQ;
    BIG_XXX w,h;

    if (!ECP2_ZZZ_fromOctet(&sQ,SST)) res=MPIN_INVALID_POINT;
    if (!ECP_ZZZ_fromOctet(&R,Z)) res=MPIN_INVALID_POINT;


    if (!ECP_ZZZ_fromOctet(&A,HID)) res=MPIN_INVALID_POINT;

    // new
    if (xCID!=NULL)
    {
        if (!ECP_ZZZ_fromOctet(&U,xCID)) res=MPIN_INVALID_POINT;
    }
    else
    {
        if (!ECP_ZZZ_fromOctet(&U,xID)) res=MPIN_INVALID_POINT;
    }
    BIG_XXX_fromBytes(w,W->val);
    BIG_XXX_fromBytes(h,H->val);


    PAIR_ZZZ_ate(&g,&sQ,&A);
    PAIR_ZZZ_fexp(&g);

    if (res==0)
    {
        PAIR_ZZZ_G1mul(&A,h);
        ECP_ZZZ_add(&R,&A);  // new
        PAIR_ZZZ_ate(&g,&sQ,&R);
        PAIR_ZZZ_fexp(&g);
        PAIR_ZZZ_G1mul(&U,w);
        FP12_YYY_trace(&c,&g);
        mpin_hash(sha,&c,&U,SK);
    }
    return res;
}

/* Generate Y = H(TimeValue, xCID/xID) */
void MPIN_ZZZ_GET_Y(int sha,int TimeValue,octet *xCID,octet *Y)
{
    BIG_XXX q,y;
    char h[MODBYTES_XXX];
    octet H= {0,sizeof(h),h};

    mhashit(sha,TimeValue,xCID,&H);
    BIG_XXX_fromBytes(y,H.val);
    BIG_XXX_rcopy(q,CURVE_Order_ZZZ);
    BIG_XXX_mod(y,q);
    BIG_XXX_toBytes(Y->val,y);
    Y->len=PGS_ZZZ;
}

/* One pass MPIN Client */
int MPIN_ZZZ_CLIENT(int sha,int date,octet *ID,csprng *RNG,octet *X,int pin,octet *TOKEN,octet *V,octet *U,octet *UT,octet *TP,/*octet *MESSAGE,*/int TimeValue,octet *Y)
{
    int rtn=0;
    char m[2*PFS_ZZZ+1];
    octet M= {0,sizeof(m),m};

    octet *pID;
    if (date == 0)
        pID = U;
    else
        pID = UT;

    rtn = MPIN_ZZZ_CLIENT_1(sha,date,ID,RNG,X,pin,TOKEN,V,U,UT,TP);
    if (rtn != 0)
        return rtn;

    OCT_joctet(&M,pID);
//    if (MESSAGE!=NULL)
//    {
//        OCT_joctet(&M,MESSAGE);
//    }

    MPIN_ZZZ_GET_Y(sha,TimeValue,&M,Y);

    rtn = MPIN_ZZZ_CLIENT_2(X,Y,V);
    if (rtn != 0)
        return rtn;

    return 0;
}

/* One pass MPIN Server */
int MPIN_ZZZ_SERVER(int sha,int date,octet *HID,octet *HTID,octet *Y,octet *sQ,octet *U,octet *UT,octet *V,octet *E,octet *F,octet *ID,/*octet *MESSAGE,*/int TimeValue)
{
    int rtn=0;
    char m[2*PFS_ZZZ+1];
    octet M= {0,sizeof(m),m};

    octet *pU;
    if (date == 0)
        pU = U;
    else
        pU = UT;

    MPIN_ZZZ_SERVER_1(sha,date,ID,HID,HTID);

    OCT_joctet(&M,pU);
//    if (MESSAGE!=NULL)
//    {
//        OCT_joctet(&M,MESSAGE);
//    }

    MPIN_ZZZ_GET_Y(sha,TimeValue,&M,Y);

    rtn = MPIN_ZZZ_SERVER_2(date,HID,HTID,Y,sQ,U,UT,V,E,F);
    if (rtn != 0)
        return rtn;

    return 0;
}


/*
int main()
{
	ECP2_ZZZ X;
	FP2_YYY x,y,rhs;
	BIG_XXX r;
	char hcid[HASH_BYTES],client_id[100];
	octet HCID={0,sizeof(hcid),hcid};
	octet CLIENT_ID={0,sizeof(client_id),client_id};

	OCT_jstring(&CLIENT_ID,"testUser@certivox.com");
	HASH_ID(&CLIENT_ID,&HCID);

	printf("Client ID= "); OCT_output_string(&CLIENT_ID); printf("\n");

	mapit2(&HCID,&X);

	ECP2_ZZZ_output(&X);

	BIG_XXX_rcopy(r,CURVE_Order_ZZZ);

	ECP2_ZZZ_mul(&X,r);

	ECP2_ZZZ_output(&X);

}
*/
