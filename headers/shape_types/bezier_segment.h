#ifndef BEZIER_SEG_INC
#define BEZIER_SEG_INC

#include "all_shapes.h"
#include "shape.h"

namespace libcan {
	class bezier_segment:public shape_type {
	private:
		point _a,_b,_c,_d;
	public:	
		bezier_segment(const point& a, const point& b, const point& c, const point& d); 
		virtual shape_type* clone() const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* new_with_property(const std::string& property, const std::string& what, const libcan_int what_int, const libcan_float what_float);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
		
	};
}


#endif