#ifndef ALL_SHAPES_INC
#define ALL_SHAPES_INC

#include "shape.h"
#include "curves/circle.h"
#include "curves/line.h"
#include <vector>

namespace glib {
	
	struct disk:public shape_type {
		
		disk(const point& center, const glib_float radius): shape_type(1,1) {
			_curves.push_back(new circle(center, radius));
		} 
	};
	
	struct segment:public shape_type {
		segment(const glib_float ax, const glib_float ay, const glib_float bx, const glib_float by): shape_type(0,0) {
			_curves.push_back(new line(point(ax,ay),point(bx,by)));
		} 
	};
	
	struct polygon:public shape_type {
		
		polygon(const point& a, const point& b, const point&c):shape_type(1,1) {
			//since STDARG does not work in c++, i am gonna do it stupid way :(
			_curves.push_back(new line(a,b));
			_curves.push_back(new line(b,c));
			_curves.push_back(new line(c,a));
		}
		
		polygon(const point& a, const point& b, const point& c, const point& d):shape_type(1,1) {
			//since STDARG does not work in c++, i am gonna do it stupid way :(
			_curves.push_back(new line(a,b));
			_curves.push_back(new line(b,c));
			_curves.push_back(new line(c,d));
			_curves.push_back(new line(d,a));
		}
		
		polygon(const point& a, const point& b, const point& c, const point& d, const point& e):shape_type(1,1) {
			//since STDARG does not work in c++, i am gonna do it stupid way :(
			_curves.push_back(new line(a,b));
			_curves.push_back(new line(b,c));
			_curves.push_back(new line(c,d));
			_curves.push_back(new line(d,e));
			_curves.push_back(new line(e,a));
		}
		
		polygon(const point& a, const point& b, const point& c, const point& d, const point& e, const point& f):shape_type(1,1) {
			//since STDARG does not work in c++, i am gonna do it stupid way :(
			_curves.push_back(new line(a,b));
			_curves.push_back(new line(b,c));
			_curves.push_back(new line(c,d));
			_curves.push_back(new line(d,e));
			_curves.push_back(new line(e,f));
			_curves.push_back(new line(f,a));
		}
		
		polygon(const std::vector<point>& points):shape_type(1,1) {
			
			std::vector<point>::const_iterator i = points.begin();
			
			point a = *i;
			point first=a;
			
			++i;
			
			for (; i!=points.end(); ++i) {
				point b = *i;
				_curves.push_back(new line(a,b));
				a=b;
			}
			_curves.push_back(new line(a,first));
			
		}
	} ;
}



#endif
