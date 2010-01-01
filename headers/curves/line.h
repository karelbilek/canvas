#ifndef LINE_INC
#define LINE_INC

#include "curve.h"
#include "point.h"
#include <list>

namespace libcan{
	class line:public curve {
		//usecka z bodu A do bodu B
	private:
			
		point _a;
		point _b;
		
		
	public:
		
		line(point a, point b);
		
		virtual std::list<moved_arrays> get_arrays();
		
		virtual shape_type get_thick_line(const libcan_float thickness, const curve* const previous, const curve* const next) const;

		virtual libcan_int get_min_y() const;
		virtual libcan_int get_max_y() const;
		virtual libcan_int get_min_x() const;
		virtual libcan_int get_max_x() const;
		virtual line* clone() const;
		virtual line* clone_double() const;
		virtual bool have_thick_line() const;
		
		virtual void rotate(const point& center, const libcan_float angle);
		virtual void resize(const point& center, const libcan_float quoc);
		virtual void move(const point& where);
		
		
	};
}



#endif