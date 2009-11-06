#ifndef KRUZ_INC
#define KRUZ_INC

#include "curve.h"
#include "point.h"
#include <list>

namespace glib{
	class circle:public curve {
	private:
	
		void paint_more(glib_float x, glib_float y, moved_arrays& left, moved_arrays& right);
		
		point _center;
		glib_float _radius;
		
	public:	
		circle(point center, glib_float radius);
		
		virtual std::list<moved_arrays> get_arrays();

		virtual shape_type get_thick_line(const glib_float thickness, const curve* const previous, const curve* const next)const;

		virtual glib_int get_min_y() const;
		virtual glib_int get_max_y() const;
		virtual glib_int get_min_x() const;
		virtual glib_int get_max_x() const;
		virtual circle* clone() const {circle* n= new circle(_center,_radius);return n;}

	};
}



#endif
