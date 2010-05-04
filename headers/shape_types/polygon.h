#ifndef PLYGON_INC
#define PLYGON_INC

#include "all_shapes.h"
#include "shape.h"

namespace libcan {
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
		virtual shape_type* clone() const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* new_with_property(const std::string& property, const std::string& what, const libcan_int what_int, const libcan_float what_float);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
		
	};
}


#endif