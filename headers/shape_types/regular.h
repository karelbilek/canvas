#ifndef REGULAR_INC
#define REGULAR_INC

#include "all_shapes.h"
#include "shape.h"
#include "geom_line.h"


namespace libcan {
	class regular:public shape_type {
	private:
		point _a, _b;
		int _n;
		static geom_line help(const point&a, const point& b, int n);
	public:
		regular(const point& a, const point& b, const int n, const bool from_center=false);
		virtual shape_type* clone() const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* new_with_property(const std::string& property, std::stringstream& what);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
	};
	
}


#endif