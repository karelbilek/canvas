#ifndef PL_INC
#define PL_INC

#include "interval.h" //kazdy radek je interval

#define __real_height (_end_height - _start_height)

namespace libcan {
	
	struct libcan_info {
		long min_x;
		long max_x;
		long y;
	};
	

	template<class T>
	class plane {
		/* 
		 * Trida na ukladani pixelu, co nezabira zbytecne mnoho mista a pritom je pomerne rychla.
		 * Kazdy radek je ulozen jako "interval" - struktura na ukladani intervalu - vice info o te strukture tam.
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
		 * vic o intervalech v interval.h (snad)
		 */
	public:
		typedef interval<T>* T_intervals;
		typedef std::list<interval_content<T> > int_cont;
		
	private:	
		
		
		long _start_height; //zacatecni vyska
		long _end_height; //konecna vyska
		
		interval<T>* _intervals;
		
	public:
		long _pivot_width; 
		long _pivot_height;
		
		typedef std::list<interval_content<T> > T_list;
			//list "obsahu" - obsah struktura T, zacatek, konec
		
		//typedef std::list<interval_content<bool> > bool_list;
			//totez special pro bool - hodi se pro jeden konstruktor
		
		plane();
		
		plane(const plane<T>& other);
		
		plane(const long start_height, const long end_height, const long pivot_width=0);
			//konstruktor, co chce pocatecni a konecnou vysku a pivotni sirku a neprida nikam nic
		
		plane(const long start_width, const long end_width, const long start_height, const long end_height, const T& what);
			//konstruktor, co se mu jeste da zacatek a konec kazdeho radku a on do kazdeho prida velky interval s danou hodnotou
	
	
		~plane();
	
		template <class U>
		plane<U> flatten_plane(const U& what, const T& min) const;
		
		long get_start_height() const;
		long get_end_height() const;
		long get_real_height() const;
		T_intervals get_intervals() const;
		
		long get_pivot_height() const {return _pivot_height;}
		long get_pivot_width() const {return _pivot_width;}
		
			//klasicke gettery
			
		std::vector<libcan_info> all_infos() const;
		
		long first_non_zero() const;
		long last_non_zero() const;
		
		long most_left(long y) const;
		long most_right(long y) const;
		
		long most_top_left() const;
		long most_down_right() const;
		
		long most_left() const;
		long most_right() const;
		
		bool is_empty() const;
		
		T_list get_row(const long y) const;
			//vrati konkretni radek v listu paru <T, rozsah>
			
		void set_whole_interval(const long y, const interval<T> new_interval);
			//vlaste pomerne troufale, ale co :)
		
		T get(const long x, const long y) const;
			//vrati hodnotu na konkretni pozici, nebo defaultni T(), pokud tam nic neni
		
		void add(const long x, const long y, const T& what);
			//nastavi konkretni hodnotu (pozor, SCITA!)
		
		void add_more(const long start_x, const long end_x, const long y, const T& what);
			//nastavi na y radku vse od start_x do end_x na what (tj, prida interval, mozna neco upravi)
		
		plane<T> move_relative(const long x, const long y);
		
		plane<T> half(const T& background, const long& max_x);
		
		plane<T> move(const long pivot_width, const long pivot_height);
			//posune cely plane, aby mel "novy" pivot_width a start_height
			//pozor, sam se sebou neposune, ale posunty VRACI
		
		void add(const plane<T>& other);
			//slouci s jinou plochou 
			//POZOR - z jine plochy vezme jen veci od _start_height po _end_height, zbytek ignoruje
		
		void selective_replace(const plane<T>& other, const plane<bool>& where);
		
		plane<T>& operator=(const plane<T>& other);
		
		bool includes_square(const long min_x, const long min_y, const long max_x, const long max_y) const;
		
		plane<T> negative(const T& what, const long min_x, const long max_x);
		
		bool is_all_same(const T& what) const;
	};
	
	template <class T> 
	plane<T> 
	plane<T>::half(const T& background, const long& max_x) {
		plane<T> res(_start_height/2, _end_height/2, _pivot_width/2);
		for (int i = 0; i<__real_height/2; ++i) {
			interval<T>* nw = _intervals[i*2].quarter(_intervals[i*2+1], background, max_x);
			res._intervals[i] = *nw;
			delete nw;
		}
		return res;
	}
	
	
	template <class T> 
	bool
	plane<T>::is_all_same(const T& what) const {
		for (int i = 0; i < __real_height; i++) {
			if (!_intervals[i].is_all_same(what)) {
				return false;
			}
		}
		return true;
	}


	//--------------------------------CONSTRUCTORS
	
	
	template<class T>
	plane<T>::plane() :
	  _start_height(0),
	  _end_height(0),
	  _intervals(NULL),
	  _pivot_width(0),
	  _pivot_height(0) {
	}
	
	template<class T>
	plane<T>::plane(const long start_height, const long end_height, const long pivot_width) :
	  _start_height(start_height), 
	  _end_height(std::max(start_height, end_height)),	
	  _intervals(new interval<T>[__real_height]),
	  _pivot_width(pivot_width),
	  _pivot_height(0){
	}
	
	template<class T>
	plane<T>::plane(const libcan::plane<T>& other) :
	  _start_height(other._start_height), 
	  _end_height(other._end_height),	
	  _intervals(new interval<T>[__real_height]),
	  _pivot_width(other._pivot_width),
	  _pivot_height(other._pivot_height) {
		
		long rs = __real_height;
		for (long i = 0; i < rs; ++i) {
			_intervals[i]=other._intervals[i];
		}
		
	}
	
	
	
	
		//cely zaplni whatem
	template<class T>
	plane<T>::plane(const long start_width, const long end_width, const long start_height, const long end_height, const T& what) : 
	  _start_height(start_height),
	  _end_height(std::max(start_height, end_height)), 
	  _intervals(new interval<T>[__real_height]),
	  _pivot_width(0),
	  _pivot_height(0) {
		
		long rs = __real_height;
		for (long i = 0; i < rs; ++i) {
			_intervals[i]=interval<T>(start_width, end_width-1, what);
		}
	}
	
	//-------------------------------DESTRUCTORS
	template<class T>
	plane<T>::~plane() {
		delete [] _intervals; 
	}
	
	//--------------------------------GETTERS
	
	
	template <class T> 
	std::vector<libcan_info>
	plane<T>::all_infos() const {
		
		std::vector<libcan_info> res;
		
		for (long i = 0; i < __real_height; ++i) {
			int_cont ob = _intervals[i].get_all();
			typename int_cont::const_iterator iter;
			for (iter=ob.begin() ; iter != ob.end(); ++iter ) {
				libcan_info newinfo;
				newinfo.y = i + _start_height;
				newinfo.min_x = iter->_start;
				newinfo.max_x = iter->_end;
				res.push_back(newinfo);
			}
		}
		return res;
	}
	
	template <class T> 
	bool
	plane<T>::is_empty() const {
		for (long i = 0; i < __real_height; ++i) {
			if (! _intervals[i].is_empty()) {
				return false;
			}
		}
		return true;
	}
	
	template <class T> 
	long
	plane<T>::most_left() const {
		long min = LONG_MAX;
		for (long i = first_non_zero(); i <= last_non_zero(); ++i) {
			min = std::min(min, _intervals[i-_start_height].most_left());
		}
		return min;
	}

	template <class T> 
	long
	plane<T>::most_right() const {
		long max = 0;
		for (long i = first_non_zero(); i <= last_non_zero(); ++i) {
			max = std::max(max, _intervals[i-_start_height].most_right());
		}
		return max;
	}

	template <class T>
	long
	plane<T>::most_top_left() const {
		return most_left(first_non_zero());
	
	}


	template <class T> 
	long
	plane<T>::most_down_right() const {
		return most_right(last_non_zero());
	}

	template <class T> 
	long
	plane<T>::most_left(long y) const {
		return _intervals[y-_start_height].most_left();
	}

	template <class T> 
	long
	plane<T>::most_right(long y) const {
		return _intervals[y-_start_height].most_right();
	}

	template <class T> 
	long
	plane<T>::first_non_zero() const {
		for (long i = 0; i < __real_height; ++i) {
			if (!_intervals[i].is_empty())
				return i+_start_height;
		}
		return _end_height;
	}
	

	template <class T> 
	long
	plane<T>::last_non_zero() const {
		if (__real_height!=0) {
			for (unsigned long i = __real_height - 1; i != 0; --i) {
				if (!_intervals[i].is_empty())
					return i+_start_height;
			}
			return _start_height-1;
		} else {
			return 0;
		}
	}

	template <class T> 
	long
	plane<T>::get_start_height() const {
		return _start_height;
	}

	template <class T> 
	long
	plane<T>::get_real_height() const {
		return __real_height;
	}

	template <class T> long
	plane<T>::get_end_height() const {
		return _end_height;
	}

	
	template <class T> 
	typename plane<T>::T_intervals
	plane<T>::get_intervals() const {
		return _intervals;
	}
	
	
	template<class T> 
	typename plane<T>::T_list 
	plane<T>::get_row(long y) const {
		if ((y >= _end_height) || (y < _start_height)) {
			return T_list();
		}
		return _intervals[y-_start_height].get_all();
			//opet, odolnost proti chybam, jinak se vse prenasi na intervals[]
	}
	
	template<class T>
	bool
	plane<T>::includes_square(const long min_x, const long min_y, const long max_x, const long max_y) const {
		if (min_y < _start_height) {
			return false;
		}
		
		if (max_y > _end_height) {
			return false;
		}
		
		for (int y = min_y; y < max_y; ++y) {
			if (!_intervals[y-_start_height].includes(min_x, max_x)) {
				return false;
			}
		}
		
		return true;
	}
	
	//--------------------------------------SETTERS
	
	template<class T>
	void
	plane<T>::selective_replace(const plane<T>& other, const plane<bool>& where){
		interval<bool>* its = where.get_intervals();
		for (int i = 0; i < __real_height; ++i) {
			_intervals[i].selective_set(other._intervals[i], its[i]);
		}
	}
	
	template<class T>
	plane<T>&
	plane<T>::operator=(const libcan::plane<T>& other) {
		_pivot_width = other._pivot_width;
		_pivot_height = other._pivot_height;
		
		_start_height = other._start_height;
		_end_height=other._end_height;
		delete [] _intervals;
		
		long rs = __real_height;
		_intervals=new interval<T>[__real_height];
		
		for (long i = 0; i < rs; ++i) {
			_intervals[i]=other._intervals[i];
		}
		return *this;
	}
	
	template<class T>
	void 
	plane<T>::set_whole_interval(const long y, const interval<T> new_interval) {
		if (y<=__real_height) {
			_intervals[y]=new_interval;
			//tohle trva, bohuzel, pomerne dlouho
		}
	}
	
	template<class T> 
	void 
	plane<T>::add(const long x, const long y, const T& what) {
		if ((y >= _end_height) || (y < _start_height)) {
			return;
		}
			//odolne proti chybam - prijde mi to uzitecnejsi
		_intervals[y-_start_height].add_one(x, what);
	
	}

	template<class T> void  
	libcan::plane<T>::add_more(const long start_x, const long end_x, const long y, const T& what) {
		if ((y >= _end_height) || (y < _start_height)) {
			return;
		}
		if (end_x >= start_x)
			_intervals[y-_start_height].add_more(start_x, end_x, what);
			//opet, pri zadani blbosti se nestane nic
	}

	template<class T> void  
	libcan::plane<T>::add(const plane<T>& other) {
	
		long start = std::max(_start_height, other._start_height);
		long end = std::min(_end_height, other._end_height);
			//start a end jsou minimalni hranice, co musim zkoumat
			//POZOR - radky, co jsou nad this->_start_height a pod this->_end_height se ignoruji!
	
		for (long i = start; i < end; ++i) {
			unsigned long it = i - _start_height;
			unsigned long oth_it = i - other._start_height;
				//beru primo z vektoru, bez get_row, musim vedet jak svuj index (it) tak index toho druheho (oth_it)

			_intervals[it].add_another(other._intervals[oth_it]);
		
		}
	}
	
	
	template<class T> T 
	plane<T>::get(const long x, const long y) const {
		if ((y >= _end_height) || (y < _start_height)) {
			return T();
		}
		return _intervals[y-_start_height].get(x);
			//vsechno prenasi jinam
	}
	
	//----------------------------------------OTHER
	
	template <class T>
	plane<T> 
	plane<T>::negative(const T& what, const long min_x, const long max_x) {
		plane<T> result(_start_height, _end_height, _pivot_width);
		
		int rs = __real_height;
		for (int i=0; i<rs; ++i) {
			interval<T>* p_int = (_intervals[i]).negative(what, min_x, max_x);
			if (p_int!=NULL) {
				result.set_whole_interval(i, *p_int);
			}
			delete p_int;
		}
		
		return result;
	}
	
	template <class T> 
	plane<T>
	plane<T>::move_relative(const long x, const long y) {
		return move(_pivot_width + x, _pivot_height + y);
	}
	
	template <class T> 
	plane<T>
	plane<T>::move(const long new_width, const long new_height) {
		plane<T> res = plane<T>(*this);
			//neni overloadnuty CC - neni se tu ceho bat, neni tu zadne pole
			//(stromova struktura intervals sama sebe umi zkopirovat)
	
		
		long height_diff = (new_height - _pivot_height); //- kdyz nahoru, + kdyz dolu
		long width_diff = (new_width - _pivot_width);
				
		res._start_height = res._start_height + height_diff;
		res._end_height = res._end_height + height_diff;
			//posouvame vysku
		
		res._pivot_width = new_width;
		res._pivot_height = new_height;
	
		for (long i = 0; i < __real_height; ++i) {
			res._intervals[i].move(-width_diff);
		}
			//posouvame sirku
		return res; //ta-taa
	}
	
	
	template<class T>
	template<class U>   
	plane<U>
	plane<T>::flatten_plane(const U& what, const T& min) const {
		
		plane<U> result(_start_height, _end_height, _pivot_width);
		
		int rs = __real_height;
		for (int i=0; i<rs; ++i) {
			interval<U>* p_int = (_intervals[i]).template flatten_interval<U>(what, min);
			result.set_whole_interval(i, *p_int);
			delete p_int;
				//tohle vypada hrozne, ale tady by mel ve vetsine pripadu vylezt interval,
				// co bude mit velmi malo potomku, jestli vubec nejake
		}
		
		return result;
	}
	

	




}

#endif
