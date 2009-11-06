#ifndef MIARRAYS_INC
#define MIARRAYS_INC

#include "types.h"

namespace glib {
	class moved_arrays {
	private:
		
		bool _first;
		//glib_int _min_x;
		//glib_int _max_x;
		
		glib_int _min_nonempty_y;
		glib_int _max_nonempty_y;
		
	
		glib_int _min_y;
		glib_int _max_y;
		
		glib_int* _starts;
		glib_int* _ends;
		bool* _is_set;
		
	public:
		glib_int _sorting_hint; //hack for comparator function
		
		moved_arrays(glib_int start, glib_int end);
		moved_arrays(glib_float start, glib_float end);
		
		moved_arrays(const moved_arrays& other);
		~moved_arrays();
		
		moved_arrays& operator=(const moved_arrays& other);
		
		glib_int get_start(const glib_int y) const;
		glib_int get_end(const glib_int y) const;
		
		void set(const glib_int x, const glib_int y);
		void set(const glib_float x, const glib_float y);
		
		bool is_set(const glib_int y) const;
		bool is_near(const glib_int x, const glib_int y) const;
		
		bool is_horizontal() const;
		
		//glib_int get_min_x() const;
		//glib_int get_min_y() const;
		//glib_int get_max_x() const;
		//glib_int get_max_y() const;
		
		glib_int get_min_nonempty_y() const;
		glib_int get_max_nonempty_y() const;
		
	};
}

#endif
