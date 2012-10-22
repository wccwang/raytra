#ifndef INTERSECTION_H_
#define INTERSECTION_H_

#include "basicmath.h"

class intersection{
	public:
		intersection():sf_id(-1), t(0.0f){}
		intersection(int SF_ID, float T, const myVector &N, const myVector &V, const myPoint &ITRPT)
			:sf_id(SF_ID), t(T), n(N), v(V), itrPt(ITRPT){}
	//private:
		int sf_id;
		float t;
		//float t2;
		myVector n;
		myVector v;
		myPoint itrPt;
};
#endif