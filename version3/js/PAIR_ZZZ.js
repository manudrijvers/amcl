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

var PAIR_ZZZ = {
/* Line function */
	line: function(A,B,Qx,Qy)
	{
		var P=new ECP2_ZZZ();
		var a,b,c;
		var r=new FP12_YYY(1);
		P.copy(A);

		var ZZ=new FP2_YYY(P.getz()); //ZZ.copy(P.getz());
		ZZ.sqr();
		var D;
		if (A==B) D=A.dbl(); 
		else D=A.add(B);
		if (D<0) return r;
		var Z3=new FP2_YYY(A.getz()); //Z3.copy(A.getz());
		c=new FP4_YYY(0);
		var X,Y,T;
		if (D===0)
		{ /* Addition */
			X=new FP2_YYY(B.getx()); //X.copy(B.getx());
			Y=new FP2_YYY(B.gety()); //Y.copy(B.gety());
			T=new FP2_YYY(P.getz()); //T.copy(P.getz());

			T.mul(Y);
			ZZ.mul(T);

			var NY=new FP2_YYY(P.gety()); /*NY.copy(P.gety());*/ NY.neg(); NY.norm();
			ZZ.add(NY); // ZZ.norm();
			Z3.pmul(Qy);
			T.mul(P.getx());
			X.mul(NY);
			T.add(X); T.norm();
			a=new FP4_YYY(Z3,T); //a.set(Z3,T);
			ZZ.neg(); ZZ.norm();
			ZZ.pmul(Qx);
			b=new FP4_YYY(ZZ); //b.seta(ZZ);
		}
		else
		{ /* Doubling */
			X=new FP2_YYY(P.getx()); //X.copy(P.getx());
			Y=new FP2_YYY(P.gety()); //Y.copy(P.gety());
			T=new FP2_YYY(P.getx()); //T.copy(P.getx());
			T.sqr();
			T.imul(3);

			Y.sqr();
			Y.add(Y);
			Z3.mul(ZZ);
			Z3.pmul(Qy);

			X.mul(T);
			X.sub(Y); X.norm();
			a=new FP4_YYY(Z3,X); //a.set(Z3,X);
			T.neg(); T.norm();
			ZZ.mul(T);

			ZZ.pmul(Qx);

			b=new FP4_YYY(ZZ); //b.seta(ZZ);
		}
		r.set(a,b,c);
		return r;		
	},

/* Optimal R-ate pairing */
	ate: function(P,Q)
	{
		var fa=new BIG_XXX(0); fa.rcopy(ROM_FIELD_YYY.Fra);
		var fb=new BIG_XXX(0); fb.rcopy(ROM_FIELD_YYY.Frb);
		var f=new FP2_YYY(fa,fb); //f.bset(fa,fb);
	
		var x=new BIG_XXX(0); x.rcopy(ROM_CURVE_ZZZ.CURVE_Bnx);
		var n=new BIG_XXX(x); //n.copy(x);
		var K=new ECP2_ZZZ();
		var lv;

		if (ECP_ZZZ.CURVE_PAIRING_TYPE==ECP_ZZZ.BN)
		{
			n.pmul(6); n.dec(2);
		}
		else
			n.copy(x);
		n.norm();

	//	P.affine();
	//	Q.affine();
		var Qx=new FP_YYY(Q.getx()); //Qx.copy(Q.getx());
		var Qy=new FP_YYY(Q.gety()); //Qy.copy(Q.gety());

		var A=new ECP2_ZZZ();
		var r=new FP12_YYY(1);

		A.copy(P);
		var nb=n.nbits();

		for (var i=nb-2;i>=1;i--)
		{
			lv=PAIR_ZZZ.line(A,A,Qx,Qy);

			r.smul(lv);

			if (n.bit(i)==1)
			{
				lv=PAIR_ZZZ.line(A,P,Qx,Qy);
				r.smul(lv);
			}
			r.sqr();
		}
		lv=PAIR_ZZZ.line(A,A,Qx,Qy);
		r.smul(lv);
		if (n.parity()==1)
		{
			lv=line(A,P,Qx,Qy);
			r.smul(lv);
		}

/* R-ate fixup */
		if (ECP_ZZZ.CURVE_PAIRING_TYPE==ECP_ZZZ.BN)
		{
			r.conj();
			K.copy(P);
			K.frob(f);
			A.neg();
			lv=PAIR_ZZZ.line(A,K,Qx,Qy);
			r.smul(lv);
			K.frob(f);
			K.neg();
			lv=PAIR_ZZZ.line(A,K,Qx,Qy);
			r.smul(lv);
		}
		return r;
	},

/* Optimal R-ate double pairing e(P,Q).e(R,S) */
	ate2: function(P,Q,R,S)
	{
		var fa=new BIG_XXX(0); fa.rcopy(ROM_FIELD_YYY.Fra);
		var fb=new BIG_XXX(0); fb.rcopy(ROM_FIELD_YYY.Frb);
		var f=new FP2_YYY(fa,fb); //f.bset(fa,fb);
		var x=new BIG_XXX(0); x.rcopy(ROM_CURVE_ZZZ.CURVE_Bnx);

		var n=new BIG_XXX(x); //n.copy(x);
		var K=new ECP2_ZZZ();
		var lv;

		if (ECP_ZZZ.CURVE_PAIRING_TYPE==ECP_ZZZ.BN)
		{
			n.pmul(6); n.dec(2);
		}
		else
			n.copy(x);
		n.norm();

	//	P.affine();
	//	Q.affine();
	//	R.affine();
	//	S.affine();

		var Qx=new FP_YYY(Q.getx()); //Qx.copy(Q.getx());
		var Qy=new FP_YYY(Q.gety()); //Qy.copy(Q.gety());

		var Sx=new FP_YYY(S.getx()); //Sx.copy(S.getx());
		var Sy=new FP_YYY(S.gety()); //Sy.copy(S.gety());

		var A=new ECP2_ZZZ();
		var B=new ECP2_ZZZ();
		var r=new FP12_YYY(1);

		A.copy(P);
		B.copy(R);
		var nb=n.nbits();

		for (var i=nb-2;i>=1;i--)
		{
			lv=PAIR_ZZZ.line(A,A,Qx,Qy);
			r.smul(lv);
			lv=PAIR_ZZZ.line(B,B,Sx,Sy);
			r.smul(lv);
			if (n.bit(i)==1)
			{
				lv=PAIR_ZZZ.line(A,P,Qx,Qy);
				r.smul(lv);
				lv=PAIR_ZZZ.line(B,R,Sx,Sy);
				r.smul(lv);
			}
			r.sqr();
		}

		lv=PAIR_ZZZ.line(A,A,Qx,Qy);
		r.smul(lv);
		lv=PAIR_ZZZ.line(B,B,Sx,Sy);
		r.smul(lv);
		if (n.parity()==1)
		{
			lv=line(A,P,Qx,Qy);
			r.smul(lv);
			lv=line(B,R,Sx,Sy);
			r.smul(lv);
		}
		
/* R-ate fixup required for BN curves */
		if (ECP_ZZZ.CURVE_PAIRING_TYPE==ECP_ZZZ.BN)
		{
			r.conj();

			K.copy(P);
			K.frob(f);
			A.neg();
			lv=PAIR_ZZZ.line(A,K,Qx,Qy);
			r.smul(lv);
			K.frob(f);
			K.neg();
			lv=PAIR_ZZZ.line(A,K,Qx,Qy);
			r.smul(lv);

			K.copy(R);
			K.frob(f);
			B.neg();
			lv=PAIR_ZZZ.line(B,K,Sx,Sy);
			r.smul(lv);
			K.frob(f);
			K.neg();
			lv=PAIR_ZZZ.line(B,K,Sx,Sy);
			r.smul(lv);
		}
		return r;
	},

/* final exponentiation - keep separate for multi-pairings and to avoid thrashing stack */
	fexp: function(m)
	{
		var fa=new BIG_XXX(0); fa.rcopy(ROM_FIELD_YYY.Fra);
		var fb=new BIG_XXX(0); fb.rcopy(ROM_FIELD_YYY.Frb);
		var f=new FP2_YYY(fa,fb);
		var x=new BIG_XXX(0); x.rcopy(ROM_CURVE_ZZZ.CURVE_Bnx);

		var r=new FP12_YYY(m); //r.copy(m);
	
/* Easy part of final exp */
		var lv=new FP12_YYY(r); //lv.copy(r);
		lv.inverse();
		r.conj();
		r.mul(lv);
		lv.copy(r);
		r.frob(f);	
		r.frob(f);
		r.mul(lv);

/* Hard part of final exp */
		if (ECP_ZZZ.CURVE_PAIRING_TYPE==ECP_ZZZ.BN)
		{
			var x0,x1,x2,x3,x4,x5;			
			lv.copy(r);
			lv.frob(f);
			x0=new FP12_YYY(lv); //x0.copy(lv);
			x0.frob(f);
			lv.mul(r);
			x0.mul(lv);
			x0.frob(f);
			x1=new FP12_YYY(r); //x1.copy(r);
			x1.conj();

			x4=r.pow(x);

			x3=new FP12_YYY(x4); //x3.copy(x4);
			x3.frob(f);
			x2=x4.pow(x);

			x5=new FP12_YYY(x2); /*x5.copy(x2);*/  x5.conj();
			lv=x2.pow(x);

			x2.frob(f);
			r.copy(x2); r.conj();

			x4.mul(r);
			x2.frob(f);

			r.copy(lv);
			r.frob(f);
			lv.mul(r);

			lv.usqr();
			lv.mul(x4);
			lv.mul(x5);
			r.copy(x3);
			r.mul(x5);
			r.mul(lv);
			lv.mul(x2);
			r.usqr();
			r.mul(lv);
			r.usqr();
			lv.copy(r);
			lv.mul(x1);
			r.mul(x0);
			lv.usqr();
			r.mul(lv);
			r.reduce();
		}
		else
		{
			var y0,y1,y2,y3;
// Ghamman & Fouotsa Method
			y0=new FP12_YYY(r); y0.usqr();
			y1=y0.pow(x);
			x.fshr(1); y2=y1.pow(x); x.fshl(1);
			y3=new FP12_YYY(r); y3.conj();
			y1.mul(y3);

			y1.conj();
			y1.mul(y2);

			y2=y1.pow(x);

			y3=y2.pow(x);
			y1.conj();
			y3.mul(y1);

			y1.conj();
			y1.frob(f); y1.frob(f); y1.frob(f);
			y2.frob(f); y2.frob(f);
			y1.mul(y2);

			y2=y3.pow(x);
			y2.mul(y0);
			y2.mul(r);

			y1.mul(y2);
			y2.copy(y3); y2.frob(f);
			y1.mul(y2);
			r.copy(y1);
			r.reduce();


/*
			x0=new FP12_YYY(r);
			x1=new FP12_YYY(r);
			lv.copy(r); lv.frob(f);
			x3=new FP12_YYY(lv); x3.conj(); x1.mul(x3);
			lv.frob(f); lv.frob(f);
			x1.mul(lv);

			r.copy(r.pow(x));  //r=r.pow(x);
			x3.copy(r); x3.conj(); x1.mul(x3);
			lv.copy(r); lv.frob(f);
			x0.mul(lv);
			lv.frob(f);
			x1.mul(lv);
			lv.frob(f);
			x3.copy(lv); x3.conj(); x0.mul(x3);

			r.copy(r.pow(x));
			x0.mul(r);
			lv.copy(r); lv.frob(f); lv.frob(f);
			x3.copy(lv); x3.conj(); x0.mul(x3);
			lv.frob(f);
			x1.mul(lv);

			r.copy(r.pow(x));
			lv.copy(r); lv.frob(f);
			x3.copy(lv); x3.conj(); x0.mul(x3);
			lv.frob(f);
			x1.mul(lv);

			r.copy(r.pow(x));
			x3.copy(r); x3.conj(); x0.mul(x3);
			lv.copy(r); lv.frob(f);
			x1.mul(lv);

			r.copy(r.pow(x));
			x1.mul(r);

			x0.usqr();
			x0.mul(x1);
			r.copy(x0);
			r.reduce(); */
		}
		return r;
	}
};

/* GLV method */
PAIR_ZZZ.glv= function(e)
{
	var u=[];
	if (ECP_ZZZ.CURVE_PAIRING_TYPE==ECP_ZZZ.BN)
	{
		var i,j;
		var t=new BIG_XXX(0);
		var q=new BIG_XXX(0); q.rcopy(ROM_CURVE_ZZZ.CURVE_Order);
		var v=[];

		for (i=0;i<2;i++)
		{
			t.rcopy(ROM_CURVE_ZZZ.CURVE_W[i]);
			var d=BIG_XXX.mul(t,e);
			v[i]=new BIG_XXX(d.div(q));
			u[i]=new BIG_XXX(0);
		}
		u[0].copy(e);
		for (i=0;i<2;i++)
			for (j=0;j<2;j++)
			{
				t.rcopy(ROM_CURVE_ZZZ.CURVE_SB[j][i]);
				t.copy(BIG_XXX.modmul(v[j],t,q));
				u[i].add(q);
				u[i].sub(t);
				u[i].mod(q);
			}
	}
	else
	{ // -(x^2).P = (Beta.x,y)
		var q=new BIG_XXX(0); q.rcopy(ROM_CURVE_ZZZ.CURVE_Order);
		var x=new BIG_XXX(0); x.rcopy(ROM_CURVE_ZZZ.CURVE_Bnx);
		var x2=BIG_XXX.smul(x,x);
		u[0]=new BIG_XXX(e);
		u[0].mod(x2);
		u[1]=new BIG_XXX(e);
		u[1].div(x2);
		u[1].rsub(q);
	}
	return u;
};

/* Galbraith & Scott Method */
PAIR_ZZZ.gs= function(e)
{
	var u=[];
	if (ECP_ZZZ.CURVE_PAIRING_TYPE==ECP_ZZZ.BN)
	{
		var i,j;
		var t=new BIG_XXX(0);
		var q=new BIG_XXX(0); q.rcopy(ROM_CURVE_ZZZ.CURVE_Order);

		var v=[];

		for (i=0;i<4;i++)
		{
			t.rcopy(ROM_CURVE_ZZZ.CURVE_WB[i]);
			var d=BIG_XXX.mul(t,e);
			v[i]=new BIG_XXX(d.div(q));
			u[i]=new BIG_XXX(0);
		}

		u[0].copy(e);
		for (i=0;i<4;i++)
			for (j=0;j<4;j++)
			{
				t.rcopy(ROM_CURVE_ZZZ.CURVE_BB[j][i]);
				t.copy(BIG_XXX.modmul(v[j],t,q));
				u[i].add(q);
				u[i].sub(t);
				u[i].mod(q);
			}
	}
	else
	{
		var x=new BIG_XXX(0); x.rcopy(ROM_CURVE_ZZZ.CURVE_Bnx);
		var w=new BIG_XXX(e);
		for (var i=0;i<3;i++)
		{
			u[i]=new BIG_XXX(w);
			u[i].mod(x);
			w.div(x);
		}
		u[3]=new BIG_XXX(w);
	}
	return u;
};	

/* Multiply P by e in group G1 */
PAIR_ZZZ.G1mul= function(P,e)
{
	var R;
	if (ROM_CURVE_ZZZ.USE_GLV)
	{
		P.affine();
		R=new ECP_ZZZ();
		R.copy(P);
		var np,nn;
		var Q=new ECP_ZZZ();
		Q.copy(P);
		var q=new BIG_XXX(0); q.rcopy(ROM_CURVE_ZZZ.CURVE_Order);
		var bcru=new BIG_XXX(0); bcru.rcopy(ROM_CURVE_ZZZ.CURVE_Cru);
		var cru=new FP_YYY(bcru);
		var t=new BIG_XXX(0);
		var u=PAIR_ZZZ.glv(e);

		Q.getx().mul(cru);

		np=u[0].nbits();
		t.copy(BIG_XXX.modneg(u[0],q));
		nn=t.nbits();
		if (nn<np)
		{
			u[0].copy(t);
			R.neg();
		}

		np=u[1].nbits();
		t.copy(BIG_XXX.modneg(u[1],q));
		nn=t.nbits();
		if (nn<np)
		{
			u[1].copy(t);
			Q.neg();
		}

		R=R.mul2(u[0],Q,u[1]);
			
	}
	else
	{
		R=P.mul(e);
	}
	return R;
};

/* Multiply P by e in group G2 */
PAIR_ZZZ.G2mul= function(P,e)
{
	var R;
	if (ROM_CURVE_ZZZ.USE_GS_G2)
	{
		var Q=[];
		var fa=new BIG_XXX(0); fa.rcopy(ROM_FIELD_YYY.Fra);
		var fb=new BIG_XXX(0); fb.rcopy(ROM_FIELD_YYY.Frb);
		var f=new FP2_YYY(fa,fb); //f.bset(fa,fb);
		var q=new BIG_XXX(0); q.rcopy(ROM_CURVE_ZZZ.CURVE_Order);

		var u=PAIR_ZZZ.gs(e);
		var t=new BIG_XXX(0);
		var i,np,nn;
		P.affine();
		Q[0]=new ECP2_ZZZ(); Q[0].copy(P);
		for (i=1;i<4;i++)
		{
			Q[i]=new ECP2_ZZZ(); Q[i].copy(Q[i-1]);
			Q[i].frob(f);
		}

		for (i=0;i<4;i++)
		{
			np=u[i].nbits();
			t.copy(BIG_XXX.modneg(u[i],q));
			nn=t.nbits();
			if (nn<np)
			{
				u[i].copy(t);
				Q[i].neg();
			}
		}

		R=ECP2_ZZZ.mul4(Q,u);
	}
	else
	{
		R=P.mul(e);
	}
	return R;
};

/* Note that this method requires a lot of RAM! Better to use compressed XTR method, see FP4_YYY.js */
PAIR_ZZZ.GTpow= function(d,e)
{
	var r;
	if (ROM_CURVE_ZZZ.USE_GS_GT)
	{
		var g=[];
		var fa=new BIG_XXX(0); fa.rcopy(ROM_FIELD_YYY.Fra);
		var fb=new BIG_XXX(0); fb.rcopy(ROM_FIELD_YYY.Frb);
		var f=new FP2_YYY(fa,fb);
		var q=new BIG_XXX(0); q.rcopy(ROM_CURVE_ZZZ.CURVE_Order);
		var t=new BIG_XXX(0);
		var i,np,nn;
		var u=PAIR_ZZZ.gs(e);

		g[0]=new FP12_YYY(d);
		for (i=1;i<4;i++)
		{
			g[i]=new FP12_YYY(0); g[i].copy(g[i-1]);
			g[i].frob(f);
		}
		for (i=0;i<4;i++)
		{
			np=u[i].nbits();
			t.copy(BIG_XXX.modneg(u[i],q));
			nn=t.nbits();
			if (nn<np)
			{
				u[i].copy(t);
				g[i].conj();
			}
		}
		r=FP12_YYY.pow4(g,u);
	}
	else
	{
		r=d.pow(e);
	}
	return r;
};

/* test group membership - no longer needed */
/* with GT-Strong curve, now only check that m!=1, conj(m)*m==1, and m.m^{p^4}=m^{p^2} */
/*
PAIR_ZZZ.GTmember= function(m)
{
	if (m.isunity()) return false;
	var r=new FP12_YYY(m);
	r.conj();
	r.mul(m);
	if (!r.isunity()) return false;

	var fa=new BIG_XXX(0); fa.rcopy(ROM_FIELD_YYY.Fra);
	var fb=new BIG_XXX(0); fb.rcopy(ROM_FIELD_YYY.Frb);
	var f=new FP2_YYY(fa,fb); //f.bset(fa,fb);

	r.copy(m); r.frob(f); r.frob(f);
	var w=new FP12_YYY(r); w.frob(f); w.frob(f);
	w.mul(m);
	if (!ROM_CURVE_ZZZ.GT_STRONG)
	{
		if (!w.equals(r)) return false;
		var x=new BIG_XXX(0); x.rcopy(ROM_CURVE_ZZZ.CURVE_Bnx);
		r.copy(m); w=r.pow(x); w=w.pow(x);
		r.copy(w); r.sqr(); r.mul(w); r.sqr();
		w.copy(m); w.frob(f);
	}
	return w.equals(r);
};
*/
