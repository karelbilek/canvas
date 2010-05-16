#ifndef SHAPE_TYPE_INC
#define SHAPE_TYPE_INC




#include "moved_arrays.h"
#include "point.h"

#define __shape_type_properties(name, ...) std::vector<std::string> name::get_specific_properties() {std::string arr[] = {__VA_ARGS__};return std::vector<std::string>(arr, arr+sizeof(arr)/sizeof(std::string));}


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
		
		void rotate(double angle);
		void resize(double quoc);
		void move(const point& where);
		
		
		void get_extremes(long& min_x,long& max_x,long& min_y,long& max_y)const;
		
			//vraci rovnou segmenty (pro zjednoduseni)
		std::list<moved_arrays> all_curve_arrays() const;
	

		shape_type& operator=(const shape_type& other);
		
				//zdvojnasobeni (kvuli antialiasu)
		shape_type* clone_double() const;
		
	
		virtual ~shape_type();
		
		virtual void get_property(const std::string& property, std::stringstream& where) const;
		
		virtual std::vector<std::string> get_specific_properties();
		virtual shape_type* clone() const;
		virtual shape_type* new_with_property(const std::string& property, std::stringstream& what);
	};
	
	
}

#include "curve.h"


#endif
