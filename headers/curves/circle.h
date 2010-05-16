#ifndef KRUZ_INC
#define KRUZ_INC

#include "curve.h"
#include "point.h"
#include "geom_line.h"

namespace libcan{
	class circle:public curve {
		//circle, with given center and radius
		
	private:
		void paint_more(double x, double y, moved_arrays& left, moved_arrays& right);
			//function for faster painting - paint about eight pixels at once, 4 to every segment
			
		point _center;
		double _radius;
		
	public:	
		circle(point center, double radius);
		
		virtual std::list<moved_arrays> get_arrays();

		virtual shape_type get_thick_line(const double thickness, const curve* const previous, const curve* const next)const;

		virtual long get_min_y() const;
		virtual long get_max_y() const;
		virtual long get_min_x() const;
		virtual long get_max_x() const;
		virtual circle* clone() const;
		virtual circle* clone_double() const;
		virtual bool have_thick_line()const;
		
		virtual void rotate(const point& center, const double angle);
		virtual void resize(const point& center, const double quoc);
		virtual void move(const point& where);
		
		
	};
}



#endif
