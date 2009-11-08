#include "curves/bezier.h"

using namespace glib;
using namespace std;

bezier::bezier(point a, point b, point c, point d):
  _a(a),
  _b(b),
  _c(c),
  _d(d) {
}



list<moved_arrays> 
bezier::get_arrays() {
	//zdroj algoritmu - http://www.niksula.cs.hut.fi/~hkankaan/Homepages/bezierfast.html
	glib_int min_y = get_min_y();
	glib_int max_y = get_max_y();
	
	glib_uint steps = __maximum((max_y - min_y), (get_max_x()-get_min_x()))*5;
	
	list<moved_arrays> res;
	moved_arrays ma(min_y, max_y);
	
	bool up; //jestli zrovna BK jede smerem up
	bool first = true; //1 jen na zacatku
	
	point f;
	point fd, fdd, fddd, fdd_per_2, fddd_per_2, fddd_per_6;
	glib_float t = static_cast<glib_float>(1.0 / steps);
	glib_float temp = t * t;
	
			//silene derivace.... 
			//priznam se, ze tomu algoritmu az tak nerozumim, ale funguje :)
	f = _a;
	fd = (_b - _a) * 3 * t;
	fdd_per_2 = (_a - _b * 2 + _c) * 3 * temp;
	fddd_per_2 = ((_b - _c) * 3 + _d - _a) * 3 * temp * t;
	
	fddd = fddd_per_2 + fddd_per_2;
	fdd = fdd_per_2 + fdd_per_2;
	fddd_per_6 = fddd_per_2 * (1.0 / 3);
	
	for (glib_uint i=0; i < steps; ++i) {
		ma.set(f.x,f.y);
		
		if (first) {
			up = ((fd + fdd_per_2 + fddd_per_6).y > 0);
			first = false;
				//prvni smer je dulezity.
			
		} else if (up) {
			if ((fd + fdd_per_2 + fddd_per_6).y < 0) {
				res.push_back(ma);
					//plivu dalsi
				ma = moved_arrays(min_y,max_y);
				ma.set(f.x, f.y);
				
				up = false;
			}
		} else if (!up) {
			if ((fd + fdd_per_2 + fddd_per_6).y > 0) {
				res.push_back(ma);
					//plivu dalsi
					
				ma = moved_arrays(min_y,max_y);
				ma.set(f.x, f.y);
				
				up = true;
			}
		}
		
		
			//IT'S MAGIC, WE DON'T HAVE TO EXPLAIN IT! 
			//						--Joe Quesada
		
		f = f + fd + fdd_per_2 + fddd_per_6;
		fd = fd + fdd + fddd_per_2;
		fdd = fdd + fddd;
		fdd_per_2 = fdd_per_2 + fddd_per_2;
	}
	
	ma.set(_d.x,_d.y);
	res.push_back(ma);
	
	
	return res;
	
}


glib_int 
bezier::get_min_x() const {
	return static_cast<glib_int>(__minimum(_a.x, __minimum(_b.x, __minimum(_c.x, _d.x))));
}

glib_int 
bezier::get_max_x() const{
	return static_cast<glib_int>(__maximum(_a.x, __maximum(_b.x, __maximum(_c.x, _d.x))));
}

glib_int 
bezier::get_min_y() const {
	return static_cast<glib_int>(__minimum(_a.y, __minimum(_b.y, __minimum(_c.y, _d.y))));
}

glib_int
bezier::get_max_y() const{
	return static_cast<glib_int> (__maximum(_a.y, __maximum(_b.y, __maximum(_c.y, _d.y))));
}