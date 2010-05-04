#ifndef DISK_INC
#define DISK_INC

#include "all_shapes.h"
#include "shape.h"

namespace libcan {
	class disk:public shape_type {
	private:
		point _center;
		libcan_float _radius;
	public:
		disk(const point& center, const libcan_float radius);
		disk(const point& center, const point& ref_point);
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* clone() const;
		virtual shape_type* new_with_property(const std::string& property, const std::string& what, const libcan_int what_int, const libcan_float what_float);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
	
	};
}


#endif