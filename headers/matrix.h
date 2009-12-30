#ifndef MATR_INC
#define MATR_INC

#include "types.h" //kvuli canlib_int

#define is_legal(_x,_y,_kolik) (((_x) < _width) && ((_y) < _height) && ((_x) >= 0) && ((_y) >= 0) && ((_kolik) < _element_width))
	//jestli je cislo v ramci mezi
#define position(_x,_y,_kolik) ((_y) * _width * _element_width + (_x) * _element_width + (_kolik))

namespace canlib {
	
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
		canlib_int _height;
		canlib_int _width;
		
		T* _array;
		
	public:
		matrix<T>& operator= (const matrix<T>& other);
		
		matrix(const matrix<T>& other);
		matrix(const canlib_int width, const canlib_int height, const small element_width);
			//vytvori matici s danou delkou, sirkou a sirkou policka
		
		matrix();
		
		~matrix();
			//odalokuje pole

        T* get_all();
		
		T** get_rows();	
			//vrati pole ukazatelu na zacatky radku (hodi se pro libpng)
		
		T get(const canlib_int x, const canlib_int y, const small element) const;
			//vrati konkretni prvek na pozici
			
		canlib_int get_width() const {return _width;}
		canlib_int get_height() const {return _height;}

		void set_more(const canlib_int start_x, const canlib_int end_x, const canlib_int y, const T* what);
			//nastavi vsechno od start_x do end_x na vysce y na to, co je na T
			//T je potreba chapat jako pole! Nastavovany prvek muze mit tloustku > 1.
		
		void set(const canlib_int x, const canlib_int y, const small element, const T& what);
		
		matrix<T> half();
	};
}


//-------------CONSTRUCTORS

template<class T>
canlib::matrix<T>::matrix() : _element_width(1), _height(1), _width(1), _array(new T[1]) { }

template <class T>
canlib::matrix<T>::matrix (const matrix<T>& other) : 
  _element_width(other._element_width),
  _height(other._height),
  _width(other._width),
  _array(new T[other._element_width*other._height*other._width]) {
	  
	const canlib_int max = _width*_height*_element_width;
	for (canlib_int i = 0; i < max; ++i) {
		_array[i] = other._array[i];
	}
}

template<class T>
canlib::matrix<T>::matrix(const canlib_int width, const canlib_int height, const small element_width) : 
  _element_width(__maximum(1, element_width)), 
  _height(__maximum(1, height)), 
  _width(__maximum(1, width)), 
  _array(new T[__maximum(1, width) * __maximum(1, height) * __maximum(1, element_width)])   {
	  
  //maxima jsou tam pro pripad, ze bych dostal jako vysku/sirku 0
		
	for (canlib_int i = 0; i <  width * height * element_width; ++i) {
		_array[i] = T();
	}
	  
	  //lepsi T() nez nejaka nahodna hodnota
}

//-----------------DESTRUCTOR
template<class T>
canlib::matrix<T>::~matrix() {
	delete [] _array;
}


//----------------GETTERS
template<class T> 
T* 
canlib::matrix<T>::get_all() {
	//possibly unsafe!!!
	return _array;
}

template<class T>
T canlib::matrix<T>::get(const canlib_int x, const canlib_int y, const small element) const {	
	if (is_legal(x,y, element)) {
		return _array[position(x,y,element)];
	} else {
		
		//pokud sahnu mimo, program nespadne, ale hodi defaultni T
		
		return T();
	}
}

template<class T>
T** canlib::matrix<T>::get_rows() {	
	T** res = new T* [_height];
	
		//nejdriv potrebuji pole s "vyskou" jako je vyska
	
	for (canlib_int i = 0; i < _height; ++i) {
		res[i] = &(_array[i * _width * _element_width]);
		
		//trivialnim zpusobem zjistim zacatky radku
		//POZOR - opet unsafe
	}
	return res;
}

//-------------SETTERS

template<class T>
void 
canlib::matrix<T>::set(const canlib_int x, const canlib_int y, const small element, const T& what) {
	
	if (is_legal(x,y,element)) {
		_array[position(x,y,element)] = what;
	}
}

template<class T>
canlib::matrix<T>&
canlib::matrix<T>::operator= (const matrix<T>& other) {
	//nemusi delat __maximum apod, protoze ten druhy uz JE v poradku
	_element_width = other._element_width;
	_height = other._height;
	_width = other._width;
	if (_array!=NULL) {
		delete [] _array;
	}
	const canlib_int max = _width*_height*_element_width;
	_array = new T[max];
	for (canlib_int i = 0; i < max; ++i) {
		_array[i] = other._array[i];
	}
	return *this;
}


template<class T>
void 
canlib::matrix<T>::set_more(const canlib_int start_x, const canlib_int end_x, const canlib_int y, const T* what) {

	for (canlib_int x = start_x; x <= end_x; ++x) { 
		if (is_legal(x,y,0)) {
			for (canlib_int elem = 0; elem < _element_width; ++elem) {
				_array[position(x,y,elem)] = what[elem];
				
				//pocitam s tim, ze mi nikdo nepodstrci prilis kratke pole, jinak tam strcim nejakou nahodnou hodnotu nebo pretecu
				//diky what[kolik]
				
			}
		}
	}
}

template<class T> 
canlib::matrix<T> canlib::matrix<T>::half() {


	matrix<T> res = matrix(_width/2, _height/2, _element_width);
	for (canlib_int x = 0; x < _width/2; ++x) {
		for (canlib_int y = 0; y < _width/2; ++y) {
			for (canlib_int i = 0; i<_element_width; ++i) {
				T a = _array[position(x*2, y*2, i)];
				T b = _array[position(x*2, y*2+1, i)];
				T c = _array[position(x*2+1, y*2, i)];
				T d = _array[position(x*2+1, y*2+1, i)];
				T prum = (a+b+c+d)/4;
				res.set(x,y,i,prum);
			}
		}
	}
	return res;
}

#endif
