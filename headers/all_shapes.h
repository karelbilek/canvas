#ifndef ALL_SHAPES_INC
#define ALL_SHAPES_INC

#include "shape.h"
#include "curves/circle.h"

namespace glib {
	
	struct disk:public shape_type {
		point _center;
		glib_float _radius;
		
		disk(const point& center, const glib_float radius):
		  _center(center),
		  _radius(radius),
		  shape_type(1,1){
			_curves.push_back(new circle(center, radius));
		} 
	};
}



#endif