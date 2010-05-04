#include "shape_types/disk.h"

using namespace libcan;
using namespace std;

__shape_type_properties(disk, "center_x", "center_y", "radius");

shape_type* 
disk::clone() const {
	return new disk(_center, _radius);	
}

shape_type* 
disk::new_with_property(const string& property, const string& what, const libcan_int what_int, const libcan_float what_float){
	if (property=="center_x") {
		_center.x = what_int;
	} else if (property=="center_y") {
		_center.y = what_int;
	} else if (property=="radius") {
		_radius = what_float; 
	}
	return new disk(_center, _radius);
}

void 
disk::get_property(const std::string& property, std::stringstream& where) const {
	if (property=="center_x") {
		where << _center.x;
	} else if (property=="center_y") {
		where << _center.y;
	} else if (property=="radius") {
		where << _radius; 
	} else if (property=="name") {
		where << "disk with center ["<<_center.x<<","<<_center.y<<"]"; 
	}
}

disk::disk(const point& center, const libcan_float radius): 
 shape_type(1,1),
 _center(center),
 _radius(radius) {
	_curves.push_back(new circle(center, radius));
}

disk::disk(const point& center, const point& ref_point):
 shape_type(1,1),
 _center(center),
 _radius(geom_line(center, ref_point).length()) {
	_curves.push_back(new circle(center, _radius));
}