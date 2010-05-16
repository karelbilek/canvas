#ifndef POINT_INC
#define POINT_INC
#include "types.h"

namespace libcan {
	
	class point {
		/*
		 * Nescetnekrat pouzivana trida na uchovavani bodu. Bod = 2 souradnice s double pointem (ano, double pointem)
		 * proc FP? protoze od jednotlivych bodu se napr. pocitaji kolmice a podobne - se zaokrouhlovanim na cela cisla je problem
		 *
		 * Umi zakladni operace, pozor, == netestuje UPLNOU rovnost, ale v ramci tolerance 0.01 nebo tak neco, ono 
		 * presne porovnavani FP stejne nema moc smysl
		 */
	public:
		double x;
		double y;
		point(double new_x, double new_y);
		point(long new_x, long new_y);
		point(int _x, int _y); 
		//point(double _x, double _y);
		
		point();
			//(0,0)
		
		
		point move(const double distance, double degrees) const;
			//posune pixel o nejakou vzdalenost po nejakem uhlu
		
		point operator+(const point& other) const;
			//secte dva pixely (druhy muze byt teoreticky i "nepixel", jenom vzdalenost
		
		point operator-(const point& other) const;
			//totez jen s odcitanim
		
		point operator*(const double n) const;
		point operator*(const long n) const;	
		point operator*(const int n) const;
		//point operator*(const double n) const;
			//Vynasob dalsim cislem (vzdy ale jenom cislem!)
			
			
		
		point trunc() const;
		
		point& operator=(const point& other);
			//prirazeni
		
		bool operator==(const point& other) const;
			//porovnani, neni uplne presne (float....)
		
		bool operator!=(const point& other) const;
			//negace vyse uvedeneho
		
		//bool smaller_deviation(const point& other) const;
			//takova zlastni fce, porovnavajici, jak daleko jsou odpuvodniho "gridu" s celyma cislama
			//je to k necemu jako trideni bodu
		
		point distance_from(const point& other) const;
			//vzdalenost od dalsiho bodu
	};
	
}


#endif
