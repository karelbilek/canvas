#include "shape_types/regular.h"

using namespace libcan;
using namespace std;


__shape_type_properties(regular, "a_x", "a_y", "b_x", "b_y", "n");

shape_type* 
regular::clone() const {
	return new regular(_a, _b, _n);
}

shape_type* 
regular::new_with_property(const string& property, stringstream& what){
	if (property=="a_x") {
		what >> _a.x;
	} else if (property=="a_y") {
		what >> _a.y;
	} else if (property=="b_x") {
		what >> _b.x;
	} else if (property=="b_y") {
		what >> _b.y;
	} else if (property=="n") {
		what >> _n;
	}
	return new regular(_a, _b, _n);
}

void 
regular::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="a_x") {
		where<<_a.x;
	} else if (property=="a_y") {
		where<<_a.y;
	} else if (property=="b_x") {
		where<<_b.x;
	} else if (property=="b_y") {
		where<<_b.y;
	} else if (property=="n") {
		where<<_n;
	} else if (property=="name") {
		where << "regular "<<_n<<"-gon"; 
	}
}

__shape_type_properties(regular_from_center, "c_x", "c_y", "a_x", "a_y", "n");

shape_type* 
regular_from_center::clone() const {
	return new regular_from_center(_c, _a, _n);
}

shape_type* 
regular_from_center::new_with_property(const string& property, stringstream& what){
	if (property=="a_x") {
		what >> _a.x;
	} else if (property=="a_y") {
		what >> _a.y;
	} else if (property=="c_x") {
		what >> _c.x;
	} else if (property=="c_y") {
		what >> _c.y;
	} else if (property=="n") {
		what >> _n;
	}
	return new regular_from_center(_c, _a, _n);
}

void 
regular_from_center::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="a_x") {
		where << _a.x;
	} else if (property=="a_y") {
		where << _a.y;
	} else if (property=="c_x") {
		where << _c.x;
	} else if (property=="c_y") {
		where << _c.y;
	} else if (property=="n") {
		where << _n;
	} else if (property=="name") {
		where << "regular "<<_n<<"-gon"; 
	}
}

regular::regular(const point& a, const point& b, int n):
 shape_type(1,1),
 _a(a),
 _b(b),
 _n(n) {
	libcan_float angle = (180-360/n);
	libcan_float length = geom_line(a,b).length();
	
	point aa = a;
	point bb = b;
	
	for (int i=0; i<n-1; i++){
		_curves.push_back(new line(aa,bb));
		point c = geom_line(aa,bb).line_from_rev_angle(angle, length).b;
		aa=bb;
		bb=c;
	}
	
	_curves.push_back(new line(aa,a));
			//nepresnosti se kumuluji, tak o pixel to vyjde jinak, ale to uz dela potize!
	
}

regular_from_center::regular_from_center(const point& c, const point& a, int n):
 shape_type(1,1),
 _c(c),
 _a(a),
 _n(n) {
	libcan_float angle = (180-360/n);
	
	geom_line me(c, a);
	
	libcan_float length = cos(__DEG2RAD(angle/2))*2*( me.length());
	geom_line l = me.line_from_rev_angle(angle/2, length);
	
	point aa = l.a;
	point bb = l.b;
	
	for (int i=0; i<n-1; i++){
		_curves.push_back(new line(aa,bb));
		point c = geom_line(aa,bb).line_from_rev_angle(angle, length).b;
		aa=bb;
		bb=c;
	}
	
	_curves.push_back(new line(aa,l.a));	
	
	
}