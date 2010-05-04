#include "shape_types/bezier_segment.h"

using namespace libcan;
using namespace std;


__shape_type_properties(bezier_segment, "a_x", "a_y", "b_x", "b_y", "c_x", "c_y", "d_x", "d_y");

shape_type* 
bezier_segment::clone() const {
	return new bezier_segment(_a, _b, _c, _d);
}

shape_type* 
bezier_segment::new_with_property(const string& property, std::stringstream& what){
	if (property=="a_x") {
		what >> _a.x;
	} else if (property=="a_y") {
		what >> _a.y;
	} else if (property=="b_x") {
		what >> _b.x;
	} else if (property=="b_y") {
		what >> _b.y;
	} else if (property=="c_x") {
		what >> _c.x;
	} else if (property=="c_y") {
		what >> _c.y;
	} else if (property=="d_x") {
		what >> _d.x;
	} else if (property=="d_y") {
		what >> _d.y;
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