#ifndef KRUZ_INC
#define KRUZ_INC

#include "curve.h"
#include "point.h"
#include <list>

namespace libcan{
	class circle:public curve {
		//circle, with given center and radius
		
	private:
		void paint_more(libcan_float x, libcan_float y, moved_arrays& left, moved_arrays& right);
			//function for faster painting - paint about eight pixels at once, 4 to every segment
			
		point _center;
		libcan_float _radius;
		
	public:	
		circle(point center, libcan_float radius);
		
		virtual std::list<moved_arrays> get_arrays();

		virtual shape_type get_thick_line(const libcan_float thickness, const curve* const previous, const curve* const next)const;

		virtual libcan_int get_min_y() const;
		virtual libcan_int get_max_y() const;
		virtual libcan_int get_min_x() const;
		virtual libcan_int get_max_x() const;
		virtual circle* clone() const;
		virtual circle* clone_double() const;
		virtual bool have_thick_line()const;
	};
}



#endif
