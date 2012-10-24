#include<cmath>
#include<iostream>
#include<vector>
#include"basicmath.h"

using namespace std;


float myVector::dot(const myVector& rhs){
	return _x*rhs._x+_y*rhs._y+_z*rhs._z;
}

myVector myVector::cross(const myVector &b){
	float x = _y*b._z - _z*b._y;
	float y = _z*b._x - _x*b._z;
	float z = _x*b._y - _y*b._x;
	return myVector(x, y, z);
}

myVector myVector::getUnitVec() const{
	float norm = sqrt(_x*_x+_y*_y+_z*_z);
	return myVector(_x/norm, _y/norm, _z/norm);
}

float myVector::getNorm() const{
	return sqrt(_x*_x+_y*_y+_z*_z);
}

myVector& myVector::operator+=(const myVector &vec){
	_x += vec._x;
	_y += vec._y;
	_z += vec._z;
	return (*this);
}
const myVector myVector::operator+(const myVector &vec) const{
	return myVector(*this)+=vec;
}
myVector& myVector::operator-=(const myVector &vec){
	_x -= vec._x;
	_y -= vec._y;
	_z -= vec._z;
	return (*this);
}
const myVector myVector::operator-(const myVector &vec) const{
	return myVector(*this)-=vec;
}
myVector& myVector::operator*=(const float scale){
	_x *= scale;
	_y *= scale;
	_z *= scale;
	return (*this);
}

const myVector myVector::operator*(const float scale){
	return myVector(*this) *= scale;
}

myVector& myVector::operator/=(const float scale){
	_x /= scale;
	_y /= scale;
	_z /= scale;
	return (*this);
}

const myVector myVector::operator/(const float scale){
	return myVector(*this) /= scale;
}

const myVector myVector::operator*(const myVector &vec){
	float x = _x*vec._x;
	float y = _y*vec._y;
	float z  = _z*vec._z;
	return myVector(x,y,z);
}

float myVector::operator[](const int i) const{
	if(i==0) return _x;
	else if(i==1) return _y;
	else if(i==2) return _z;
	else{
		cerr<<"Vector index error!" << endl;
		return 0;
	}
}

myPoint& myPoint::operator+=(const myVector &vec){
	_x = _x+vec._x;
	_y = _y+vec._y;
	_z = _z+vec._z;
	return (*this);
}
myPoint& myPoint::operator-=(const myVector &vec){
	_x = _x-vec._x;
	_y = _y-vec._y;
	_z = _z-vec._z;
	return (*this);
}
const myPoint myPoint::operator+(const myVector &vec){
	return myPoint(*this)+=vec;
}
const myPoint myPoint::operator-(const myVector &vec){
	return myPoint(*this)-=vec;
}
const myVector myPoint::operator-(const myPoint &pt) const{
	float fx = _x - pt._x;
	float fy = _y - pt._y;
	float fz = _z - pt._z;
	return myVector(fx, fy,fz);
}
float myPoint::operator[](const int i) const{
	if(i==0) return _x;
	else if(i==1) return _y;
	else if(i==2) return _z;
	else{
		cerr<<"Point index error!" << endl;
		return 0;
	}
}

void printPoint(myPoint pt){
	cout<<pt._x<<" "<<pt._y << " " << pt._z << endl;
}

void printVector(myVector vec){
	std::cout << vec._x << " " << vec._y << " " << vec._z << std::endl;
}
