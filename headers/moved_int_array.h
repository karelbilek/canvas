#ifndef MIARRAY_INC
#define MIARRAY_INC

#include "types.h"

namespace glib {
	class moved_int_array {
	private:
		glib_int _start;
		glib_int _end;
		
		glib_int* _array;
		bool* _is_set;
		
	public:
		moved_int_array(glib_int start, glib_int end);
		moved_int_array(moved_int_array& other);
		~moved_int_array();
		
		moved_int_array& operator=(const moved_int_array& other);
		
		glib_int get(const glib_int n) const;
		void set(const glib_int n, const glib_int co);
		bool is(const glib_int n) const;
	};
}

#endif