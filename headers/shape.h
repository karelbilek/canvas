#ifndef SHAPE_INC
#define SHAPE_INC
#include "plane.h"
#include "RGBa.h"
#include "curve.h"

#include "shape_type.h"
#include "point.h"
#include <list>
#include <map>
#include <set>
#include <string>




namespace libcan {
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
		
		libcan_int _line_size;
		RGBa _line_color;
		RGBa _fill_color; 
		
				
		shape_style(libcan_int line_size=1, const RGBa& line_color=RGBa(0,0,0), const RGBa& fill_color=RGBa(0,0,0));

	};

	class shape {
	private:
		
			//aby se pokazde, kdyz mam tloustku velikosti N, nemusel znova generovat krouzek
		static std::map<libcan_int, plane<bool> > brushes;
		
		shape_style _style;
		shape_type _type;
		
		plane<RGBa> _pixels;
		bool _changed;
		bool _painted;
		bool _footprint_given;
		plane<bool> _old_footprint;
		plane<bool> _new_footprint;
		

		static plane<bool> paint(const shape_type* const type, libcan_int min_y, libcan_int max_y) ;
		
		static bool compare_by_row(const moved_arrays& a, const moved_arrays& b);
		
		void destroying_change();
		void get_extremes(libcan_int& min_x, libcan_int& max_x, libcan_int& max_x, libcan_int& max_y, const bool& antialias, const libcan_int height, const libcan_int width) const;
		
	public:
		
		bool should_paint() const;
		
		shape_style& get_style(const bool& will_change);
		
		shape(const shape_style& style, const shape_type& type);
	
			
		plane<RGBa> get_pixels(const libcan_int height, const libcan_int width, const bool& antialias, const plane<bool>& where_not_paint, const bool& force=false);
		
		void rotate(libcan_float angle);
		void resize(libcan_float quoc);
		void move(const point& where);
		
		plane<bool> get_footprint(const bool& antialias, const libcan_int height, const libcan_int width, const bool do_change);
		
		std::set<std::string> get_properties();
		
		
		

	};
}
	
	
#endif

