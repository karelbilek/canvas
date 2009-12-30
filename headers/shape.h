#ifndef SHAPE_INC
#define SHAPE_INC
#include "plane.h"
#include "RGBa.h"
#include "curve.h"
#include "shape_type.h"
#include <list>
#include <map>

namespace glib {
	/*
	* Se shape to nakonec vypada takhle:
	*
	* instance shape_type je napriklad "polygon s temito body: ...."
	* tj. nic nevi o tom, co ma za barvy, a umi se jenom zkopirovat nebo 2x zvetsit
	* a hlavně umí vrátit svoje curves a jejich segmenty
	* (tj. celý svůj "dokonalý" popis a popis čar)
	* ty čáry pak umí říct shape_type, co popisuje jejich tloušťku (nebo neumí, ale to tolik neva)
	* 
	* shape_style je prostě pouze styl (tj. tloušťka čáry, barva čáry, plnosti a tak)
	*
	* shape tohle spojuje, má atributy _style a _type
	* a podle typu a barvy vrací plochu s RGBa
	*/
	
	
	struct shape_style {
		
		glib_int _line_size;
		RGBa _line_color;
		bool _fill_is; 
		RGBa _fill_color; 
		
		
		shape_style(glib_int line_size=1, const RGBa& line_color=RGBa(0,0,0), bool fill_is=false, const RGBa& fill_color=RGBa(0,0,0));
	};


	class shape {
	private:
		
			//aby se pokazde, kdyz mam tloustku velikosti N, nemusel znova generovat krouzek
		static std::map<glib_int, plane<bool> > brushes;
		
		shape_style _style;
		shape_type _type;
		

		static plane<bool> paint(const shape_type* const type, glib_int min_y, glib_int max_y) ;
		
		static bool compare_by_row(const moved_arrays& a, const moved_arrays& b);
		
	public:
		shape(const shape_style& style, const shape_type& type);
			
		plane<RGBa> get_pixels(const glib_int height, const glib_int width, const bool antialias, const plane<bool>& painted_so_far, bool& done);

	};
}
	
	
#endif

