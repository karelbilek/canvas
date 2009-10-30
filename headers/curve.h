#ifndef CURVE_INC
#define CURVE_INC

#include "pixel.h"
#include "matrix.h"
#include "plane.h"

namespace glib{
	
	
	class curve {
	private:
	//	plane<bool> _pixels;
	//	moved_vector _starts;
	//	moved_vector _ends;
	
	//	bool _is_set;
				//mam uz nakresleno pomoci set_line?

		// integer _last;

		//floating _painting_rest;
				//"hack" - kolik mam uz nakresleno?
				//viz filled.cpp, metoda fill_pixels nebo tak neco

		integer _min_x;
		integer _max_x;


		//point _higher_pix;
		//point _lower_pix;
				//nejaky pixel z nejvrchnejsiho / nejspodnejsiho radku
				//k prvnimu serazeni u vyplnovani

protected:
		void set_all(const plane<bool>& pixels, moved_vector begins, moved_vector ends);
				//rovnou nastavi vsechno

		void set_pix(floating x1, floating x2);
		void set_pix(integer x1, integer x2);
				//pro nastavovani bod po bodu

public:
		curve(const pix& higher_pix, const pix& lower_pix, integer min_x, integer max_x);
				//nastavim hranice

		virtual void set_line() = 0;
				//potomek musi pomoci set_pix a set_all nastavit, co vsechno je cara

		virtual plane<bool> get_thick_line(floating thickness) = 0;
				//cara s tloustkou - ted uz "typicka" plane<bool>
				//casto pouze zavola nejaky filled

//----divne virt. procedury
		virtual int curve_type() const = 0;
				//vrati typ cary - smyslene cislo, tak, ze 2 cary maji ruzny typ <=> ruzne tohle cislo

		virtual bool compare2(const curve& dalsi) = 0;
				//porovna dve cary - tentokrat tak, ze plati, ze jsou si rovny <=> jsou si podobne,
				//jsou si rovny znamena false na obe strany, tj. musi se vymyslet nejake usporadani

		virtual void copy_self(curve*& pointer) = 0;
				//udela svoji vlastni kopii (tj. new!) a strci ji do pointeru na curve



		//bool operator<(curve& h);
				//seradi cary podle toho, jak jsou na radce, co zrovna kreslim (pres _painting_rest)
				//tak, abych nikdy nemel end predchoziho za beginem nasledujiciho, tj abych mohl v klidu kreslit

		integer get_end(integer y);
		integer get_start(integer y);
				//vrati zacatek / konec na urcitem radku

		integer get_min_y() const;
		integer get_max_y() const;
		integer get_min_x() const;
		integer get_max_x() const;
				//vrati maxima/minima apod

		//pix get_higher_pix() const;
		//pix get_lower_pix() const;

		//bool is_horizontal() const;

		plane<bool> get_all();
				//vrati line, na zaklade set_line

		//void painting_rest_dec();
		//bool nomore() const;
				//painting_rest se nikdy nezlower_pix a nikdy nepotrebuju vedet, kolik je, jenom jestli != 0

		virtual ~curve() {};
				//pro jistotu :)

	};

	namespace curve_help {
			//pomocne procedury, co jsem je nechtel strkat do curve, ale nechce se mi je nechat povalovat mimo namespace :)

		bool compare1(const curve& h1, const curve& h2);
				//jedno z porovnani, slouzici k serazeni k vyplnovani
				//v tomhle pripade tak, abych mohl jit po shora radkach zleva doprava a narazel na zacatky v danem poradi

		typedef std::list<curve*> p_curves;
		p_curves list_sort_copy(p_curves pc);
				//vezme list car, zkopiruje je (tj. new!) do dalsiho listu car, seradi je

		bool p_compare(curve* a, curve* b);
		bool p_compare1(const curve* a, const curve* b);
				//pomocne - abych mohl radit i pointery na cary

	}

}

#endif
