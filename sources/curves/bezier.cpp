#include "curves/bezier.h"

using namespace canlib;
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
	//(C) Hannu Kankaanpää
	
	canlib_int min_y = get_min_y();
	canlib_int max_y = get_max_y();
	
	canlib_uint steps = __maximum((max_y - min_y), (get_max_x()-get_min_x()))*5;
			//!!!!!!ta 5ka je umele dodana konstanta
			//mozna by to slo zvetsit/zmensit? ale me se to zda OK
	
	list<moved_arrays> res;
	moved_arrays ma(min_y, max_y);
			//tohle je samozrejme "navic" nez v tom algoritmu nahore
	
	bool up; //jestli zrovna BK jede smerem up
	bool first = true; //1 jen na zacatku
	
	point f;
	point fd, fdd, fddd, fdd_per_2, fddd_per_2, fddd_per_6;
	canlib_float t = static_cast<canlib_float>(1.0 / steps);
	canlib_float temp = t * t;
	
			//silene derivace.... 
			
	f = _a;
	fd = (_b - _a) * 3 * t;
	fdd_per_2 = (_a - _b * 2 + _c) * 3 * temp;
	fddd_per_2 = ((_b - _c) * 3 + _d - _a) * 3 * temp * t;
	
	fddd = fddd_per_2 + fddd_per_2;
	fdd = fdd_per_2 + fdd_per_2;
	fddd_per_6 = fddd_per_2 * (1.0 / 3);
	
	for (canlib_uint i=0; i < steps; ++i) {
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


canlib_int 
bezier::get_min_x() const {
	return static_cast<canlib_int>(__minimum4(_a.x,_b.x,_c.x, _d.x));
	//(b<((c<d)?(c):(d)))?(b):((c<d)?(c):(d))
}

canlib_int 
bezier::get_max_x() const{
	return static_cast<canlib_int>(__maximum4(_a.x, _b.x, _c.x, _d.x)+2);
}

canlib_int 
bezier::get_min_y() const {
	return static_cast<canlib_int>(__minimum4(_a.y,_b.y, _c.y, _d.y));
}

canlib_int
bezier::get_max_y() const{
	return static_cast<canlib_int> (__maximum4(_a.y, _b.y, _c.y, _d.y)+2);
}