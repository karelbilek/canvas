#ifndef MATR_INC
#define MATR_INC

#include "types.h" //kvuli glib_int

#define is_legal(_x,_y,_kolik) (((_x) < _width) && ((_y) < _height) && ((_x) >= 0) && ((_y) >= 0) && ((_kolik)>=0) && ((_kolik) < _element_width))
	//jestli je cislo v ramci mezi
#define position(_x,_y,_kolik) ((_y) * _width * _element_width + (_x) * _element_width + (_kolik))

namespace glib {
	
	template<class T>
	class matrix {
		/*
		 * Trida na rychle uchovavani matice jakychkoliv hodnot, uzpusobena jednak pro psani na png,
		 * a jednak tomu, jak data tvori canvas.
		 * 
		 * Data muzou byt nekolik mist "tlusta" - tj. v jednom miste muze byt ulozeno vic hodnot
		 * (tj. ta matice je jakoby tolikrat siroka)
		 * Je to k tomu, jak je reprezentovana RGBa a hlavne, jak ji chce png_lib.
		 *
		 * Matice je vnitrne ulozena jako pole. 
		 * Jde pristoupit k libovolnemu prvku pres get(), nebo "natvrdo" primo k radkam pres get_rows.
		 * Pokud ale dame get_rows, dostaneme se primo na pole radku, kde je vlastne mozno menit data primo!
		 * Cimz vlastne muzu obejit to private:, ktere je tim vlastne pro _array jenom tak pro okrasu,
		 * ale ja toho v tom poli tolik zkazit nemuzu.
		 */
	public:
		typedef unsigned char small;
	private:
		
		
		small _element_width;
		glib_int _height;
		glib_int _width;
		
		T* _array;
		
	public:
		matrix<T>& operator= (const matrix<T>& other);
		
		matrix(const matrix<T>& other);
		matrix(const glib_int width, const glib_int height, const small element_width);
			//vytvori matici s danou delkou, sirkou a sirkou policka
		
		matrix();
		
		~matrix();
			//odalokuje pole

        T* get_all();
		
		T** get_rows();	
			//vrati pole ukazatelu na zacatky radku (hodi se pro libpng)
		
		T get(const glib_int x, const glib_int y, const small element) const;
			//vrati konkretni prvek na pozici

		void set_more(const glib_int start_x, const glib_int end_x, const glib_int y, const T* what);
			//nastavi vsechno od start_x do end_x na vysce y na to, co je na T
			//T je potreba chapat jako pole! Nastavovany prvek muze mit tloustku > 1.
		
		void set(const glib_int x, const glib_int y, const small element, const T& what);
		
		matrix<T> half();
	};
}


//-------------CONSTRUCTORS

template<class T>
glib::matrix<T>::matrix() : _element_width(1), _height(1), _width(1), _array(new T[1]) { }

template <class T>
glib::matrix<T>::matrix (const matrix<T>& other) : 
  _element_width(other._element_width),
  _height(other._height),
  _width(other._width),
  _array(new T[other._element_width*other._height*other._width]) {
	  
	const glib_int max = _width*_height*_element_width;
	for (glib_int i = 0; i < max; ++i) {
		_array[i] = other._array[i];
	}
}

template<class T>
glib::matrix<T>::matrix(const glib_int width, const glib_int height, const small element_width) : 
  _element_width(__maximum(1, element_width)), 
  _height(__maximum(1, height)), 
  _width(__maximum(1, width)), 
  _array(new T[__maximum(1, width) * __maximum(1, height) * __maximum(1, element_width)])   {
	  
  //maxima jsou tam pro pripad, ze bych dostal jako vysku/sirku 0
		
	for (glib_int i = 0; i <  width * height * element_width; ++i) {
		_array[i] = T();
	}
	  
	  //lepsi T() nez nejaka nahodna hodnota
}

//-----------------DESTRUCTOR
template<class T>
glib::matrix<T>::~matrix() {
	delete [] _array;
}


//----------------GETTERS
template<class T> 
T* 
glib::matrix<T>::get_all() {
	//possibly unsafe!!!
	return _array;
}

template<class T>
T glib::matrix<T>::get(const glib_int x, const glib_int y, const small element) const {	
	if (is_legal(x,y, element)) {
		return _array[position(x,y,element)];
	} else {
		
		//pokud sahnu mimo, program nespadne, ale hodi defaultni T
		
		return T();
	}
}

template<class T>
T** glib::matrix<T>::get_rows() {	
	T** res = new T* [_height];
	
		//nejdriv potrebuji pole s "vyskou" jako je vyska
	
	for (glib_int i = 0; i < _height; ++i) {
		res[i] = &(_array[i * _width * _element_width]);
		
		//trivialnim zpusobem zjistim zacatky radku
		//POZOR - opet unsafe
	}
	return res;
}

//-------------SETTERS

template<class T>
void 
glib::matrix<T>::set(const glib_int x, const glib_int y, const small element, const T& what) {
	if (is_legal(x,y,element)) {
		_array[position(x,y,element)] = what;
	}
}

template<class T>
glib::matrix<T>&
glib::matrix<T>::operator= (const matrix<T>& other) {
	//nemusi delat __maximum apod, protoze ten druhy uz JE v poradku
	_element_width = other._element_width;
	_height = other._height;
	_width = other._width;
	if (_array!=NULL) {
		delete [] _array;
	}
	const glib_int max = _width*_height*_element_width;
	_array = new T[max];
	for (glib_int i = 0; i < max; ++i) {
		_array[i] = other._array[i];
	}
	return *this;
}


template<class T>
void 
glib::matrix<T>::set_more(const glib_int start_x, const glib_int end_x, const glib_int y, const T* what) {

	for (glib_int x = start_x; x <= end_x; ++x) { 
		if (is_legal(x,y,0)) {
			for (glib_int elem = 0; elem < _element_width; ++elem) {
				_array[position(x,y,elem)] = what[elem];
				
				//pocitam s tim, ze mi nikdo nepodstrci prilis kratke pole, jinak tam strcim nejakou nahodnou hodnotu nebo pretecu
				//diky what[kolik]
				
			}
		}
	}
}

template<class T> 
glib::matrix<T> glib::matrix<T>::half() {


	matrix<T> res = matrix(_width/2, _height/2, _element_width);
	for (glib_int x = 0; x < _width/2; ++x) {
		for (glib_int y = 0; y < _width/2; ++y) {
			for (glib_int i = 0; i<_element_width; ++i) {
				T a = _array[position(x*2, y*2, i)];
				T b = _array[position(x*2, y*2+1, i)];
				T c = _array[position(x*2+1, y*2, i)];
				T d = _array[position(x*2+1, y*2+1, i)];
				T prum = (a+b+c+d)/4;
				res.set(x,y,i,prum);
					//here I cannot use position macro, because it has DIFFERENT size!
			}
		}
	}
	return res;
}

#endif
