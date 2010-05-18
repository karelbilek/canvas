#ifndef SHAPE_INC
#define SHAPE_INC
#include "plane.h"
#include "RGBa.h"
#include "curve.h"

#include "shape_type.h"
#include "point.h"





namespace libcan {

	
	
	struct shape_style {
		
		long _line_size;
		RGBa _line_color;
		RGBa _fill_color; 
		
				
		shape_style(long line_size=1, const RGBa& line_color=RGBa(0,0,0), const RGBa& fill_color=RGBa(0,0,0));

	};

	class shape {
	private:
		
		std::string _name;
			
			
			//aby se pokazde, kdyz mam tloustku velikosti N, nemusel znova generovat krouzek
		static std::map<long, plane<bool> > brushes;
		
		
		shape_style _style;
		shape_type* _type;
		
		plane<RGBa> _pixels;
		
		
		bool _changed;			//booleany pro ruzne stavy
		bool _painted;
		bool _footprint_given;
		
		
		plane<bool> _old_footprint;
		plane<bool> _new_footprint;
			//pokud neco nakreslim, dam footprint do _new_footprint (i kdyz neni zmena)
			//kdyz pak udelam zmenu, chci vedet, co prekreslit.
			//je to ta stara stopa + (odhad) nove stopy.
			//pokud novou stopu vim, dam ji, pokud ji NEVIM, odhadnu ji

		static plane<bool> paint(const shape_type* const type, long min_y, long max_y) ;
		
		static bool compare_by_row(const moved_arrays& a, const moved_arrays& b);
		
		void get_extremes(long& min_x, long& max_x, long& min_y, long& max_y, const bool& double_it, const long height, const long width) const;
		void destroying_change();
		
	public:
		void undestroying_change();
		
		shape(const shape& another);
		shape& operator=(const shape& another);
		
		bool should_paint() const;
		
		shape_style& get_style(const bool& will_change);
		
		shape(const shape_style& style, const shape_type& type);
	
			
		plane<RGBa> get_pixels(const long height, const long width, const bool& antialias, const plane<bool>& where_not_paint, const RGBa& background, const bool& force=false);
		
		void rotate(double angle);
		void resize(double quoc);
		void move(const point& where);
		
		plane<bool> get_footprint(const long height, const long width, const bool do_change, const bool force = false);
		
		std::string get_property(const std::string& property);
		std::set<std::string> get_properties();
		
		void set_property(const std::string& property, const std::string& what);
		
		

	};
}
	
	
#endif

