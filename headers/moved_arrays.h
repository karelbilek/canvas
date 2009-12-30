#ifndef MIARRAYS_INC
#define MIARRAYS_INC

#include "types.h"
#include "plane.h"
#include <vector>

namespace libcan {
	class moved_arrays {
		/*
		 * Trida na uchovavani segmentu car.
		 * 
		 * Ma to dva vektory - zacatky a konce. Tim je "vyreseno" i to, ze kus segmentu nemuze
		 * mit dva kusy vedle sebe, viz taky curve.h nebo kam jsem to psal.
		 */
	private:
		
		bool _first;
		
		libcan_int _min_nonempty_y;
		libcan_int _max_nonempty_y;
		
	
		libcan_int _min_y;
		libcan_int _max_y;
		
		std::vector<libcan_int> _starts;
		std::vector<libcan_int> _ends;
		std::vector<bool> _is_set;
		
	public:
		libcan_int _sorting_hint; 
			//HACK - obcas chci seradit comparatorem vsechny segmenty
			//ale podle toho, jak jsou na urcite RADCE
			//a komparatoru NEMUZU dat argument
			//tak ho nastavim tady... rikam, hack
		
		moved_arrays(libcan_int start, libcan_int end);
		moved_arrays(libcan_float start, libcan_float end);
		
		
		libcan_int get_start(const libcan_int y) const;
		libcan_int get_end(const libcan_int y) const;
		
		plane<bool> to_plane() const;
		
		void set(const libcan_int x, const libcan_int y);
		void set(const libcan_float x, const libcan_float y);
		
		bool is_set(const libcan_int y) const;
		bool is_near(const libcan_int x, const libcan_int y) const;
		
		bool is_horizontal() const;
		
		
		libcan_int get_min_nonempty_y() const;
		libcan_int get_max_nonempty_y() const;
		
	};
}

#endif
