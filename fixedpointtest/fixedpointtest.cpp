// fixedpointtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <stdint.h>

class cfixp{
	unsigned char q;
	signed long val;
public:
	cfixp& operator+(cfixp& v) {
		cfixp t;
		
		t.val = val+v.val;
		return t;
	}

	cfixp& operator-(cfixp& v) {
		cfixp t;
		
		t.val = val-v.val;
		return t;
	}

	cfixp& operator*(cfixp& v) {	
		cfixp t;
		signed long long tmp;
		tmp = (val*(signed long long)v.val) + (1 << (q - 1));
		tmp >>= q;			

		
		t.val = tmp;		
		return t;
	}

	cfixp& operator/(cfixp& v){
		cfixp t;	
		signed long long tmp;
		tmp = ((signed long long)val)<<q;
		
		t.val = tmp/v.val;

		return t;
	}

	void setval(float a) {
		val = a * (1<<q);
	}

	float getval(void) {
		return val/(float)(1<<q);
	}
	void print(void) {
		printf("%f\n",getval());
	}
	cfixp(float v=0,char qq=28):q(qq){
		setval(v);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	cfixp a(3.1415);
	cfixp b(2.0);
	cfixp c(1.0);

	printf(" a+b = ");
	c = a+b;
	c.print();

	printf(" a-b = ");
	c = a-b;
	c.print();

	printf(" a*b = ");
	c = a*b;
	c.print();

	printf(" a/b = ");
	c = a/b;
	c.print();

	return 0;
}


