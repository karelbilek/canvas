#ifndef CANVAS_INC
#define CANVAS_INC

#include "matrix.h"
#include "shape.h"

namespace glib {
	class canvas {
	public:
		typedef plane<RGBa>::T_list colors_row;	//list struktur (barva, zacatek, konec) z plane
	private:
		
		bool _antialias; //je zapnuty antialias?
		glib_int _height;
		glib_int _width; //sirka/vyska
		                
		RGBa _background; 
		
		std::list<shape> _shapes;
		
	public:
		
		canvas(const size_t width, const size_t height, const RGBa& background, bool antialias);
						//vytvori novy canvas s sirkou/delkou...
		canvas();
						//prazdny canvas
		
		~canvas();
						//smaz canvas
		
		matrix<glib_component> get_matrix(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos); 
						//vrat matrix s temahle pozicema
						//NENI const, viz niz
						
		plane<RGBa> get_plane();
						//vrati plane nakresleny
						//NENI CONST
						//sou to sice "gettery", ale az ty gettery rikaji potomkum "nakreslete se!"
		
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
		shape get_object(size_t pos) const;
	};
}

#endif

