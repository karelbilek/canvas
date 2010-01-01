#ifndef MATR_INC
#define MATR_INC

#include "types.h" //kvuli libcan_int

#define __m_is_legal(_x,_y,_kolik) (((_x) < _width) && ((_y) < _height) && ((_x) >= 0) && ((_y) >= 0) && ((_kolik) < _element_width))
	//jestli je cislo v ramci mezi
#define __m_position(_x,_y,_kolik) ((_y) * _width * _element_width + (_x) * _element_width + (_kolik))

namespace libcan {
	
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
	private:
		
		
		small _element_width;
		libcan_int _height;
		libcan_int _width;
		
		T* _array;
		
	public:
		matrix<T>& operator= (const matrix<T>& other);
		
		matrix(const matrix<T>& other);
		matrix(const libcan_int width, const libcan_int height, const small element_width);
			//vytvori matici s danou delkou, sirkou a sirkou policka
		
		matrix();
		
		~matrix();
			//odalokuje pole

        T* get_all();
		
		T** get_rows();	
			//vrati pole ukazatelu na zacatky radku (hodi se pro libpng)
		
		T get(const libcan_int x, const libcan_int y, const small element) const;
			//vrati konkretni prvek na pozici
			
		libcan_int get_width() const {return _width;}
		libcan_int get_height() const {return _height;}

		void set_more(const libcan_int start_x, const libcan_int end_x, const libcan_int y, const T* what);
			//nastavi vsechno od start_x do end_x na vysce y na to, co je na T
			//T je potreba chapat jako pole! Nastavovany prvek muze mit tloustku > 1.
		
		void set(const libcan_int x, const libcan_int y, const small element, const T& what);
		
		matrix<T> half();
	};
}


//-------------CONSTRUCTORS

template<class T>
libcan::matrix<T>::matrix() : _element_width(1), _height(1), _width(1), _array(new T[1]) { }

template <class T>
libcan::matrix<T>::matrix (const matrix<T>& other) : 
  _element_width(other._element_width),
  _height(other._height),
  _width(other._width),
  _array(new T[other._element_width*other._height*other._width]) {
	  
	const libcan_int max = _width*_height*_element_width;
	for (libcan_int i = 0; i < max; ++i) {
		_array[i] = other._array[i];
	}
}

template<class T>
libcan::matrix<T>::matrix(const libcan_int width, const libcan_int height, const small element_width) : 
  _element_width(__maximum(1, element_width)), 
  _height(__maximum(1, height)), 
  _width(__maximum(1, width)), 
  _array(new T[__maximum(1, width) * __maximum(1, height) * __maximum(1, element_width)])   {
	  
  //maxima jsou tam pro pripad, ze bych dostal jako vysku/sirku 0
		
	for (libcan_int i = 0; i <  width * height * element_width; ++i) {
		_array[i] = T();
	}
	  
	  //lepsi T() nez nejaka nahodna hodnota
}

//-----------------DESTRUCTOR
template<class T>
libcan::matrix<T>::~matrix() {
	delete [] _array;
}


//----------------GETTERS
template<class T> 
T* 
libcan::matrix<T>::get_all() {
	//possibly unsafe!!!
	return _array;
}

template<class T>
T libcan::matrix<T>::get(const libcan_int x, const libcan_int y, const small element) const {	
	if (__m_is_legal(x,y, element)) {
		return _array[__m_position(x,y,element)];
	} else {
		
		//pokud sahnu mimo, program nespadne, ale hodi defaultni T
		
		return T();
	}
}

template<class T>
T** libcan::matrix<T>::get_rows() {	
	T** res = new T* [_height];
	
		//nejdriv potrebuji pole s "vyskou" jako je vyska
	
	for (libcan_int i = 0; i < _height; ++i) {
		res[i] = &(_array[i * _width * _element_width]);
		
		//trivialnim zpusobem zjistim zacatky radku
		//POZOR - opet unsafe
	}
	return res;
}

//-------------SETTERS

template<class T>
void 
libcan::matrix<T>::set(const libcan_int x, const libcan_int y, const small element, const T& what) {
	
	if (__m_is_legal(x,y,element)) {
		_array[__m_position(x,y,element)] = what;
	}
}

template<class T>
libcan::matrix<T>&
libcan::matrix<T>::operator= (const matrix<T>& other) {
	//nemusi delat __maximum apod, protoze ten druhy uz JE v poradku
	_element_width = other._element_width;
	_height = other._height;
	_width = other._width;
	if (_array!=NULL) {
		delete [] _array;
	}
	const libcan_int max = _width*_height*_element_width;
	_array = new T[max];
	for (libcan_int i = 0; i < max; ++i) {
		_array[i] = other._array[i];
	}
	return *this;
}


template<class T>
void 
libcan::matrix<T>::set_more(const libcan_int start_x, const libcan_int end_x, const libcan_int y, const T* what) {

	for (libcan_int x = start_x; x <= end_x; ++x) { 
		if (__m_is_legal(x,y,0)) {
			for (libcan_int elem = 0; elem < _element_width; ++elem) {
				_array[__m_position(x,y,elem)] = what[elem];
				
				//pocitam s tim, ze mi nikdo nepodstrci prilis kratke pole, jinak tam strcim nejakou nahodnou hodnotu nebo pretecu
				//diky what[kolik]
				
			}
		}
	}
}

template<class T> 
libcan::matrix<T> libcan::matrix<T>::half() {


	matrix<T> res = matrix(_width/2, _height/2, _element_width);
	for (libcan_int x = 0; x < _width/2; ++x) {
		for (libcan_int y = 0; y < _width/2; ++y) {
			for (libcan_int i = 0; i<_element_width; ++i) {
				T a = _array[__m_position(x*2, y*2, i)];
				T b = _array[__m_position(x*2, y*2+1, i)];
				T c = _array[__m_position(x*2+1, y*2, i)];
				T d = _array[__m_position(x*2+1, y*2+1, i)];
				T prum = (a+b+c+d)/4;
				res.set(x,y,i,prum);
			}
		}
	}
	return res;
}

#endif
