#ifndef INODE_INC
#define INODE_INC

#include "types.h"

namespace glib {
	
	template <class T>
	struct interval_content {
		T _cont;
		glib_int _start;
		glib_int _end;
		interval_content(const T& cont, glib_int start, glib_int end): _cont(cont), _start(start), _end(end){}
	};



	template<class T>
	class interval_node {
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
		 * ten BVS zatim NENI vyvazovany, a jelikoz tohle zabira v canvasu stejne minimum casu, asi nikdy nebude.
		 */
	public:
		typedef std::list<interval_content<T> > contents;
			//pouze pro vraceni vysledku (nebudu vracet celou strukturu)
	protected:
		
		glib_int _start;
			//Zacatek intervalu
		glib_int _end; 
			//Konec intervalu - vcetne!!! 
		
		T _content;
			//Obsah intervalu
		
		interval_node<T>* _left;
			//levy podstrom
		interval_node<T>* _right;
			//Pravy podstrom
		
		contents recur_all() const;
			//vrati obsahy vsech svych deti, *ale ne* sebe

	public:
		
		glib_int most_left() const;
		glib_int most_right() const;
		
		interval_node<T>* give_copy() const;
			//vytvori novy strom, kopii sam sebe, a vrati.
		
		interval_node (const glib_int start, const glib_int end, const T& content);
			//"klasicky" konstruktor
		interval_node (const interval_node<T>& other);
			//zkopiruje z dalsiho nodu
		
		T get(glib_int where) const;
			//najde danou pozici - pokud ne, da T()
		
		
		void move(glib_int sirka);
			//posune sebe i synky o sirka doprava
		
		~interval_node();
			//Smaze svoje deti
		
		void check();
			//zkontroluje, jestli nezabira zbytecne misto - tj, jestli vedle sebe nejsou dva stejne intervaly, co by sly spojit
			//To se muze stat dost dobre - napr. pridam hned po sobe 5,3,4 se stejnym obsahem
			//- kdyby necheckovalo, mel bych 3 zbytecne nody, takhle mam jeden. Myslim.
		
		contents get_all() const;
			//vrati obsahy vsech svych deti, *vcetne* sebe
		
		void add_more(const glib_int start, const glib_int end, const T& what);
			//prida novy interval - pokud se s nicim nekryje. Pokud ano, vse se upravi.
		
		void add_one(const glib_int where, const T& what);
			//Prida jeden "bod" - napr. mam interval 4-8, pridam 3, interval se zmeni na 3-8.
			//Tady je videt to mysleni "po pixelech" - ano, je to diskretni :)
		
		
		
		interval_node<T>& operator=(const interval_node<T>& other);
			//vrati kopii
	};
	

	//----------------CONSTRUCTORS
	template<class T>
	interval_node<T>::interval_node (const interval_node<T>& other) : 
	  _start(other._start), 
	  _end(other._end),
	  _content(other._content),
	  _left((other._left!=NULL)?(other._left->give_copy()):(NULL)),
	  _right((other._right!=NULL)?(other._right->give_copy()):(NULL)) { 
		  //Vic se mi libi, kdyz je vsechno v inicializaci. Jako napr. tady.

	}

	template<class T>
	interval_node<T>::interval_node (const glib_int start, const glib_int end, const T& content): 
	  _start(start), 
	  _end(__maximum(end,start)),
	  _content(content),
	  _left(NULL),
	  _right(NULL) { 
	}
	
	//---------------------DESTRUCTOR
	
	template<class T>
	interval_node<T>::~interval_node() {
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
	
	template<class T> interval_node<T>&
	interval_node<T>::operator=(const interval_node<T>& other) {
		_start = other._start;
		_end = other._end;
		_content = other._content;
	
		if (_left != NULL) {
			delete _left; 
			_left = NULL;
		}
		if (other._left != NULL) {
			//pokud je, zustane NULL z minula, vse ok
			_left = other._left->give_copy();
		}
	
		if (_right != NULL) {
			delete _right;
			_right = NULL;
		}
		if (other._right != NULL) {
			_right = other._right->give_copy();
		
		}

		return *this;
	}
	
	template<class T>
	void 
	interval_node<T>::move(glib_int sirka) {
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
	
	template<class T>
	void 
	interval_node<T>::add_more(const glib_int start, const glib_int end, const T& what) {
		//Tady je to trochu zajimave!
		// = -> stare, - -> nove

		if (start < _start) {
			glib_int smaller = __minimum (_start-1, end);
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
				_left = new interval_node<T> (start, smaller, what);
			} else {
				_left -> add_more(start, smaller, what);
					//pokud vlevo neco je, musime to poslat jemu
			
				_left -> check();
					//nechci, aby checku bylo moc, tak se spusti pouze kdyz si myslim, ze je potreba :)
			}
		}
		if (end > _end) {
			//totez v bledemodrem
			glib_int bigger = __maximum(_end+1, start);
			if (_right == NULL) {
				_right = new interval_node<T> (bigger, end, what);
			} else {
				_right -> add_more(bigger, end, what);
				_right -> check();
			}
		}
	
		//Uspesne jsem vyresil to, co se deje pred === a za nim, ted ale musim doresit pripadne prekryvy
		if (start <= _start && end >= _end) {
				//    =========
				//  --------------
				//ten zacatek a konec je doresen, zbyva jenom zmichat sam sebe s tim, co prislo
			_content = _content + what;
		
		} else if (start <= _start && end < _end && end >= _start) {
				//      =============
				//   -------
				//musim poresit cast vlevo, udelam to tak, ze sam sebe posunu doprava a
				//pridam "novou" levou cast - je to kvuli tomu, aby se mohla pripadne checknout a 
				//sloucit s necim, co uz je vlevo
			glib_int b = _start;
			_start = end + 1;
			add_more(b, end, _content + what);
		
		} else if (start > _start && end >= _end && start <= _end) {
			//       ==========
			//           ---------
			glib_int e = _end;
			_end = start - 1;
			add_more(start, e, _content + what);
		
		} else if (start > _start && end < _end) {
				// je tam ostra!
				// ===========
				//      ---
				//3 kousky
			glib_int e = _end;
			_end = start - 1;
			add_more(end + 1, e, _content); //jeden
			add_more(start, end, _content + what); //druhy	
		
		}

	}
	
	template<class T>
	void 
	interval_node<T>::add_one(const glib_int where, const T& what) {
		//taky zajimave - vlozi jen 1 pix
		if (where == _start && _end == _start) {
			//1 pixel na 1-ciselny interval
			_content = _content + what;
			
		} else if (where == _start) {
			//takovy mozna hack, ale co
			++_start;
			add_one(where, _content + what);
		} else if (where == _end) {
			--_end;
			add_one(where, _content + what);
				//tady si nejsem jist, jestli je tohle dost efektivni
		
		} else if (where == _start - 1 && _content == what) {
				//jsem tesne vlevo! a jsem totez!
			--_start;
			
			
		} else if (where == _end + 1 && _content == what) {
				//jsem tesne vpravo! a totez!
			++_end;
		
		} else if (where > _start && where < _end) {
				//Strkam nekam doprostred
			glib_int end = _end;
			_end = where - 1;
				//nejdriv se posunu pred to,
			add_one(where, _content + what);
				//pak pridam tu jednu prostredni
			add_more(where+1, end, _content);
				//a nakonec pujde ta vpravo
		
		} else if (where < _start) {
			//nekde moc vlevo
			if (_left == NULL) {
				_left = new interval_node<T>(where, where, what);
				
			} else {
				_left->add_one(where, what);
			}
		
		} else if (where > _end) {
			//moc vpravo
			if (_right == NULL) {
				_right = new interval_node<T>(where, where, what);
			} else {
				_right->add_one(where, what);
			}
		
		}
		check();
		
	}

	//----------------------------GETTERS
	
	template<class T>
	T 
	glib::interval_node<T>::get(glib::glib_int where) const {
		//fakt trivialni rekurze

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
	
	template<class T> glib::glib_int
	glib::interval_node<T>::most_left() const {

		if (_left != NULL) {
			return _left->most_left(); 
		} else {
			return _start;
		}
	
	}

	template<class T> glib::glib_int
	glib::interval_node<T>::most_right() const {
		if (_left != NULL) {
			return _left->most_right(); 
		} else {
			return _end;
		}

	}
	
	template<class T>
	glib::interval_node<T>* glib::interval_node<T>::give_copy() const {
	
		interval_node<T>* res = new interval_node<T>(_start, _end, _content);
				//rekurze - jednoducha, jasna.
	
		if (_left!=NULL) {
			res->_left = _left->give_copy();
		}
		if (_right!=NULL) {
			res->_right = _right->give_copy();
		}
		
		return res;
	}
	
		
	template<class T>   
	typename glib::interval_node<T>::contents 
	glib::interval_node<T>::get_all() const {
		//prida nejdriv sam sebe, pak zbytek
		contents res;
		res.push_back(interval_content<T>(_content, _start, _end));
	
		{
			contents k = recur_all();
			res.splice(res.end(), k);
				//splice mi spolehlive fungoval pouze takhle
				//ten extra blok je spis pro okrasu :)
		}
	
		return res;
	}
	
	
	template<class T>
	typename glib::interval_node<T>::contents 
	glib::interval_node<T>::recur_all() const {
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
	glib::interval_node<T>::check() {
		
		//Otce nekontroluju (ja o nem vlastne i prd vim) - kdyztak, on zkontruluje on me
		//Proto kontroluji i vpravo i vlevo.
		if (_right != NULL) {
			if (_right->_start == _end+1 && _right->_content == _content) {
					//vpravo je totez
				interval_node<T>* p = _right;
				_end = _right->_end;
				_right = _right->_right;
				p->_right = NULL; //nechci, aby pri destruovani smazal svuj podstrom!!!!!!
				delete p;
			}
		}
		
		if (_left != NULL) {
			if (_left->_end == _start-1 && _left->_content == _content) {
					//vlevo je totez
				interval_node<T>* p = _left;
				_start = _left->_start;
				_left = _left->_left;
				p->_left = NULL; //nechci, aby pri destruovani smazal svuj podstrom!!!!!!
				delete p;
			
			}
		}

	}

}


#endif
