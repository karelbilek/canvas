#ifndef LINE_INC
#define LINE_INC

#include "curve.h"
#include "point.h"
#include <list>

namespace canlib{
	class line:public curve {
		//usecka z bodu A do bodu B
	private:
			
		point _a;
		point _b;
		
		
	public:
		
		line(point a, point b);
		
		virtual std::list<moved_arrays> get_arrays();
		
		virtual shape_type get_thick_line(const canlib_float thickness, const curve* const previous, const curve* const next) const;

		virtual canlib_int get_min_y() const;
		virtual canlib_int get_max_y() const;
		virtual canlib_int get_min_x() const;
		virtual canlib_int get_max_x() const;
		virtual line* clone() const {line* n= new line(_a,_b);return n;}
		virtual line* clone_double() const {line* n= new line(_a*2,_b*2);return n;}
		virtual bool have_thick_line() {return 1;}
	};
}



#endif