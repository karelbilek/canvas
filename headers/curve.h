#ifndef CURVE_INC
#define CURVE_INC


#include "gr_object.h"
#include "moved_arrays.h"
#include <list>

namespace glib{
	
	
	class curve {

	public:
		
		virtual const std::list<const moved_arrays> get_arrays() = 0;

		virtual gr_object* get_thick_line(const glib_float thickness, const curve* const previous, const curve* const next) = 0;
				//cara s tloustkou
				//prepsat gr object na neco lepsiho!!§
				
		virtual glib_int get_min_y() const = 0;
		virtual glib_int get_max_y() const = 0;
		virtual glib_int get_min_x() const = 0;
		virtual glib_int get_max_x() const = 0;

	
		virtual ~curve() {};
				//pro jistotu :)

	};

	// namespace curve_help {

		// bool compare1(const curve& h1, const curve& h2);
				// jedno z porovnani, slouzici k serazeni k vyplnovani
				// v tomhle pripade tak, abych mohl jit po shora radkach zleva doprava a narazel na zacatky v danem poradi

		// typedef std::list<curve*> p_curves;
		// p_curves list_sort_copy(p_curves pc);
				// vezme list car, zkopiruje je (tj. new!) do dalsiho listu car, seradi je

		// bool p_compare(curve* a, curve* b);
		// bool p_compare1(const curve* a, const curve* b);
		//		pomocne - abych mohl radit i pointery na cary

	// }

}

#endif
