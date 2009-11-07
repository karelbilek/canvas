#ifndef SHAPE_INC
#define SHAPE_INC
#include "plane.h"
#include "RGBa.h"
#include "curve.h"
#include "shape_type.h"
#include <list>
#include <map>

namespace glib {
	
	struct shape_style {
		
		glib_int _line_size;
		RGBa _line_color;
		bool _fill_is; 
		RGBa _fill_color; 
		
		
		shape_style(glib_int line_size=1, const RGBa& line_color=RGBa(0,0,0), bool fill_is=false, const RGBa& fill_color=RGBa(0,0,0));
	};


	class shape {
	private:
		
		static std::map<glib_int, plane<bool> > brushes;
		
		shape_style _style;
		shape_type _type;
		

		static plane<bool> paint(const shape_type* const type, glib_int min_y, glib_int max_y);
				//neni const, protoze meni sorting hint		
		
		//static bool compare_by_top(const moved_arrays& a, const moved_arrays& b);
		static bool compare_by_row(const moved_arrays& a, const moved_arrays& b);
		
	public:
		shape(const shape_style& style, const shape_type& type);
			
		plane<RGBa> get_pixels(const glib_int height, const glib_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done);

	};
}
	
	
#endif

