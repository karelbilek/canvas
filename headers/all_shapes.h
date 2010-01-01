#ifndef ALL_SHAPES_INC
#define ALL_SHAPES_INC

#include "shape.h"
#include "curves/circle.h"
#include "curves/line.h"
#include "curves/bezier.h"
#include <vector>
#include <list>


namespace libcan {
	
		//všechny známé shape
	class disk:public shape_type {
	public:
		disk(const point& center, const libcan_float radius);
		disk(const point& center, const point& ref_point);
	};
	
	class segment:public shape_type {
	public:	
		segment(const libcan_float ax, const libcan_float ay, const libcan_float bx, const libcan_float by);
		segment(const point& a, const point& b);
	};
	
	class bezier_segment:public shape_type {
	public:	
		bezier_segment(const point& a, const point& b, const point& c, const point& d); 
	};
	
	class regular:public shape_type {
	public:
		regular(const point& a, const point& b, int n);
	};
	
	class rectangle:public shape_type {
	public:
		rectangle(const point& a, const point& b, const point& p);
	};
	
	class elipse:public shape_type {
	public:
		elipse(const point& s, const point& a, const point& p);
	};
	
	class regular_from_center:public shape_type {
	public:
		regular_from_center(const point& c, const point& a, int n);
	};
	
	class polygon:public shape_type {
	public:
			//since STDARG does not work in c++, i am gonna do it stupid way :/
		polygon(const point& a, const point& b, const point&c);
		
		polygon(const point& a, const point& b, const point& c, const point& d);
		
		polygon(const point& a, const point& b, const point& c, const point& d, const point& e);
		
		polygon(const point& a, const point& b, const point& c, const point& d, const point& e, const point& f);
		
		polygon(const std::vector<point>& points);
		polygon(const std::list<point>& points);
		
	};
}



#endif
