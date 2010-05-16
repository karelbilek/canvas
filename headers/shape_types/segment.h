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
		segment(const double ax, const double ay, const double bx, const double by);
		segment(const point& a, const point& b);
		virtual shape_type* clone() const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* new_with_property(const std::string& property, std::stringstream& what);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
	};
}


#endif