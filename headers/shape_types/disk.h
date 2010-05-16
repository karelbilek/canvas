#ifndef DISK_INC
#define DISK_INC

#include "all_shapes.h"
#include "shape.h"

namespace libcan {
	class disk:public shape_type {
	private:
		point _center;
		double _radius;
	public:
		disk(const point& center, const double radius);
		disk(const point& center, const point& ref_point);
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* clone() const;
		virtual shape_type* new_with_property(const std::string& property, std::stringstream& what);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
	
	};
}


#endif