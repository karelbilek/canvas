#ifndef ELIPSE_INC
#define ELIPSE_INC

#include "all_shapes.h"
#include "shape.h"

namespace libcan {
	class elipse:public shape_type {
	private:
		point _s, _a, _v;
	public:
		elipse(const point& s, const point& a, const point& v);
		virtual shape_type* clone() const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* new_with_property(const std::string& property, const std::string& what, const libcan_int what_int, const libcan_float what_float);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
	};
}


#endif