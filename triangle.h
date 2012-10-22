#ifndef TRIANGLE_H_
#define TRIANGLE_H_
#include"surface.h"

class triangle : public surface{
	public:
		triangle(const myPoint &P1, const myPoint &P2, const myPoint &P3):p1(P1), p2(P2), p3(P3){
			myVector ab = p2-p1;
			myVector ac = p3-p1;
			normal = ab.cross(ac);
			normal = normal.getUnitVec();
		}
		bool intersect(ray* , int);
	private:
		myPoint p1;
		myPoint p2;
		myPoint p3;
		myVector normal;
};
#endif