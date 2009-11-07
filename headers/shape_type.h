#ifndef SHAPE_TYPE_INC
#define SHAPE_TYPE_INC

#include <list>
#include "moved_arrays.h"

namespace glib {
	class curve;
	
	struct shape_type {
		std::list<curve*> _curves;
		bool _filled;
		bool _joined_ends;
		shape_type(const bool filled, const bool joined_ends, const std::list<curve*>& curves=std::list<curve*>());
		shape_type(const shape_type& other);
		
		std::list<moved_arrays> all_curve_arrays() const;
	
		shape_type& operator=(const shape_type& other);
		
		shape_type* clone_double() const;
	
		virtual ~shape_type();
	};
}

#include "curve.h"


#endif
