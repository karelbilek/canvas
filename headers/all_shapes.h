#ifndef ALL_SHAPES_INC
#define ALL_SHAPES_INC

#include "shape.h"
#include "curves/circle.h"
#include "curves/line.h"
#include "curves/bezier.h"
#include <vector>
#include <string>

#include <list>


namespace libcan {
	
		//zname shape
	class disk:public shape_type {
	private:
		point _center;
		libcan_float _radius;
	public:
		disk(const point& center, const libcan_float radius);
		disk(const point& center, const point& ref_point);
		virtual std::vector<std::string> get_specific_properties();
	};
	
	class segment:public shape_type {
	private:
		point _a;
		point _b;
	public:	
		segment(const libcan_float ax, const libcan_float ay, const libcan_float bx, const libcan_float by);
		segment(const point& a, const point& b);
		virtual std::vector<std::string> get_specific_properties();
	};
	
	class bezier_segment:public shape_type {
	private:
		point _a,_b,_c,_d;
	public:	
		bezier_segment(const point& a, const point& b, const point& c, const point& d); 
		virtual std::vector<std::string> get_specific_properties();
		
	};
	
	class regular:public shape_type {
	private:
		point _a, _b;
		int _n;
	public:
		regular(const point& a, const point& b, int n);
		virtual std::vector<std::string> get_specific_properties();
		
	};
	
	class rectangle:public shape_type {
	private:
		point _a,_b,_v;
	public:
		rectangle(const point& a, const point& b, const point& v);
		virtual std::vector<std::string> get_specific_properties();
		
	};
	
	class elipse:public shape_type {
	private:
		point _s, _a, _v;
	public:
		elipse(const point& s, const point& a, const point& v);
		virtual std::vector<std::string> get_specific_properties();
		
	};
	
	class regular_from_center:public shape_type {
	private:
		point _c, _a;
		int _n;
	public:
		regular_from_center(const point& c, const point& a, int n);
		virtual std::vector<std::string> get_specific_properties();
		
	};
	
	class polygon:public shape_type {
	private:
		std::vector<point> _points;
	public:
			//since STDARG does not work in c++, i am gonna do it stupid way :/
		polygon(const point& a, const point& b, const point&c);
		
		polygon(const point& a, const point& b, const point& c, const point& d);
		
		polygon(const point& a, const point& b, const point& c, const point& d, const point& e);
		
		polygon(const point& a, const point& b, const point& c, const point& d, const point& e, const point& f);
		
		polygon(const std::vector<point>& points);
		polygon(const std::list<point>& points);
		virtual std::vector<std::string> get_specific_properties();
		
		
	};
}



#endif
