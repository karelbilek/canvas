#ifndef ALL_SHAPES_INC
#define ALL_SHAPES_INC

#include "shape.h"
#include "curves/circle.h"
#include "curves/line.h"
#include "curves/bezier.h"
#include <vector>

namespace glib {
	
		//all the known shape_types in the universe
	class disk:public shape_type {
	public:
		disk(const point& center, const glib_float radius);
	};
	
	class segment:public shape_type {
	public:	
		segment(const glib_float ax, const glib_float ay, const glib_float bx, const glib_float by);
	};
	
	class bezier_segment:public shape_type {
	public:	
		bezier_segment(const point& a, const point& b, const point& c, const point& d); 
	};
	
	class polygon:public shape_type {
	public:
			//since STDARG does not work in c++, i am gonna do it stupid way :/
		polygon(const point& a, const point& b, const point&c);
		
		polygon(const point& a, const point& b, const point& c, const point& d);
		
		polygon(const point& a, const point& b, const point& c, const point& d, const point& e);
		
		polygon(const point& a, const point& b, const point& c, const point& d, const point& e, const point& f);
		
		polygon(const std::vector<point>& points);
	};
}



#endif
