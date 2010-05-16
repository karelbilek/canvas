#ifndef MIARRAYS_INC
#define MIARRAYS_INC

#include "types.h"
#include "plane.h"

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
		
		long _min_nonempty_y;
		long _max_nonempty_y;
		
	
		long _min_y;
		long _max_y;
		
		std::vector<long> _starts;
		std::vector<long> _ends;
		std::vector<bool> _is_set;
		
	public:
		long _sorting_hint; 
			//HACK - obcas chci seradit comparatorem vsechny segmenty
			//ale podle toho, jak jsou na urcite RADCE
			//a komparatoru NEMUZU dat argument
			//tak ho nastavim tady
		
		moved_arrays(long start, long end);
		moved_arrays(double start, double end);
		
		
		long get_start(const long y) const;
		long get_end(const long y) const;
		
		plane<bool> to_plane() const;
		
		void set(const long x, const long y);
		void set(const double x, const double y);
		
		bool is_set(const long y) const;
		bool is_near(const long x, const long y) const;
		
		bool is_horizontal() const;
		
		
		long get_min_nonempty_y() const;
		long get_max_nonempty_y() const;
		
	};
}

#endif
