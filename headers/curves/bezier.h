#ifndef BEZIER_INC
#define BEZIER_INC

#include "point.h"
#include "curve.h"

namespace canlib{
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
		
		virtual shape_type get_thick_line(const canlib_float thickness, const curve* const previous, const curve* const next) const{throw 1;}
		
		virtual canlib_int get_min_y() const;
		virtual canlib_int get_max_y() const;
		virtual canlib_int get_min_x() const;
		virtual canlib_int get_max_x() const;
		virtual bezier* clone() const {bezier* n= new bezier(_a,_b,_c,_d);return n;}
		virtual bezier* clone_double() const {bezier* n= new bezier(_a*2,_b*2,_c*2,_d*2);return n;}
		virtual bool have_thick_line() {return 0;}
	};
}


#endif