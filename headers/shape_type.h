#ifndef SHAPE_TYPE_INC
#define SHAPE_TYPE_INC

#include <list>
#include "moved_arrays.h"
#include "point.h"

namespace libcan {
	class curve;
	
	struct shape_type {
			//vsechny kraje
		std::list<curve*> _curves;
		
			//je vyplneny, nebo neni?
			//(nejde o styl! a naopak na nem nezalezi! jde o to, jestli tu vypln vubec delat)
		bool _filled;
		
			//jestli ma spojene konce
		bool _joined_ends;
		
			//pozor, ocekava se, ze ty cary jsou poslany naalokovane a destruktor je pak odalokuje!
		shape_type(const bool filled, const bool joined_ends, const std::list<curve*>& curves=std::list<curve*>());
		shape_type(const shape_type& other);
		
		shape_type();
		
		point center() const;
		
		void rotate(libcan_float angle);
		void resize(libcan_float quoc);
		void move(const point& where);
		
		
		void get_extremes(libcan_int& min_x,libcan_int& max_x,libcan_int& min_y,libcan_int& max_y)const;
		
			//vraci rovnou segmenty (pro zjednoduseni)
		std::list<moved_arrays> all_curve_arrays() const;
	

		shape_type& operator=(const shape_type& other);
		
				//zdvojnasobeni (kvuli antialiasu)
		shape_type* clone_double() const;
	
		virtual ~shape_type();
	};
	
	
}

#include "curve.h"


#endif
