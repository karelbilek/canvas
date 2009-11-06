#ifndef SHAPE_TYPE_INC
#define SHAPE_TYPE_INC

#include <list>

namespace glib {
	class curve;
	
	struct shape_type {
		std::list<curve*> _curves;
		bool _filled;
		bool _joined_ends;
		shape_type(const bool filled, const bool joined_ends, const std::list<curve*>& curves=std::list<curve*>());
		shape_type(const shape_type& other);
	
		shape_type& operator=(const shape_type& other);
	
		virtual ~shape_type();
	};
}

#include "curve.h"


#endif
