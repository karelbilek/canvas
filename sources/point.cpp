#include "point.h"
#include <cmath>

using namespace libcan;

//-------------------------CONSTRUCTORS
point::point(libcan_float new_x, libcan_float new_y):x(new_x),y(new_y) {}
point::point(libcan_int new_x, libcan_int new_y):x(new_x),y(new_y) {}
point::point(int new_x, int new_y):x(new_x),y(new_y) {}


point::point():x(0), y(0) { }

//-------------------------OTHER
point
point::move(const libcan_float distance, libcan_float degrees) const{
	while (degrees<0) {
		degrees+=360;
	}
	while (degrees>=360) {
		degrees-=360;
	}	//modulo, ale na cislech, kde mi to c++ nedovoli
	
	libcan_float rad = __DEG2RAD(degrees);
	return point(x+distance*cos(rad), y+distance*sin(rad));
}



point
point::operator+(const point& other) const{
	return point(x+other.x, y+other.y);
}

point 
point::operator-(const point& other) const {
	return point(x-other.x, y-other.y);
}

point 
point::operator*(const libcan_float n) const {
	return point(x*n, y*n);
}

point 
point::operator*(const libcan_int n) const {
	return point(x*n, y*n);
}

point 
point::operator*(const int n) const {
	return point(x*n, y*n);
}
point
point::trunc() const {
	return point(static_cast<libcan_int>(x), static_cast<libcan_int>(y));
}

point& 
point::operator=(const point& other) {
	x=other.x;
	y=other.y; 
	return *this;
}

bool 
point::operator==(const point& other) const {
	return (__abs(x - other.x)<0.01 && __abs(y - other.y)<0.01);
}

bool 
point::operator!=(const point& other) const {
	return (__abs(x - other.x)>=0.01 || __abs(y - other.y)>=0.01);
}


point 
point::distance_from(const point& other) const {
	return point(other.x-x,other.y-y);
}

