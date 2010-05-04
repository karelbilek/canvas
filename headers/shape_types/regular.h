#ifndef REGULAR_INC
#define REGULAR_INC

#include "all_shapes.h"
#include "shape.h"

namespace libcan {
	class regular:public shape_type {
	private:
		point _a, _b;
		int _n;
	public:
		regular(const point& a, const point& b, int n);
		virtual shape_type* clone() const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* new_with_property(const std::string& property, const std::string& what, const libcan_int what_int, const libcan_float what_float);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
	};
	
	class regular_from_center:public shape_type {
	private:
		point _c, _a;
		int _n;
	public:
		regular_from_center(const point& c, const point& a, int n);
		virtual shape_type* clone() const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* new_with_property(const std::string& property, const std::string& what, const libcan_int what_int, const libcan_float what_float);
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
	};
}


#endif