#ifndef CANVAS_INC
#define CANVAS_INC

#include "matrix.h"
#include "gr_object.h"

namespace glib {
	class canvas {
	public:
		typedef std::list<gr_object*> gr_objects;
		//list vsech grafickych objektu
		typedef plane<RGBa>::T_list colors_row;
		//list struktur (barva, zacatek, konec)
	private:
		
		bool _antialias;
		size_t _height;
		size_t _width;
		gr_objects _gr_objects;                
	protected:
		bool _del;
		RGBa _background;
		size_t _red_pos;
		size_t _green_pos;
		size_t _blue_pos;
		size_t _alpha_pos;
	public:		
		size_t get_width() const;
		size_t get_height() const;
		canvas(const canvas& other, const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos);
		canvas(const size_t width, const size_t height, const RGBa& background, bool antialias, const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos, bool del=true);
		canvas(const size_t red_pos, const size_t green_pos, const size_t blue_pos, const size_t alpha_pos, bool del=true);
		
		virtual ~canvas();
		
		matrix<component> raster() const; 
		plane<RGBa> color_plane() const;
		
		void remove(const size_t pos);
		void pop_back();
		void pop_front();
		void push_front(gr_object* g);
		void push_back(gr_object* g);
		void push_front(gr_object* g, size_t pos);
		void push_back(gr_object* g, size_t pos);
		void remove(gr_object* g);
		void remove_all();
	};
}

#endif

