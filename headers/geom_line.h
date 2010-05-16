#ifndef GEOM_LINE_INC
#define GEOM_LINE_INC

#include "point.h"


namespace libcan {
	struct geom_line {
		//pomocna trida, co reprezentuje "cistou" usecku
		//neni to u line, protoze puvodne bylo dost oddelene, co delaly
		//ted se to trochu smazalo, ale prijde mi prehlednejsi mit to takhle nez to vsechno cpat do line.h
		point a;
		point b;
		geom_line(const point& my_a, const point& my_b);
		geom_line();
		
		static geom_line line_from_point_angle(const point& first, double angle, const double length);
		
		point move_point(const point& point) const;
		
		geom_line rotate_fixed_a(const double angle) const;
		
		geom_line line_from_rev_angle(const double angle, const double length) const;
		
		geom_line resize(double quoc) const;
	
		
			//proste jenom prehodi poradi .)
		geom_line reverted() const; 
		
			//vypocita uhel, pokud ale predpokladam, ze
			// b=another.a,
			// to znamena ze vlastne pocitam uhel od obracene orientovane usecky
		double count_rev_angle(const geom_line another) const;
		
			//uhel od horizontaly
		double angle_from_x() const;
		
			//delka
		double length() const;
		
			//zkrati/prodlouzi to velikost na new_length
		geom_line normalised(const double new_length) const;
		
			//udela pravy uhel
		point right_angle_a(const bool clockwise, const double length) const;
		point right_angle_b(const bool clockwise, const double length) const;
		
		point right_angle_b(const point& c) const;
		
		
		geom_line right_angle(const bool clockwise, const point& start, const double length) const;
		
			//udela rovnobezku, vzdalenou distance ode mne
			//vlevo / vpravo
		geom_line parallel(const bool left, const double distance) const;
	
			//prusecik s jinou useckou (muze, a casto je mimo)
		point intersection(const geom_line& another) const;
		
			//vrati ty dva bodiky, co jsou potreba pri kresleni tluste cary
			//presneji - "obal" cary je vzdycky 4uhelnik, na kazde strane 
			//usecky jsou dva body. Ty dva body ale nezalezi jenom na 
			//usecce, ale i na te sousedni. Tahle je spocita a vrati.
			//second_hint je proto, ze pokud je uhel svirany temi useckami moc maly,
			//udela se ten obal trochu jinak, kde potrebuju v jednom pripade prehodit poradi bodu
		geom_line thick_cover(const geom_line& another, const double distance, bool second_hind) const;
		
		double distance(const point& p) const;
		
	};
	
}






#endif