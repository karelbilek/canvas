#ifndef INTERVAL_INC
#define INTERVAL_INC

#include "types.h"

namespace libcan {
	

	
	
	template <class T>
	struct interval_content {
		T _cont;
		libcan_int _start;
		libcan_int _end;
		interval_content(const T& cont, libcan_int start, libcan_int end): _cont(cont), _start(start), _end(end){}
	};



	template<class T>
	class interval {
		/*
		 * Takovy mozna zvlastni zpusob ukladani intervalu - pomoci BVS, ktery je ale trochu upraven.
		 * Kazdy vrchol ma svoji levou a pravou hodnotu, intervaly se nikdy nekrizi.
		 * Pokud pridavame doleva od leveho nebo doprava od praveho, nic se nedeje, "klasika" BVS.
		 * 
		 * Pokud pridavame "doprostred", mame problem - vytvorime tedy tri ruzne intervaly, 
		 * jeden vlevo, jeden vpravo, s upravenymi zacatky/konci,
		 * a hlavne, jeden veprostred, co bude soucet "stareho" s novym.
		 *
		 * Pokud T nema definovanou funkci soucet, je to blbe, asi se ten kod nezkompiluje.
		 * Stejne jako je potreba, aby na T bylo definovano == a taky constructor().
		 *
		 * ten BVS zatim NENI vyvazovany, a jelikoz tohle zabira v canvasu stejne minimum casu, asi nikdy 
		 * nebude.
		 */
	public:
		typedef std::list<interval_content<T> > contents;
			//pouze pro vraceni vysledku (nebudu vracet celou strukturu)
	protected:
		
		
		libcan_int _start;
			//Zacatek intervalu
		libcan_int _end; 
			//Konec intervalu - vcetne!!! 
		
		T _content;
			//Obsah intervalu
		
		interval<T>* _left;
			//levy podstrom
		interval<T>* _right;
			//Pravy podstrom
			
		bool _empty;
			//jediny "empty" muze byt root! dalsi uz jsou NULLy
			
			
			
		
		contents recur_all() const;
			//vrati obsahy vsech svych deti, *ale ne* sebe

		

	public:
		
		
		libcan_int most_left() const;
		libcan_int most_right() const;
		libcan_int get_start() const;
		libcan_int get_end() const;
		
		T get_this() const;
		
		const interval<T> get_left() const;
		const interval<T> get_right() const;
		
		void add_left(interval<T>* const other);
		void add_right(interval<T>* const other);
		
		bool has_left() const;
		bool has_right() const;
		bool is_empty() const;
		
		bool includes(const libcan_int from, const libcan_int to) const;
		
		
		interval (const libcan_int start, const libcan_int end, const T& content);
			//"klasicky" konstruktor
		interval (const interval<T>& other);
			//zkopiruje z dalsiho nodu
		
			
		interval ();
			//prazdny
		
		T get(libcan_int where) const;
			//najde danou pozici - pokud ne, da T()
		
		
		void move(libcan_int sirka);
			//posune sebe i synky o sirka doLEVA
		
		~interval();
			//Smaze svoje deti
		
		
		// void recur_check();
		void check();

		
		contents get_all() const;
			//vrati obsahy vsech svych deti, *vcetne* sebe
			
		void selective_set(const interval<T>& what, const interval<bool>& where);
		
		void reduce(const interval<bool>& how, interval<T>& res) const;
		interval<T>* reduce_one(libcan_int how_start, libcan_int how_end) const;
		interval<T>* remove_all(const T& what);
		
		void set_more(const libcan_int start, const libcan_int end, const T& what);
		void set_one(const libcan_int where, const T& what);
			//jenom zavola add_more/one s add=0
		
		
		void add_more(const libcan_int start, const libcan_int end, const T& what, int add=1);
			//prida novy interval - pokud se s nicim nekryje. Pokud ano, vse se upravi.
		
		void add_one(const libcan_int where, const T& what, int add=1);
			//Prida jeden "bod" - napr. mam interval 4-8, pridam 3, interval se zmeni na 3-8.
			//Tady je videt to mysleni "po pixelech" - ano, je to diskretni :)
		
		void add_another(const interval<T>& other, int add=1);
		
		
		interval<T>* add_far_right(interval<T>* other);
		
		interval<T>* half(bool& left, const bool on_right) const;
		
		
		interval<T>* quarter(interval<T> other, const T& background, const libcan_int& max_x) const;
		
		
		interval<T>& operator=(const interval<T>& other);
			//vrati kopii
		
		interval<T>* negative(const T& what, const libcan_int min_x, const libcan_int max_x);
		
			
		template <class U>
		interval<U>* flatten_interval(const U& what, const T& min) const;
		
		bool is_all_same(const T& what) const;
	};
	
	template <class T> 
	interval<T>* 
	interval<T>::remove_all(const T& what) {
		if (_left!=NULL) {
			_left = _left->remove_all(what);
		}
		if (_right!=NULL) {
			_right = _right->remove_all(what);
		}
		
		if (what==_content) {
			if (_left==NULL) {
				if (_right == NULL) {
					delete this; //rychle pryc odsud!
					return NULL;
				} else {
					_start = _right->_start;
					_end = _right->_end;
					_content = _right->_content;
					_left = _right->_left;
					interval<T>* r = _right->_right;
					_right->_right = NULL;
					_right->_left = NULL; //kvuli destrctru
					delete _right;
					_right = r;
					return this;
				}
			} else {
				if (_left == NULL) {
					_start = _left->_start;
					_end = _left->_end;
					_content = _left->_content;
					_right = _left->_right;
					interval<T>* l = _left->_left;
					_left->_right = NULL;
					_left->_left = NULL; //kvuli destrctru
					delete _left;
					_left = l;
					return this;
				} else {
					//nejhorsi varianta
					interval<T>* r = _left->add_far_right(_right);
					_left = NULL;
					_right = NULL;
					delete this;
					return r;
				}
			}
		} else {
			return this;
		}
	}
	
	template <class T> 
	interval<T>*
	interval<T>::quarter(interval<T> other, const T& background, const libcan_int& max_x) const{
		interval<T> added = *this;
		//added.add_another(interval<T>(0, max_x, background), 3);
		//other.add_another(interval<T>(0, max_x, background), 3);
		
		added.add_another(other, 2);
		added.check();
		bool l;
		//interval<T>* res = new interval<T>(added);
		interval<T>* res = added.half(l, false);
		res = res->remove_all(background);
		if (res==NULL) {
			res = new interval<T>();
		}
		return res;
	}
	
	template <class T> 
	interval<T>*
	interval<T>::half(bool& left, const bool on_right) const {
		bool l;
		libcan_int start = (_start % 2) ?(_start/2+1):(_start/2);
		libcan_int end = _end / 2;
		left = (_start%2);
		libcan_int where = (_start / 2);
		
		
		if (end < start) {
			//pouze, kdyz mam 2 stejna licha cisla
			if (_left==NULL) {				
				if (_right==NULL) {
					
					if (!on_right) {
						
						left = false;
						return new interval<T>(end, end, _content);
					} else {
						
						return NULL;
					}
				} else {
					
					//bool add_left = false;
					interval<T>* res = _right->half(l, true);
					if (l==true) {
						throw 1;
					}
					if (!on_right) {
						
						left = false;
						res->add_one(where, _content, 2);
					} 	
									
					return res;
				}
			} else {
				
				interval<T>* left_res = _left->half(l, false);
				left_res->add_one(where, _content, 2);	
				left = false;			
				if (_right == NULL) {
					
					return left_res;
				} else {
					
					interval<T>* right_res = _right->half(l, true);
					if (l==true) {
						throw 1;
					}
					return left_res->add_far_right(right_res);
				}
			}
		} else {
			
			interval<T>* res = new interval<T>(start, end, _content);
			if (_left != NULL) {
				interval<T>* left_res = _left->half(l, false);
				res->_left = left_res;
			}
			if (_right != NULL) {
				bool add_right;
				interval<T>* right_res = _right->half(add_right, true);
				res->_right = right_res;
				if (add_right) {
					res->add_one(_right->_start/2, _right->_content, 2);
				}
			}
			if ((!on_right || res->_left!=NULL) && left) {
				left = false;
				res->add_one(where, _content, 2);			
			}
			return res;
		}
	}
	
	template<class T>
	bool
	interval<T>::is_all_same(const T& what) const {
		if (_content == what) {
			if (_left!=NULL) {
				bool r = _left->is_all_same(what);
				if (r) {
					return false;
				}
			}
			if (_right!=NULL) {
				bool r = _right->is_all_same(what);
				if (r) {
					return false;
				}
			}			
			return true;
		} else {
			return false;
		}
	}
	
	template<class T>
	void
	interval<T>::reduce(const interval<bool>& how, interval<T>& res) const {
		interval<T>* reduced = reduce_one(how.get_start(), how.get_end());
		if (reduced!=NULL) {
			res.add_another(*reduced);
		}
		delete reduced;
		if (how.has_left()) {
			reduce(how.get_left(), res);
		}
		if (how.has_right()) {
			reduce(how.get_right(), res);
		}
	}
	
	template<class T>
	interval<T>* 
	interval<T>::reduce_one(libcan_int how_start, libcan_int how_end) const {
		if (_start <= how_start) {
			if (_end < how_start) {
				if (_right!=NULL) {
					return _right->reduce_one(how_start, how_end);
				} else {
					return NULL;
				}
			} else if (_end <= how_end) {
				interval<T>* res = new interval<T>(how_start, _end, _content);
				if (_right!=NULL && _end<how_end) {
					res->_right = _right->reduce_one(_end+1, how_end);
				}
				return res;
			} else {
				//_end > how_end
				return new interval<T>(how_start, how_end, _content);
			}
		} else if (_start<=how_end) {
			if (_end <= how_end) {
				interval<T>* res = new interval<T>(_start, _end, _content);
				if (_left!=NULL) {
					res->_left = _left->reduce_one(how_start, _start-1);
				}
				if (_right!=NULL && how_end > _end) {
					res->_right = _right->reduce_one(_end+1, how_end);
				}
				return res;
			} else {
				interval<T>* res = new interval<T>(_start, how_end, _content);
				if (_left!=NULL) {
					res->_left = _left->reduce_one(how_start, _start-1);
				}
				return res;
			}
		} else {
			if (_left!=NULL) {
				return _left->reduce_one(how_start, how_end);
			} else {
				return NULL;
			}
		}
	}
	
	template<class T>
	void 
	interval<T>::selective_set(const interval<T>& what, const interval<bool>& where){
		interval<T> reduced;
		what.reduce(where, reduced);
		add_another(reduced, 0);
	}
	
	template<class T>
	interval<T>*
	interval<T>::add_far_right(interval<T>* other) {
		if (_right==NULL) {
			_right=other;
			return this;
		} else {
			interval<T>* finding = this;
			while (finding->_right->_right != NULL) {
				finding = finding->_right;
			}
			interval<T>* new_head = finding->_right;
			finding->_right = finding->_right->_left;
			
			new_head->_left = this;
			new_head->_right = other;
			return new_head;
		}
	}
	
	template<class T>
	interval<T>* 
	interval<T>::negative(const T& what, const libcan_int min_x, const libcan_int max_x) {
		
		if ((_start <= min_x) && (_end >= max_x)) {
			return NULL;
		} else if (_start <= min_x) {
			//jsem-li vlevo...
			
			libcan_int bigger_left = __maximum((_end+1), min_x);
			if (_right == NULL) {
				return new interval<T>(bigger_left, max_x, what);
			} else {
				
				return _right->negative(what, bigger_left, max_x);
			}
			
		} else if (_end >= max_x) {
			
			//jsem-li vpravo
			libcan_int smaller_right = __minimum(max_x, (_start-1));
			
			if (_left == NULL) {
				return new interval<T>(min_x, smaller_right, what);
			} else {
				return _left->negative(what, min_x, smaller_right);
			}
		} else {
			//jsem-li vprostred..
			libcan_int on_left = _start - 1;
			libcan_int on_right = _end + 1;
			interval<T>* left_side;
			interval<T>* right_side;
			
			if (_left!=NULL) {
				left_side = _left->negative(what, min_x, on_left);
			} else {
				left_side = new interval<T>(min_x, on_left, what);
			}
			
			if (_right!=NULL) {
				right_side = _right->negative(what, on_right, max_x);
			} else {
				right_side = new interval<T>(on_right, max_x, what);
			}
			
			if (left_side == NULL) {
				return right_side;
			}
			if (right_side == NULL) {
				return left_side;
			}
			interval<T>* res = left_side->add_far_right(right_side);
			res->check();
			return res;
		}
	}
	
	template<class T>
	bool 
	interval<T>::includes(const libcan_int from, const libcan_int to) const {
		//paradoxne, to co jde primo "na me" ignoruju
		
		if (from < _start) {
			if (_left == NULL) {
				return false;
			} else {
				if(!_left->includes(from,_start-1)) {
					return false;
				}
			}
		}
		if (to > _end) {
			if (_right==NULL) {
				return false;
			} else {
				if (!_right->includes(_end+1,to)) {
					return false;
				}
			}
		}
		return true;
	}
	
	

	//----------------CONSTRUCTORS
	template<class T>
	interval<T>::interval (const interval<T>& other) : 
	  _start(other._start), 
	  _end(other._end),
	  _content(other._content),
	  _left((other._left!=NULL)?(new interval<T> (*(other._left))):(NULL)),
	  _right((other._right!=NULL)?(new interval<T> (*(other._right))):(NULL)),
	  _empty(other._empty) { 
	}

	template<class T>
	interval<T>::interval (const libcan_int start, const libcan_int end, const T& content): 
	  _start(start), 
	  _end(__maximum(end,start)),
	  _content(content),
	  _left(NULL),
	  _right(NULL),
	  _empty(false) { 
	}
	
	template<class T>
	interval<T>::interval ():
	  _start(0),
	  _end(0),
	  _content(T()),
	  _left(NULL),
	  _right(NULL),
	  _empty(true) {

	}	
	
	
	//---------------------DESTRUCTOR
	
	template<class T>
	interval<T>::~interval() {
		if (_left!=NULL) {
			delete _left;
			_left = NULL;
		}
		if (_right!=NULL) {
			delete _right;
			_right = NULL;
		}
	}


	//----------------------------SETTERS
	
	
	template<class T>
	void 
	interval<T>::add_left(interval<T>* const other) {
		if (_left==NULL) {
			_left = other;
		} else {
			throw 1;
		}
	}
	
	template<class T>
	void 
	interval<T>::add_right(interval<T>* const other) {
		if (_right==NULL) {
			_right = other;
		} else {
			throw 1;
		}
	}
	
	template<class T>void 
	interval<T>::add_another(const interval<T>& other, int add) {
		if (!other._empty) {
			add_more(other._start, other._end, other._content, add);
			if (other._left!=NULL) {
				add_another(*other._left, add);
			} 
			if (other._right!=NULL) {
				add_another(*other._right, add);
			}
		}
		
	}
	
	
	template <class T>
	template <class U>
	interval<U>* 
	interval<T>::flatten_interval(const U& what, const T& min) const {
		if (_content >= min ) {
				//jednodussi varianta
			interval<U>* result = new interval<U>(_start, _end, what);
			
			if (_left!=NULL) {
				result->add_left(_left->flatten_interval<U>(what, min));
			}
			if (_right!=NULL) {
				result->add_right(_right->flatten_interval<U>(what, min));
			}
			result->check();
			return result;
		} else {
				//trva mnohem dyl :/ horsi varianta
			if (_left!=NULL) {
				interval<U>* novy = _left->flatten_interval<U>(what, min);
				if (_right!=NULL) {
					novy->add_another(*(_right->flatten_interval<U>(what, min)));
					novy->check();
				}
				return novy;
			} else if (_right != NULL) {
				interval<U>* novy = _right->flatten_interval<U>(what, min);
				novy->check();
				return novy;
			} else {
				interval<U>* novy= new interval<U>();
				return novy;
			}
		}
	}
	
	
	template<class T> interval<T>&
	interval<T>::operator=(const interval<T>& other) {
	
		
	
		if (_left != NULL) {
			delete _left; 
			_left = NULL;
		}
		if (_right != NULL) {
			delete _right;
			_right = NULL;
		}
		
		if (other._empty) {
			_empty = true;
		} else {
			_empty = false;
			_start = other._start;
			_end = other._end;
			_content = other._content;
			
			if (other._left != NULL) {
				//pokud je, zustane NULL z minula, vse ok
				_left = new interval(*other._left);
			}
			
			if (other._right != NULL) {
				_right = new interval(*other._right);

			}
		}
		return *this;
	}
	
	template<class T>
	void 
	interval<T>::move(libcan_int sirka) {
		if (!_empty) {
			if (_left!=NULL) {
				_left->move(sirka);
			}
			if (_right!=NULL) {
				_right->move(sirka);
			}
			_start -= sirka;
			_end -= sirka;
				//pozor, je tam minus!!!
		}
		
	}
	
	template<class T>
	void 
	interval<T>::set_one(const libcan_int where, const T& what) {
		add_more(where, what, 0);	
	}
	
	template<class T>
	void 
	interval<T>::set_more(const libcan_int start, const libcan_int end, const T& what) {
		add_more(start, end, what, 0);	
	}
	
	template<class T>
	void 
	interval<T>::add_more(const libcan_int start, const libcan_int end, const T& what, int add) {
		
		T new_mixed;
		if (add == 0){
			new_mixed = what;
		} else if (add == 1) {
			new_mixed = (_content + what);
		} else if (add == 2) {
			new_mixed = _content * what;
		} else if (add == 3) {
			new_mixed = _content;
		}
		
		
		if (_empty) {
			_empty= false;
			_start = start;
			_end = end;
			_content = what;
			_left = NULL;
			_right = NULL;
		} else {
			if (start < _start) {
				
				libcan_int smaller = __minimum (_start-1, end);
					//Chceme vlozit neco, co zacina driv
					//Uvazuju ale jenom to, co "nekouka" pripadne prese mne - to pak poresim.
					//Pokud je situace takhle:
					//       =====
					// --
					//situace je ALL OK
		
					//pokud je to ale 
					//		=====
					//  -------
		
					//musi se to resit dal, protoze tady se do "smaller" da start-1, tj. tesne pred zacatkem ==
		
				if (_left == NULL) {
					_left = new interval<T> (start, smaller, what);
				} else {
					_left -> add_more(start, smaller, what,add);
						//pokud vlevo neco je, musime to poslat jemu
						
					_left -> check();
						//nechci, aby checku bylo moc, tak se spusti pouze kdyz si myslim, ze je potreba :)
				}
			}
			if (end > _end) {
				//totez v bledemodrem
				
				libcan_int bigger = __maximum(_end+1, start);
				if (_right == NULL) {
					
					_right = new interval<T> (bigger, end, what);
				} else {
					_right -> add_more(bigger, end, what,add);
					_right -> check();
				}
			}
	
			//Uspesne jsem vyresil to, co se deje pred === a za nim, ted ale musim doresit pripadne prekryvy
			if (start <= _start && end >= _end) {
					//    =========
					//  --------------
					//ten zacatek a konec je doresen, zbyva jenom zmichat sam sebe s tim, co prislo
				_content = new_mixed;
		
			} else if (start <= _start && end < _end && end >= _start) {
					//      =============
					//   -------
					//musim poresit cast vlevo, udelam to tak, ze sam sebe posunu doprava a
					//pridam "novou" levou cast - je to kvuli tomu, aby se mohla pripadne checknout a 
					//sloucit s necim, co uz je vlevo
					
				
				libcan_int b = _start;
				_start = end + 1;
				add_more(b, end, new_mixed,0);//sic
		
		
			} else if (start > _start && end >= _end && start <= _end) {
				//       ==========
				//           ---------
				

				libcan_int e = _end;
				_end = start - 1;
				add_more(start, e, new_mixed,0);//sic
				
				
		
			} else if (start > _start && end < _end) {
					// je tam ostra!
					// ===========
					//      ---
					//3 kousky
					
				
				libcan_int old_end = _end;
				libcan_int old_start = _start;
				T old_content = _content;
				
				_start = start;
				_end = end;
				
				
				_content = new_mixed;
				
				add_more(old_start, start-1, old_content,0); //vlevo, ty ukazatele si to vyresi
				add_more(end+1, old_end, old_content,0); //vpravo
				
		
			}
			//check();
		}
	}
	
	template<class T>
	void 
	interval<T>::add_one(const libcan_int where, const T& what, int add) {
		
		T new_mixed;
		if (add == 0){
			new_mixed = what;
		} else if (add == 1) {
			new_mixed = (_content + what);
		} else if (add == 2) {
			new_mixed = _content * what;
		} else if (add == 3) {
			new_mixed = _content;
		}
		

		
		
		if (_empty) {
			
			_empty= false;
			_start = where;
			_end = where;
			_content = what;
			_left = NULL;
			_right = NULL;
		} else {
		
			//taky zajimave - vlozi jen 1 pix
			if (where == _start && _end == _start) {
				//1 pixel na 1-ciselny interval
				
				_content = new_mixed;
			
			} else if (where == _start) {
				//takovy mozna hack, ale co
				++_start;
				add_one(where, new_mixed,0);
			} else if (where == _end) {
				--_end;
				add_one(where, new_mixed,0);
					//tady si nejsem jist, jestli je tohle dost efektivni
		
			} else if (where == _start - 1 && _content == new_mixed) {
					//jsem tesne vlevo! a jsem totez!
				--_start;
			
			
			} else if (where == _end + 1 && _content == new_mixed) {
					//jsem tesne vpravo! a totez!
				++_end;
		
			} else if (where > _start && where < _end) {
					//Strkam nekam doprostred
				libcan_int end = _end;
				_end = where - 1;
					//nejdriv se posunu pred to,
				add_one(where, new_mixed,0);
					//pak pridam tu jednu prostredni
				add_more(where+1, end, _content,0);
					//a nakonec pujde ta vpravo
		
			} else if (where < _start) {
				//nekde moc vlevo
				if (_left == NULL) {
					_left = new interval<T>(where, where, what);
				
				} else {
					_left->add_one(where, what,add);
				}
		
			} else if (where > _end) {
				//moc vpravo
				if (_right == NULL) {
					_right = new interval<T>(where, where, what);
					
				} else {
					_right->add_one(where, what,add);
				}
		
			}
			check();
		}		
	}

	//----------------------------GETTERS
	 
	template<class T>
	T 
	interval<T>::get_this() const {
		return _content;
	}
	
	template<class T>
	T 
	interval<T>::get(libcan::libcan_int where) const {
		//fakt trivialni rekurze
		
		if (_empty) {
			return T();
		} else {
			if (_start <= where && _end >= where) {
				return _content;
			}
			if (where < _start) {
				if (_left == NULL) {
					return T();
				} else {
					return _left->get(where);
				}
			}
			if (where > _end) {
				if (_right == NULL) {
					return T();
				} else {
					return _right->get(where);
				}
			}
			return T();
		}
	}
	
	template<class T> libcan_int
	interval<T>::most_left() const {

		if (_empty) {
			return 0;
		} else if (_left != NULL) {
			return _left->most_left(); 
		} else {
			return _start;
		}
	
	}
	
	template<class T> libcan_int
	interval<T>::get_start() const {
		return _start;
	}
	
	template<class T> const interval<T> 
	interval<T>::get_left() const {
		return *_left;
	}
	
	template<class T> bool 
	interval<T>::has_left() const {
		return (_left!=NULL);
	}
	
	template<class T> bool 
	interval<T>::is_empty() const {
		return _empty;
	}
	
	template<class T> const interval<T> 
	interval<T>::get_right() const {
		return *_right;
	}
	
	template<class T> bool 
	interval<T>::has_right() const {
		return (_right!=NULL);
	}
	
	
	template<class T> libcan_int
	interval<T>::get_end() const {
		return _end;
	}
	

	template<class T> libcan::libcan_int
	interval<T>::most_right() const {
		if (_empty) {
			return 0;
		} else if (_right != NULL) {
			return _right->most_right(); 
		} else {
			return _end;
		}

	}
	
	
		
	template<class T>   
	typename interval<T>::contents 
	interval<T>::get_all() const {
		
		if (_empty) {
			return contents();
		} else {
		
			//prida nejdriv sam sebe, pak zbytek
			contents res;
			res.push_back(interval_content<T>(_content, _start, _end));
	
			{
				contents k = recur_all();
				res.splice(res.end(), k);
					//splice mi spolehlive fungoval pouze takhle
			}
			
			return res;
		}
	}
	
	
	template<class T>
	typename interval<T>::contents 
	interval<T>::recur_all() const {
		//vraci v trochu zvlastnim poradi, ale ono je to jedno
		contents res;
		if (_left != NULL) {
			res.push_back(interval_content<T>(_left->_content, _left->_start, _left->_end));
		} 
		if (_right != NULL) {
			res.push_back(interval_content<T>(_right->_content, _right->_start, _right->_end));
		} 
			//Je to trochu zvlastni, ale ono je to skoro jedno.
	
		if (_left != NULL) {
			contents k = _left->recur_all();
			res.splice(res.end(), k);
				//open - splicem pripojuju na konec listu
		}
		if (_right != NULL) {
			contents k = _right->recur_all();
			res.splice(res.end(), k);
		}
		return res;
	}
	
	//-------------------OTHER
	
	template<class T>
	void 
	interval<T>::check() {
		
		//Otce nekontroluju (ja o nem vlastne i prd vim) - kdyztak, on zkontruluje on me
		//Proto kontroluji i vpravo i vlevo.
		if (_right != NULL) {
			if (_right->_start == _end+1 && _right->_content == _content) {
					//vpravo je totez
				interval<T>* p = _right;
				_end = _right->_end;
				_right = _right->_right;
				p->_right = NULL; //nechci, aby pri destruovani smazal svuj podstrom!!!!!!
				delete p;
			}
		}
		
		if (_left != NULL) {
			if (_left->_end == _start-1 && _left->_content == _content) {
					//vlevo je totez
				interval<T>* p = _left;
				_start = _left->_start;
				_left = _left->_left;
				p->_left = NULL; //nechci, aby pri destruovani smazal svuj podstrom!!!!!!
				delete p;
			
			}
		}

	}

}


#endif
