#ifndef BEZIER_INC
#define BEZIER_INC

#include "point.h"
#include "curve.h"

namespace libcan{
	class bezier:public curve {
		//(kubicka) bezierova krivka, dana 4mi body
		//vice http://en.wikipedia.org/wiki/Bézier_curve#Cubic_B.C3.A9zier_curves
		//
		//virtualni metody viz curve.h
	private:
		point _a;
		point _b;
		point _c;
		point _d;
	public:
		
		bezier(point a, point b, point c, point d);
		
		virtual std::list<moved_arrays> get_arrays();
		
		virtual shape_type get_thick_line(const double thickness, const curve* const previous, const curve* const next) const;
		
		virtual long get_min_y() const;
		virtual long get_max_y() const;
		virtual long get_min_x() const;
		virtual long get_max_x() const;
		virtual bezier* clone() const;
		virtual bezier* clone_double() const;
		virtual bool have_thick_line()const;
		
		virtual void rotate(const point& center, const double angle);
		virtual void resize(const point& center, const double quoc);
		virtual void move(const point& where);
		
		
	};
}


#endif