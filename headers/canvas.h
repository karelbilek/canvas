#ifndef CANVAS_INC
#define CANVAS_INC

#include "matrix.h"
#include "shape.h"

namespace glib {
	class canvas {
	public:
		//typedef std::list<shape> shapes; //list vsech grafickych objektu
		typedef plane<RGBa>::T_list colors_row;	//list struktur (barva, zacatek, konec) z plane
	private:
		
		bool _antialias;
		glib_int _height;
		glib_int _width; //mohlo by byt uint, ale to bych tam stejne nic nikdy nenakreslil
		                
		RGBa _background;
		
		std::list<shape> _shapes;
		
	public:
		glib_int get_width() const;
		glib_int get_height() const;
		
		canvas(const size_t width, const size_t height, const RGBa& background, bool antialias);
		canvas();
		
		virtual ~canvas();
		
		matrix<glib_component> get_matrix(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos); 
		plane<RGBa> get_plane();
			//NENI CONST
			//sou to sice gettery, ale az ty gettery rikaji potomkum "hele, nakreslete se, kthx"!
		
		void remove(const size_t pos);
		void pop_back();
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

