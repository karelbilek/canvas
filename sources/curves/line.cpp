#include "curves/line.h"

using namespace glib;
using namespace std;

line::line(point a, point b) :
  _width_bigger( __abs(a.x-b.x) > __abs(a.y-b.y) ),
  _switched(_width_bigger?(b.x>a.x):(b.y>a.y)),
  _a( _switched ? a.trunc() : b.trunc()) ,
  _b( _switched ? b.trunc() : a.trunc() ),
  _increasing( _width_bigger ? ((_b.y-_a.y)>0) : ((_b.x-_a.x)>0)){
}

list<moved_arrays>
line::get_arrays() {
	
	moved_arrays res(__minimum(_a.y,_b.y),__maximum(_a.y, _b.y));
	
	glib_int lower_size = static_cast<glib_int>(_width_bigger?__abs(_a.y-_b.y):__abs(_a.x-_b.x));
	glib_int bigger_size = static_cast<glib_int>(_width_bigger?__abs(_a.x-_b.x):__abs(_a.y-_b.y));
	
	
	glib_int D = 2*lower_size - bigger_size;
	glib_int inc0 = 2*lower_size;
	glib_int inc1 = 2*(lower_size - bigger_size);
	
	
	point moving = _a;
	res.set(moving.x, moving.y);
	
	glib_float& moving_bigger = _width_bigger?moving.x:moving.y;
	glib_float& moving_lower = _width_bigger?moving.y:moving.x;
	

	
	for (glib_int i = 0; i < bigger_size; ++i) {
		if (D<=0){
			D+=inc0;
		} else {
			D+=inc1;
			moving_lower += _increasing?(1):(-1);
		}
		++moving_bigger;
		res.set(moving.x, moving.y);
	}
	res.set(_b.x, _b.y);
	
	list<moved_arrays> res2;
	res2.push_back(res);
	
	return res2;
	
}

glib_int line::get_min_y() const {return __minimum(_a.y,_b.y);}
glib_int line::get_max_y() const {return __maximum(_a.y,_b.y);}

glib_int line::get_min_x() const {return __minimum(_a.x,_b.x);}
glib_int line::get_max_x() const {return __maximum(_a.x,_b.x);}

