
package com.ibm.zurich.amcl.RSA4096;

public final class public_key
{
    public int e;
    public FF n;

	public public_key(int m)
	{
		e=0;
		n=new FF(m);
	}
}
