#include "curves/bezier.h"
#include "geom_line.h"

using namespace libcan;
using namespace std;

bezier::bezier(point a, point b, point c, point d):
  _a(a),
  _b(b),
  _c(c),
  _d(d) {
}

bezier* 
bezier::clone() const {bezier* n= new bezier(_a,_b,_c,_d);return n;}

bezier* 
bezier::clone_double() const {bezier* n= new bezier(_a*2,_b*2,_c*2,_d*2);return n;}

bool 
bezier::have_thick_line() const {return 0;}

void 
bezier::rotate(const point& center, const libcan_float angle){
	_a = geom_line(center, _a).rotate_fixed_a(angle).b;
	_b = geom_line(center, _b).rotate_fixed_a(angle).b;
	_c = geom_line(center, _c).rotate_fixed_a(angle).b;
	_d = geom_line(center, _d).rotate_fixed_a(angle).b;
	
}

void 
bezier::resize(const point& center, const libcan_float quoc){
	_a = geom_line(center, _a).resize(quoc).b;
	_b = geom_line(center, _b).resize(quoc).b;
	_c = geom_line(center, _c).resize(quoc).b;
	_d = geom_line(center, _d).resize(quoc).b;
}


void 
bezier::move(const point& where){
	_a = geom_line(point(0,0), where).move_point(_a);
	_b = geom_line(point(0,0), where).move_point(_b);
	_c = geom_line(point(0,0), where).move_point(_c);
	_d = geom_line(point(0,0), where).move_point(_d);
}

shape_type 
bezier::get_thick_line(const libcan_float thickness, const curve* const previous, const curve* const next) const {throw 1;}

list<moved_arrays> 
bezier::get_arrays() {
	//zdroj algoritmu - http://www.niksula.cs.hut.fi/~hkankaan/Homepages/bezierfast.html
	//(C) Hannu Kankaanpää
	
	libcan_int min_y = get_min_y();
	libcan_int max_y = get_max_y();
	
	libcan_uint steps = __maximum((max_y - min_y), (get_max_x()-get_min_x()))*5;
			//!!!!!!ta 5ka je umele dodana konstanta
			//mozna by to slo zvetsit/zmensit? ale me se to zda OK
	
	list<moved_arrays> res;
	moved_arrays ma(min_y, max_y);
			//tohle je samozrejme "navic" nez v tom algoritmu nahore
	
	bool up; //jestli zrovna BK jede smerem up
	bool first = true; //1 jen na zacatku
	
	point f;
	point fd, fdd, fddd, fdd_per_2, fddd_per_2, fddd_per_6;
	libcan_float t = static_cast<libcan_float>(1.0 / steps);
	libcan_float temp = t * t;
	
			//silene derivace.... 
			
	f = _a;
	fd = (_b - _a) * 3 * t;
	fdd_per_2 = (_a - _b * 2 + _c) * 3 * temp;
	fddd_per_2 = ((_b - _c) * 3 + _d - _a) * 3 * temp * t;
	
	fddd = fddd_per_2 + fddd_per_2;
	fdd = fdd_per_2 + fdd_per_2;
	fddd_per_6 = fddd_per_2 * (1.0 / 3);
	
	for (libcan_uint i=0; i < steps; ++i) {
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


libcan_int 
bezier::get_min_x() const {
	return static_cast<libcan_int>(__minimum4(_a.x,_b.x,_c.x, _d.x));
	//(b<((c<d)?(c):(d)))?(b):((c<d)?(c):(d))
}

libcan_int 
bezier::get_max_x() const{
	return static_cast<libcan_int>(__maximum4(_a.x, _b.x, _c.x, _d.x)+2);
}

libcan_int 
bezier::get_min_y() const {
	return static_cast<libcan_int>(__minimum4(_a.y,_b.y, _c.y, _d.y));
}

libcan_int
bezier::get_max_y() const{
	return static_cast<libcan_int> (__maximum4(_a.y, _b.y, _c.y, _d.y)+2);
}