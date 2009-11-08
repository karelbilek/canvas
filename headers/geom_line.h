#ifndef GEOM_LINE_INC
#define GEOM_LINE_INC

#include "point.h"


namespace glib {
	struct geom_line {
		point a;
		point b;
		geom_line(const point& my_a, const point& my_b);
		geom_line();
		
		
		geom_line normalised(const glib_float length) const;
		
		point right_angle_a(const bool clockwise, const glib_float length) const;
		point right_angle_b(const bool clockwise, const glib_float length) const;
		
		geom_line parallel(const bool left, const glib_float distance) const;
		point intersection(const geom_line& another) const;
		
		//tady tu geom_line jenom hnusne pouzivam na ulozeni 2 pointu
		geom_line parallel_intersection(const geom_line& another, const glib_float distance) const;
	};
	
}






#endif