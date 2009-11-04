#ifndef SHAPE_INC
#define SHAPE_INC
#include "plane.h"
#include "RGBa.h"
#include <list>

namespace glib {
	
	struct shape_style {
		
		glib_int _line_size;
		RGBa _line_color;
		bool _fill_is; 
		RGBa _fill_color; 
		
		
		shape_style(glib_int line_size=1, const RGBa& line_color=RGBa(0,0,0), bool fill_is=false, const RGBa& fill_color=RGBa(0,0,0));
	};

	struct shape_type {
		std::list<curve*> _curves;
		bool _filled;
		bool _joined_ends;
		shape_type(const bool filled, const bool joined_ends, const list<curve*>& curves=list<curve*>());
		shape_type(const shape_type& other);
		
		shape_type& operator=(const shape_type& other);
		
		~shape_type();
	};

	class shape {
	private:
		
		shape_style _style;
		shape_type _type;
		
		static plane<bool> paint(const shape_type& type, const glib_int min_y=0, const glib_int max_y=0);
		
		static bool compare_by_top(const moved_arrays a, const moved_arrays b);
		static bool compare_by_row(const moved_arrays a, const moved_arrays b);
		
	public:
		shape(shape_style& style, shape_type& type);
			
		plane<RGBa> get_pixels(const glib_int height, const glib_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done);

	};
}
	
	
#endif

