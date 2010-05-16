#ifndef CANVAS_INC
#define CANVAS_INC

#include "shape.h"


#include "RGBa.h"
#include "point.h"
#include "all_shapes.h"
#include "plane.h"

namespace libcan {
	class canvas {
			//canvas je "hlavni" trida
			//na tu se umistuji shapes, ona jim pak rika "kreslete se!"
	public:
		typedef plane<RGBa>::T_list colors_row;	//list struktur (barva, zacatek, konec) z plane
	private:
		
			//je antialias?
		bool _antialias; 
		
			//ma kreslit, i kdyz neni zmena?
		bool _force_paint;
		
			//bylo neco smazano?
		bool _is_deleted;
		
			//na jake plose?
		plane<bool> _deleted;
		
		long _height;
		long _width; 
			//sirka/vyska
		              
		  	//pozadi
		RGBa _background; 
		
			//tvary
		std::list<shape> _shapes;
		
			//naposledy nakresleno
		plane<RGBa> _saved_plane;
		
			//co mam nakreslit? v podobe plane<bool>
			//change = ma to zmeny "oznacit jako prectene"?
		plane<bool> what_to_paint(const bool change=false);
		
	public:
		long get_width() const;
		long get_height() const;
		bool is_force_paint() const;
			//gettery
		
			//mam kreslit?
		bool should_paint() const;
		
			//vrat jenom jmena
		std::vector<std::string> get_names();
		
			//zavola what_to_paint a prevede do neceho lepsiho
		std::vector<interval_info> what_should_paint();
		
	
		canvas(const size_t width, const size_t height, const RGBa& background=RGBa(0,0,0,0), bool antialias=false);
						//vytvori novy canvas s sirkou/delkou...
	
		canvas();
						//prazdny canvas
		
	
		~canvas();
						//smaz canvas
						
	
		plane<RGBa> get_plane();
						//vrati plane nakresleny
		
		void get_colors(unsigned char* p_red, unsigned char* p_green, unsigned char* p_blue, unsigned char* p_alpha) const;
		
		void set_antialias(const bool what);
		void set_RGBa(const RGBa& what);
		
		void remove(const size_t pos);
						//smaz neco na x-te pozici
						
		void pop_back();
						//smaz zezadu
		void pop_front();
		void push_front(const shape g);
		void push_back(const shape g);
		
		void push_front(const shape_style& style, const shape_type& type);
		void push_back(const shape_style& style, const shape_type& type);
		
		void push_front(const shape_style& style, const shape_type& type, size_t pos);
		void push_back(const shape_style& style, const shape_type& type, size_t pos);
		
		void push_front(const shape g, size_t pos);
		void push_back(const shape g, size_t pos);
		void remove_all();
		
		shape& get_object(size_t pos);
		shape& get_front();
		shape& get_back();
		
		void change_order(size_t from, size_t to);
		
		size_t count();
		
	};
}

#endif

