#include "curves/circle.h"

using namespace glib;
using namespace std;

circle::circle(point center, glib_float radius) :
  _center(center),
  _radius(radius) {
}


list<moved_arrays> 
circle::get_arrays() {

	std::list<moved_arrays> res;
	
	if (_radius <= 0.5) {
		moved_arrays one(_center.y, _center.y);
		one.set(_center.x, _center.y);
		
		res.push_back(one);
	}
	else if (_radius <= 1) {
		moved_arrays small(_center.y-1, _center.y+1);
		
		small.set(_center.x, _center.y);
		small.set(_center.x+1, _center.y);
		small.set(_center.x-1, _center.y);
		small.set(_center.x, _center.y+1);
		small.set(_center.x, _center.y-1);
		
		res.push_back(small);
	} else {
	
		glib_float lx = 0;
		glib_float ly = _radius;
		glib_float d = 1 - ly;
	
		moved_arrays left(_center.y-_radius, _center.y+_radius);
		moved_arrays right(_center.y-_radius, _center.y+_radius);
	
	
		paint_more(0,ly,left,right);
		while (ly>lx) {
			if (d<0) {
				d = d + 2*lx + 3;
			} else {
				d = d + 2*(lx-ly) + 5;
				--ly;
			}
			++lx;
			paint_more(lx,ly,left,right);
		} //^^ algoritmus ze slidu :)
		
		res.push_back(left);
		res.push_back(right);
	}
	return res;
}


void 
circle::paint_more(glib_float x, glib_float y, moved_arrays& left, moved_arrays& right){
	left.set(_center.x-x, _center.y+y);
	left.set(_center.x-y, _center.y+x);
	left.set(_center.x-x, _center.y-y);
	left.set(_center.x-y, _center.y-x);
	
	right.set(_center.x+x, _center.y+y);
	right.set(_center.x+y, _center.y+x);
	right.set(_center.x+x, _center.y-y);
	right.set(_center.x+y, _center.y-x);
}

glib_int 
circle::get_min_y() const {
	return _center.y - _radius;
}

glib_int 
circle::get_max_y() const {
	return _center.y + _radius;
}


glib_int 
circle::get_min_x() const {
	return _center.x - _radius;
}

glib_int 
circle::get_max_x() const {
	return _center.x + _radius;
}

gr_object* 
circle::get_thick_line(const glib_float thickness, const curve* const previous, const curve* const next) {
	throw 1;
	return NULL;
}
