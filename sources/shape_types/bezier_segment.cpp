#include "shape_types/bezier_segment.h"

using namespace libcan;
using namespace std;


__shape_type_properties(bezier_segment, "a_x", "a_y", "b_x", "b_y", "c_x", "c_y", "d_x", "d_y");

shape_type* 
bezier_segment::clone() const {
	return new bezier_segment(_a, _b, _c, _d);
}

shape_type* 
bezier_segment::new_with_property(const string& property, const string& what, const libcan_int what_int, const libcan_float what_float){
	if (property=="a_x") {
		_a.x = what_int;
	} else if (property=="a_y") {
		_a.y = what_int;
	} else if (property=="b_x") {
		_b.x = what_int;
	} else if (property=="b_y") {
		_b.y = what_int;
	} else if (property=="c_x") {
		_c.x = what_int;
	} else if (property=="c_y") {
		_c.y = what_int;
	} else if (property=="d_x") {
		_d.x = what_int;
	} else if (property=="d_y") {
		_d.y = what_int;
	}
	return new bezier_segment(_a, _b, _c, _d);
}

void 
bezier_segment::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="a_x") {
		where<<_a.x;
	} else if (property=="a_y") {
		where<<_a.y;
	} else if (property=="b_x") {
		where<<_b.x;
	} else if (property=="b_y") {
		where<< _b.y;
	} else if (property=="c_x") {
		where<<_c.x;
	} else if (property=="c_y") {
		where<<_c.y;
	} else if (property=="d_x") {
		where<<_d.x;
	} else if (property=="d_y") {
		where<<_d.y;
	} else if (property=="name") {
		where << "bezier curve"; 
	}
}


bezier_segment::bezier_segment(const point& a, const point& b, const point& c, const point& d): 
 shape_type(0,0),
 _a(a),
 _b(b),
 _c(c),
 _d(d) {
	_curves.push_back(new bezier(a,b,c,d));
}