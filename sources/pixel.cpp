#include "pixel.h"
#include <cmath>

using namespace glib;


pix::pix(floating _x, floating _y):x(_x),y(_y) {}
pix::pix(integer _x, integer _y):x(_x),y(_y) {}
pix::pix(int _x, int _y):x(_x),y(_y) {}

pix::pix(double _x, double _y):x(static_cast<floating>(_x)),y(static_cast<floating>(_y)) {}

pix::pix():x(0), y(0) { }


pix
pix::move(floating distance, floating degrees) {
	while (degrees < 0) {
		degrees+=360;
	}
	while (degrees > 360) {
		degrees = degrees-360;
	}
	floating rad = DEG2RAD(degrees);
	return pix(x+distance*cos(rad), y+distance*sin(rad));
}

pix
pix::operator+(const pix& other) const{
	return pix(x+other.x, y+other.y);
}

pix 
pix::operator-(const pix& other) const {
	return pix(x-other.x, y-other.y);
}

pix 
pix::operator*(const floating n) const {
	return pix(x*n, y*n);
}

pix 
pix::operator*(const integer n) const {
	return pix(x*n, y*n);
}

pix 
pix::operator*(const int n) const {
	return pix(x*n, y*n);
}

pix 
pix::operator*(const double n) const {
	return pix(x*n, y*n);
}

pix& 
pix::operator=(const pix& other) {
	x=other.x;
	y=other.y; 
	return *this;
}

bool 
pix::operator==(const pix& other) const {
	return (__abs2(x - other.x)<0.01 && __abs2(y - other.y)<0.01);
}

bool 
pix::operator!=(const pix& other) const {
	return (__abs2(x - other.x)>=0.01 || __abs2(y - other.y)>=0.01);
}

bool 
pix::smaller_deviation(const pix& other) const {
	floating my_x_dev = x-static_cast<integer> (x);
	floating my_y_dev = y-static_cast<integer> (y);
	floating other_x_dev = other.x-static_cast<integer> (other.x);
	floating other_y_dev = other.y-static_cast<integer> (other.y);
	if (__abs2(my_x_dev-other_x_dev)<0.01 && __abs2(my_y_dev-other_y_dev)<0.01 ) 
		return false;
	
	if (my_x_dev<other_x_dev) return true;
	if (my_x_dev>other_x_dev) return false;
	if (my_y_dev<other_y_dev) return true;
	if (my_y_dev>other_y_dev) return false;
	return false;
}

pix 
pix::distance_from(const pix& other) const {
	return pix(other.x-x,other.y-y);
}

pix 
curve_def::start() const {
	return _a;
}

pix 
curve_def::end() const {
	if (_is_bezier)
		return _d;
	else
		return _b;
}

curve_def::curve_def(const pix& a, const pix& b, const pix& c, const pix& d) : _a(a), _b(b), _c(c), _d(d), _is_bezier(true) { }

curve_def::curve_def(const pix& a, const pix& b):_a(a), _b(b), _c(), _d(), _is_bezier(false) { }
