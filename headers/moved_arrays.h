#ifndef MIARRAYS_INC
#define MIARRAYS_INC

#include "types.h"
#include "plane.h"
#include <vector>

namespace glib {
	class moved_arrays {
		/*
		 * Trida na uchovavani segmentu car.
		 * 
		 * Ma to dva vektory - zacatky a konce. Tim je "vyreseno" i to, ze kus segmentu nemuze
		 * mit dva kusy vedle sebe, viz taky curve.h nebo kam jsem to psal.
		 */
	private:
		
		bool _first;
		
		glib_int _min_nonempty_y;
		glib_int _max_nonempty_y;
		
	
		glib_int _min_y;
		glib_int _max_y;
		
		std::vector<glib_int> _starts;
		std::vector<glib_int> _ends;
		std::vector<bool> _is_set;
		
	public:
		glib_int _sorting_hint; 
			//HACK - obcas chci seradit comparatorem vsechny segmenty
			//ale podle toho, jak jsou na urcite RADCE
			//a komparatoru NEMUZU dat argument
			//tak ho nastavim tady... rikam, hack
		
		moved_arrays(glib_int start, glib_int end);
		moved_arrays(glib_float start, glib_float end);
		
		
		glib_int get_start(const glib_int y) const;
		glib_int get_end(const glib_int y) const;
		
		plane<bool> to_plane() const;
		
		void set(const glib_int x, const glib_int y);
		void set(const glib_float x, const glib_float y);
		
		bool is_set(const glib_int y) const;
		bool is_near(const glib_int x, const glib_int y) const;
		
		bool is_horizontal() const;
		
		
		glib_int get_min_nonempty_y() const;
		glib_int get_max_nonempty_y() const;
		
	};
}

#endif
