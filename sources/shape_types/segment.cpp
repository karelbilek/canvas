#include "shape_types/segment.h"

using namespace libcan;
using namespace std;


__shape_type_properties(segment, "a_x", "a_y", "b_x", "b_y");

shape_type* 
segment::clone() const {
	return new segment(_a, _b);
}

shape_type* 
segment::new_with_property(const string& property, const string& what, const libcan_int what_int, const libcan_float what_float){
	if (property=="a_x") {
		_a.x = what_int;
	} else if (property=="a_y") {
		_a.y = what_int;
	} else if (property=="b_x") {
		_b.x = what_int;
	} else if (property=="b_y") {
		_b.y = what_int;
	}
	return new segment(_a, _b);
}

void 
segment::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="a_x") {
		where<<_a.x;
	} else if (property=="a_y") {
		where<<_a.y;
	} else if (property=="b_x") {
		where<<_b.x;
	} else if (property=="b_y") {
		where<<_b.y;
	} else if (property=="name") {
		where << "segment "<<_a.x<<","<<_a.y<<"] - ["<<_b.x<<","<<_b.y<<"]"; 
	}
}

segment::segment(const libcan_float ax, const libcan_float ay, const libcan_float bx, const libcan_float by): 
 shape_type(0,0),
 _a(point(ax,ay)),
 _b(point(bx,by)) {
	_curves.push_back(new line(_a, _b));
} 

segment::segment(const point& a, const point& b): 
 shape_type(0,0),
 _a(a),
 _b(b) {
	_curves.push_back(new line(a,b));
}
