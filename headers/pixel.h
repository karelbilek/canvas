#ifndef PIXEL_INC
#define PIXEL_INC
#include "types.h"

namespace glib {

	struct point {
		/*
		 * Nescetnekrat pouzivana trida na uchovavani bodu. Bod = 2 souradnice s glib_float pointem (ano, glib_float pointem)
		 * proc FP? protoze od jednotlivych bodu se napr. pocitaji kolmice a podobne - se zaokrouhlovanim na cela cisla je problem
		 *
		 * Umi zakladni operace, pozor, == netestuje UPLNOU rovnost, ale v ramci tolerance 0.01 nebo tak neco, ono 
		 * presne porovnavani FP stejne nema moc smysl
		 */
		glib_float x;
		glib_float y;
		pix(glib_float _x, glib_float _y);
		pix(glib_int _x, glib_int _y);
		pix(int _x, int _y); 
		pix(double _x, double _y);
			//"klasicke" kry - kazdy je nekdy potreba
		
		pix();
			//(0,0)
		
		
		pix move(glib_float distance, glib_float degrees);
			//posune pixel o nejakou vzdalenost po nejakem uhlu
		
		pix operator+(const pix& other) const;
			//secte dva pixely (druhy muze byt teoreticky i "nepixel", jenom vzdalenost
		
		pix operator-(const pix& other) const;
			//totez jen s odcitanim
		
		pix operator*(const glib_float n) const;
		pix operator*(const glib_int n) const;	
		pix operator*(const int n) const;
		pix operator*(const double n) const;
			//Vynasob dalsim cislem (vzdy ale jenom cislem!)


		pix& operator=(const pix& other);
			//prirazeni
		
		bool operator==(const pix& other) const;
			//porovnani, neni uplne presne
		
		bool operator!=(const pix& other) const;
			//negace vyse uvedeneho
		
		bool smaller_deviation(const pix& other) const;
			//takova zlastni fce, porovnavajici, jak daleko jsou odpuvodniho "gridu" s nulama
			//je to k necemu jako trideni bodu
		
		pix distance_from(const pix& other) const;
			//vzdalenost od dalsiho bodu
	};

	typedef std::vector<pix> pixs;
		//nejake dalsi pomocne veci s pochybnym duvodem k existenci

}

namespace glib {
	
	struct curve_def {
		//pomocna struktura - definuje libovolnou curve, at uz line nebo bezier
		pix _a;
		pix _b;
		pix _c;
		pix _d;
		bool _is_bezier;
		pix start() const;
		
		pix end() const;
		
		curve_def(const pix& a, const pix& b, const pix& c, const pix& d);
		//bezier
		curve_def(const pix& a, const pix& b);
		//line
	};
	
	typedef std::vector<curve_def> curve_defs;
}

#endif
