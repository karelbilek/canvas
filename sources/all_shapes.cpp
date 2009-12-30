#include "all_shapes.h"
using namespace canlib;

disk::disk(const point& center, const canlib_float radius): shape_type(1,1) {
	_curves.push_back(new circle(center, radius));
}
	
segment::segment(const canlib_float ax, const canlib_float ay, const canlib_float bx, const canlib_float by): shape_type(0,0) {
	_curves.push_back(new line(point(ax,ay),point(bx,by)));
} 
	
bezier_segment::bezier_segment(const point& a, const point& b, const point& c, const point& d): shape_type(0,0) {
	_curves.push_back(new bezier(a,b,c,d));
} 

polygon::polygon(const point& a, const point& b, const point&c):shape_type(1,1) {
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,a));
}
		
polygon::polygon(const point& a, const point& b, const point& c, const point& d):shape_type(1,1) {
	
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,a));
}

polygon::polygon(const point& a, const point& b, const point& c, const point& d, const point& e):shape_type(1,1) {
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,e));
	_curves.push_back(new line(e,a));
}

polygon::polygon(const point& a, const point& b, const point& c, const point& d, const point& e, const point& f):shape_type(1,1) {
	_curves.push_back(new line(a,b));
	_curves.push_back(new line(b,c));
	_curves.push_back(new line(c,d));
	_curves.push_back(new line(d,e));
	_curves.push_back(new line(e,f));
	_curves.push_back(new line(f,a));
}

polygon::polygon(const std::vector<point>& points):shape_type(1,1) {
	
	std::vector<point>::const_iterator i = points.begin();
	
	point a = *i;
	point first=a;
	
	++i;
	
	for (; i!=points.end(); ++i) {
		point b = *i;
		_curves.push_back(new line(a,b));
		a=b;
	}
	_curves.push_back(new line(a,first));
	
}
