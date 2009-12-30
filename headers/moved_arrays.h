#ifndef MIARRAYS_INC
#define MIARRAYS_INC

#include "types.h"
#include "plane.h"
#include <vector>

namespace canlib {
	class moved_arrays {
		/*
		 * Trida na uchovavani segmentu car.
		 * 
		 * Ma to dva vektory - zacatky a konce. Tim je "vyreseno" i to, ze kus segmentu nemuze
		 * mit dva kusy vedle sebe, viz taky curve.h nebo kam jsem to psal.
		 */
	private:
		
		bool _first;
		
		canlib_int _min_nonempty_y;
		canlib_int _max_nonempty_y;
		
	
		canlib_int _min_y;
		canlib_int _max_y;
		
		std::vector<canlib_int> _starts;
		std::vector<canlib_int> _ends;
		std::vector<bool> _is_set;
		
	public:
		canlib_int _sorting_hint; 
			//HACK - obcas chci seradit comparatorem vsechny segmenty
			//ale podle toho, jak jsou na urcite RADCE
			//a komparatoru NEMUZU dat argument
			//tak ho nastavim tady... rikam, hack
		
		moved_arrays(canlib_int start, canlib_int end);
		moved_arrays(canlib_float start, canlib_float end);
		
		
		canlib_int get_start(const canlib_int y) const;
		canlib_int get_end(const canlib_int y) const;
		
		plane<bool> to_plane() const;
		
		void set(const canlib_int x, const canlib_int y);
		void set(const canlib_float x, const canlib_float y);
		
		bool is_set(const canlib_int y) const;
		bool is_near(const canlib_int x, const canlib_int y) const;
		
		bool is_horizontal() const;
		
		
		canlib_int get_min_nonempty_y() const;
		canlib_int get_max_nonempty_y() const;
		
	};
}

#endif
