#ifndef KRUZ_INC
#define KRUZ_INC

#include "curve.h"
#include "point.h"

namespace glib{
	class circle:public curve {
	private:
	
		void paint_more(glib_float x, glib_float y, moved_arrays& left, moved_arrays& right);
		
		point _center;
		glib_float _radius;
		
	public:	
		circle(point center, glib_float radius);
		
		virtual std::list<moved_arrays> get_arrays();

		virtual gr_object* get_thick_line(const glib_float thickness, const curve* const previous, const curve* const next);

		virtual glib_int get_min_y() const;
		virtual glib_int get_max_y() const;
		virtual glib_int get_min_x() const;
		virtual glib_int get_max_x() const;

	};
}



#endif
