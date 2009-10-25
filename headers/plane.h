#ifndef PL_INC
#define PL_INC

#include "intervals.h" //kazdy radek je interval

namespace glib {

	template<class T>
	class plane {
		/* 
		 * Trida na ukladani pixelu, co nezabira zbytecne mnoho mista a pritom je pomerne rychla.
		 * Kazdy radek je ulozen jako "intervals" - struktura na ukladani intervalu - vice info o te strukture tam.
		 * Radky pak jsou ulozeny v "obycejnem" vektoru.
		 *
		 * Nevyhoda je v tom, ze napriklad u obdelniku bude v kazdem radku totez, ty same intervaly. Na
		 * druhou stranu, je pak jednoduche smichat vic ploch dohromady - s tim, ze smichat
		 * dva radky s intervaly je jednoduche, kdezto nejaka struktura pamatujuci si vylozene 2D plochy
		 * by tohle mela o dost komplikovanejsi, stejne jako pridavani "po pixelech".
		 *
		 * Radky jsou cislovany relativne - tim je mysleno, ze vektor je sice indexovan od nuly, ale 
		 * navenek se plocha tvari, ze je indexovana od jakehokoliv radku po jakykoliv, tj. klidne i v zapornych cislech - 
		 * hodnota prvniho a posledniho radku ale musi byt znama uz pri vytvareni konstruktorem.
		 *
		 * vic o intervalech v intervals.h a interval_node.h (snad)
		 */
		
	private:	
		u_glib_int _real_height;
			//skutecna vyska plochy (je 0.._real_height-1)
		glib_int _pivot_width;
			//sirka, co se pouziva pro posouvani z mista na misto (pokud se napr. stejny disk kresli vicekrat, staci ho posunout)
		glib_int _start_height;
			//zacatecni vyska
		glib_int _end_height;
			//konecna vyska
		typedef std::vector< intervals<T> > T_intervals;
		T_intervals _intervals; 
		
	public:	
		typedef std::list<interval_content<T> > T_list;
			//list "obsahu" - obsah struktura T, zacatek, konec
		
		typedef std::list<interval_content<bool> > bool_list;
			//totez special pro bool - hodi se pro jeden konstruktor

		void whatthefuckshit() {_intervals[0].whatthefuckshit();}
		
		plane();
			//defaultni konstruktor - vytvori prazdnou plochu s velikosti 1
		
		plane(const glib_int start_height, const glib_int end_height, const glib_int pivot_width);
			//konstruktor, co chce pocatecni a konecnou vysku a pivotni sirku a neprida nikam nic
		
		plane(const glib_int start_width, const glib_int end_width, const glib_int start_height, const glib_int end_height, const T& what);
			//konstruktor, co se mu jeste da zacatek a konec kazdeho radku a on do kazdeho prida velky interval s danou hodnotou
		
		plane(const plane<bool>& other, const T& what);
			//POZOR - bool je umyslne! Tenhle konstruktor vezme nejaky uz hotovy plane<bool>, a kde je true, da what.
		
		glib_int get_start_height() const;
		glib_int get_end_height() const;
		glib_int get_real_height() const;
		glib_int get_pivot_width() const;
			//klasicke gettery
		
		glib_int first_non_zero() const;
		glib_int last_non_zero() const;
		
		glib_int most_left(glib_int y) const;
		glib_int most_right(glib_int y) const;
		
		glib_int most_top_left() const;
		glib_int most_down_right() const;
		
		glib_int most_left() const;
		glib_int most_right() const;
		
		glib_int min_x() const;
		glib_int max_x() const;
		
		T_list get_row(glib_int y) const;
			//vrati konkretni radek v listu paru <T, rozsah>
		
		T get(const glib_int x, const glib_int y) const;
			//vrati hodnotu na konkretni pozici, nebo defaultni T(), pokud tam nic neni
		
		void set(const glib_int x, const glib_int y, const T& what);
			//nastavi konkretni hodnotu
		
		void set_more(const glib_int start_x, const glib_int end_x, const glib_int y, const T& what);
			//nastavi na y radku vse od start_x do end_x na what (tj, prida interval, mozna neco upravi)
		
		plane<T> move(const glib_int pivot_width, const glib_int start_height);
			//posune cely plane, aby mel "novy" pivot_width a start_height
		
		void add(const plane<T>& other);
			//slouci s jinou plochou 
			//POZOR - z jine plochy vezme jen veci od _start_height po _end_height, zbytek ignoruje
	};
}

template <class T> glib::glib_int
glib::plane<T>::most_left() const {
	glib_int min = INTEGER_MAX;
	for (glib_int i = first_non_zero(); i <= last_non_zero(); ++i) {
		min = __minimum(min, _intervals[i-_start_height].most_left());
	}
	return min;
}

template <class T> glib::glib_int
glib::plane<T>::most_right() const {
	glib_int max = 0;
	for (glib_int i = first_non_zero(); i <= last_non_zero(); ++i) {
		max = __maximum(max, _intervals[i-_start_height].most_right());
	}
	return max;
}

template <class T> glib::glib_int
glib::plane<T>::most_top_left() const {
	return most_left(first_non_zero());
	
}


template <class T> glib::glib_int
glib::plane<T>::most_down_right() const {
	return most_right(last_non_zero());
}

template <class T> glib::glib_int
glib::plane<T>::most_left(glib::glib_int y) const {
	return _intervals[y-_start_height].most_left();
}

template <class T> glib::glib_int
glib::plane<T>::most_right(glib::glib_int y) const {
	return _intervals[y-_start_height].most_right();
}

template <class T> glib::glib_int
glib::plane<T>::first_non_zero() const {
	for (u_glib_int i = 0; i < _real_height; ++i) {
		if (!_intervals[i].is_empty())
			return i+_start_height;
	}
	return _end_height;
}

template <class T> glib::glib_int
glib::plane<T>::last_non_zero() const {
	if (_real_height!=0) {
		for (u_glib_int i = _real_height - 1; i != 0; --i) {
			if (!_intervals[i].is_empty())
				return i+_start_height;
		}
		return _start_height-1;
	} else {
		return 0;
	}
}

template <class T> glib::glib_int
glib::plane<T>::get_start_height() const {
	return _start_height;
}

template <class T> glib::glib_int
glib::plane<T>::get_real_height() const {
	return _real_height;
}

template <class T> glib::glib_int
glib::plane<T>::get_end_height() const {
	return _end_height;
}

template <class T> glib::glib_int
glib::plane<T>::get_pivot_width() const {
	return _pivot_width;
}

template <class T> glib::plane<T>
glib::plane<T>::move(const glib::glib_int pivot_width, const glib::glib_int start_height) {
	plane<T> res = plane<T>(*this);
		//neni overloadnuty CC - neni se tu ceho bat, neni tu zadne pole
		//(stromova struktura intervals sama sebe umi zkopirovat)
	
	glib_int height_diff = (start_height - _start_height); //- kdyz nahoru, + kdyz dolu
	glib_int width_diff = (_pivot_width - pivot_width);
	
	res._start_height += height_diff;
	res._end_height += height_diff;
		//posouvame vysku
	
	for (size_t i = 0; i < _real_height; ++i) {
		res._intervals[i].move(width_diff);
	}
		//posouvame sirku
	return res; //ta-taa
}


template<class T>
glib::plane<T>::plane() : _real_height(0), _end_height(0), _start_height(0), _pivot_width(0), _intervals() {}

template<class T>
glib::plane<T>::plane(const glib::glib_int start_height, const glib::glib_int end_height, const glib::glib_int pivot_width)
  : _start_height(start_height), 
  _end_height(__maximum(start_height, end_height)),	
		//nechce se mi hazet vyjimka - pokud dam mensi vysku, proste bude vektor velky 0, chybu by to hodit nemelo

  _real_height(__maximum(start_height, end_height) - start_height), 
		//tady mozna bude 0

  _intervals(__maximum(start_height, end_height) - start_height),
		//defaultni konstruktor vektoru

  _pivot_width(pivot_width) { 
}

template<class T>
glib::plane<T>::plane(const glib::glib_int start_width, const glib::glib_int end_width, const glib::glib_int start_height, const glib::glib_int end_height, const T& what)
  : _start_height(start_height),
  _end_height(__maximum(start_height, end_height)), 
  _real_height(__maximum(start_height, end_height) - start_height),
  _intervals(__maximum(start_height, end_height) - start_height, intervals<T>(start_width, end_width-1, what)),
		//totez co minule, jen tady rovnou do intervalu nastrkam hodnoty
  _pivot_width(start_width) {
}

template<class T> void 
glib::plane<T>::set(const glib::glib_int x, const glib::glib_int y, const T& what) {
	if ((y >= _end_height) || (y < _start_height)) {
		return;
	}
		//odolne proti chybam - prijde mi to uzitecnejsi
	_intervals[y-_start_height].set(x, what);
	
}

template<class T> void  
glib::plane<T>::set_more(const glib::glib_int start_x, const glib::glib_int end_x, const glib::glib_int y, const T& what) {
	if ((y >= _end_height) || (y < _start_height)) {
		return;
	}
	if (end_x > start_x)
		_intervals[y-_start_height].set_more(start_x, end_x-1, what);
		//opet, pri zadani blbosti se nestane nic
}

template<class T> void  
glib::plane<T>::add(const plane<T>& other) {
	
	glib_int start = __maximum(_start_height, other._start_height);
	glib_int end = __minimum(_end_height, other._end_height);
		//start a end jsou minimalni hranice, co musim zkoumat
		//POZOR - radky, co jsou nad this->_start_height a pod this->_end_height se ignoruji!
	
	for (glib_int i = start; i < end; ++i) {
		u_glib_int it = i - _start_height;
		u_glib_int oth_it = i - other._start_height;
			//beru primo z vektoru, bez get_row, musim vedet jak svuj index (it) tak index toho druheho (oth_it)
		_intervals[it].add(other._intervals[oth_it]);
	}
}


template<class T> 
typename glib::plane<T>::T_list 
glib::plane<T>::get_row(glib::glib_int y) const {
	if ((y >= _end_height) || (y < _start_height)) {
		return T_list();
	}
	return _intervals[y-_start_height].get_all();
		//opet, odolnost proti chybam, jinak se vse prenasi na intervals[]
}


template<class T> T 
glib::plane<T>::get(const glib::glib_int x, const glib::glib_int y) const {
	if ((y >= _end_height) || (y < _start_height)) {
		return T();
	}
	return _intervals[y-_start_height].get(x);
		//vsechno prenasi jinam
}


template<class T>   
glib::plane<T>::plane(const plane<bool>& other, const T& what)
  : _start_height(other.get_start_height()),
  _end_height(other.get_end_height()), 
  _real_height(other.get_real_height()),
  _intervals(other.get_real_height()),
		//pozor - tenhle neni CC, ale jenom nastavi velikost

  _pivot_width(other.get_pivot_width()) {
	  
	  
	for (glib_int i = _start_height; i < _end_height; ++i) {
		bool_list row = other.get_row(i);
			//co ze vlastne chci.
		for (bool_list::const_iterator iter = row.begin(); iter != row.end(); ++iter) {
			if (iter->_cont == true) {
				//teoreticky muze byt i false.
				//TODO: zkusit to napsat jako void
				_intervals[i - _start_height].set_more(iter->_start, iter->_end, what);
			}
		}
	}
}

#endif
