#ifndef CURVE_INC
#define CURVE_INC

#include "moved_arrays.h"
#include "shape_type.h"
#include "point.h"



namespace libcan{
	
	
	class curve {
			//abstraktni cara
			//co umi "nakreslit" sama sebe, jak normalne, tak nekdy s tloustkou
			//trochu vic o tom, co to vlastne vraci:
				//
				// kazda cara se sklada ze "segmentu", pro ktere vsechny plati, ze nejsou
				// dva plne useky na y ose vedle sebe
				// tj. tohle :
				/*
						-
						 -
						  --------
								  -----
				*/
				// povoleno je
				//tohle 
				/*
						-
						 ------    -------
				*/
				// povoleno neni (v ramci "segmentu").
				// tyhle segmenty jsou ve skutecnosti reprezentovany dvema poli - zacatky a konce
				// vice v moved_arrays.h
				// 
				// nektere cary umi nakreslit samy sebe
				// kdyz umi, nevrati to jako segmenty, ale jako shape!
				// proc? abych napriklad mohl jednoduse (z hlediska kodu) vratit tlustou "rovnou caru" jako 
				// 4uhelnik, a ten uz zase umim nakreslit jinak
				//
				// (pokud se nakreslit neumi, musi vracet have_thick_line nulu a canvas je tluste nakresli sam)
				//
				// poznamka: potomci tehle tridy by si napr. NEMELI ty pole generovat uz v konstruktorech
				// jelikoz to get_arrays() je volano jenom, pokud je kreslit potreba
	public:
		
		virtual std::list<moved_arrays> get_arrays() = 0;
				//dej mi segmenty

		virtual bool have_thick_line() const=0;
				//umis tloustku?
		virtual shape_type get_thick_line(const double thickness, const curve* const previous, const curve* const next) const = 0;
				//cara s tloustkou
				
		virtual long get_min_y() const = 0;
		virtual long get_max_y() const = 0;
		virtual long get_min_x() const = 0;
		virtual long get_max_x() const = 0;
				//vraci leva/prava/horni/dolni maxima
		
		virtual void rotate(const point& center, const double angle)=0;
		virtual void resize(const point& center, const double quoc)=0;
		
		virtual void move(const point& where)=0;
		
		
		
		virtual curve* clone() const=0;
				//dej mi svoji kopii
		virtual curve* clone_double() const=0;
				//dej mi svoji kopii, dvakrat vetsi
				
		virtual ~curve() {};
				//pro jistotu :)

	};

}

#endif
