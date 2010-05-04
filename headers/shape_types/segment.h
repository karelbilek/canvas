#ifndef SEGMENT_INC
#define SEGMENT_INC

#include "all_shapes.h"
#include "shape.h"

namespace libcan {
	class segment:public shape_type {
	private:
		point _a;
		point _b;
	public:	
		segment(const libcan_float ax, const libcan_float ay, const libcan_float bx, const libcan_float by);
		segment(const point& a, const point& b);
		virtual shape_type* clone() const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* new_with_property(const std::string& property, const std::string& what, const libcan_int what_int, const libcan_float what_float);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
	};
}


#endif