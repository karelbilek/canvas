#include "circle.h"

using namespace glib;
using namespace std;

circle::circle(point center, glib_float radius) :
  _center(center),
  _radius(radius) {
}

circle::
const list<const moved_arrays> get_arrays() {

	list<const moved_arrays> res;
	
	if (_radius <= 0.5) {
		moved_arrays one(_center.y, _center.y);
		one.set(_center.x, _center.y);
		
		res.push(one);
	}
	else if (_radius <= 1) {
		moved_arrays small(_center.y-1, _center.y+1);
		
		small.set(_center.x, _center.y);
		small.set(_center.x+1, _center.y);
		small.set(_center.x-1, _center.y);
		small.set(_center.x, _center.y+1);
		small.set(_center.x, _center.y-1);
		
		res.push(small);
	} else {
	
		glib_float lx = 0;
		glib_float ly = _radius;
		glib_float d = 1 - ly;
	
		moved_arrays left(_center.y-_radius, _center.y+_radius);
		moved_arrays right(_center.y-_radius, _center.y+_radius);
	
	
		paint4(0,ly,left,right);
		while (ly>lx) {
			if (d<0) {
				d = d + 2*lx + 3;
			} else {
				d = d + 2*(lx-ly) + 5;
				--ly;
			}
			++lx;
			paint4(lx,ly,left,right);
		} //^^ algoritmus ze slidu :)
		
		res.push(left);
		res.push(right);
	}
	return res;
}

circle::
void paint_more(glib_float x, glib_float y, moved_arrays& left, moved_arrays& right){
	left.set(_center.x-x, _center.y+y);
	left.set(_center.x-y, _center.y+x);
	left.set(_center.x-x, _center.y-y);
	left.set(_center.x-y, _center.y-x);
	
	right.set(_center.x+x, _center.y+y);
	right.set(_center.x+y, _center.y+x);
	right.set(_center.x+x, _center.y-y);
	right.set(_center.x+y, _center.y-x);
}

circle::
glib_int get_min_y() const {
	return _center.y - radius;
}

circle::
glib_int get_max_y() const {
	return _center.y + radius;
}

circle::
glib_int get_min_x() const {
	return _center.x - radius;
}

circle::
glib_int get_max_x() const {
	return _center.x + radius;
}
