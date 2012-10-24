#ifndef BASICMATH_H_
#define BASICMATH_H_

#include<vector>

class myVector{
	public:
		myVector(float x=0.0f, float y=0.0f, float z=0.0f):_x(x), _y(y), _z(z){}
		float dot(const myVector& rhs);
		myVector cross(const myVector&);
		myVector getUnitVec() const;
		float getNorm() const;
		myVector& operator+=(const myVector &vec);
		const myVector operator+(const myVector &vec) const;
		myVector& operator-=(const myVector &vec);
		const myVector operator-(const myVector &vec) const;
		myVector& operator*=(const float scale);
		const myVector operator*(const float scale);
		myVector& operator/=(const float scale);
		const myVector operator/(const float scale);
		const myVector operator*(const myVector &vec);
		float operator[](const int i) const;
		friend class myPoint;
		friend class camera;
		friend void printVector(myVector vec);
	private:
		float _x;
		float _y;
		float _z;
}; 
class myPoint{
	public:
		myPoint(float x=0.0f, float y=0.0f, float z=0.0f):_x(x), _y(y), _z(z){}
		myPoint& operator+=(const myVector &vec);
		myPoint& operator-=(const myVector &vec);
		const myPoint operator+(const myVector &vec);
		const myPoint operator-(const myVector &vec);
		const myVector operator-(const myPoint &pt) const;
		float operator[](const int i) const;
		friend void printPoint(myPoint pt);
	private:
		float _x; 
		float _y;
		float _z;
};

#endif