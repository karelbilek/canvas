#ifndef CANVAS_INC
#define CANVAS_INC

#include "matrix.h"
#include "shape.h"

namespace libcan {
	class canvas {
			//canvas je "hlavni" trida
			//na tu se umistuji shapes, ona jim pak rika "kreslete se!"
	public:
		typedef plane<RGBa>::T_list colors_row;	//list struktur (barva, zacatek, konec) z plane
	private:
		
		bool _antialias; //je zapnuty antialias?
		bool _last_antialias;
		bool _first_paint;
		libcan_int _height;
		libcan_int _width; //sirka/vyska
		                
		RGBa _background; 
		
		std::list<shape> _shapes;
		plane<RGBa> _saved_plane;
		
		
	public:
		
		plane<bool> what_to_paint() const;
		
		canvas(const size_t width, const size_t height, const RGBa& background, bool antialias);
						//vytvori novy canvas s sirkou/delkou...
		canvas();
						//prazdny canvas
		
		~canvas();
						//smaz canvas
		
		matrix<libcan_component> get_matrix(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos) const; 
						//vrat matrix s temahle pozicema
						//NENI const, viz niz
						
		plane<RGBa> get_plane() const;
						//vrati plane nakresleny
						//NENI CONST
						//sou to sice "gettery", ale az ty gettery rikaji potomkum "nakreslete se!"
		
		void get_colors(libcan_component* p_red, libcan_component* p_green, libcan_component* p_blue, libcan_component* p_alpha) const;
		
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
		
		void push_front(const shape g, size_t pos);
		void push_back(const shape g, size_t pos);
		void remove_all();
		
			//this is NOT const by DEFINITION
			//this returns a reference to OBJECT, which I can manipulate
			//and by manipulating it, I can change it DIRECTLY in the canvas
		shape& get_object(size_t pos);
		shape& get_front();
		shape& get_back();
		
		void change_order(size_t from, size_t to);
		
		size_t count();
		
	};
}

#endif

