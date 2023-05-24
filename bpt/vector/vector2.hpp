#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"
#include "utility.hpp"

#include <climits>
#include <cstddef>

namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector 
{
private:
	T *arr;
	size_t siz,mx_siz;
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		pointer pt;
		vector* vec;
		
	public:
		iterator(T* p = nullptr, vector* a = nullptr):pt(p),vec(a){}
		iterator(const iterator& ot){
			pt = ot.pt;
			vec = ot.vec;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const 
		{
			if(pt + n > (vec -> arr) + (vec -> siz)) throw invalid_iterator();
			return iterator(pt + n, vec);
			//TODO
		}
		iterator operator-(const int &n) const 
		{
			if(pt - n < (vec -> arr)) throw invalid_iterator();
			return iterator(pt - n, vec);
			//TODO
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			if(vec != rhs.vec) throw invalid_iterator();
			return pt - rhs.pt;
			//TODO
		}
		iterator& operator+=(const int &n) 
		{
			if(pt + n > (vec -> arr) + (vec -> siz)) throw invalid_iterator();
			pt += n;
			return pt;
			//TODO
		}
		iterator& operator-=(const int &n) 
		{
			if(pt - n < (vec -> arr)) throw invalid_iterator();
			pt -= n;
			return pt;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator it(*this);
			++pt;
			if(pt > (vec -> arr) + (vec -> siz)) throw invalid_iterator();
			return *this;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++pt;
			if(pt > (vec -> arr) + (vec -> siz)) throw invalid_iterator();
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator it(*this);
			--pt;
			if(pt < (vec -> arr)) throw invalid_iterator();
			return *this;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--pt;
			if(pt < (vec -> arr)) throw invalid_iterator();
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			if(pt < vec -> arr || pt >= (vec -> arr) + (vec -> siz))
				throw invalid_iterator();
			return *pt;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {return pt == rhs.pt;}
		bool operator==(const const_iterator &rhs) const {return pt == rhs.pt;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return pt != rhs.pt;}
		bool operator!=(const const_iterator &rhs) const {return pt != rhs.pt;}
        bool operator<(const iterator &rhs) const {return pt < rhs.pt;}
		bool operator<(const const_iterator &rhs) const {return pt < rhs.pt;}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		/*TODO*/
		const T* pt;
		const vector* vec;
	public:
		const_iterator(const T* p = nullptr, const vector* a = nullptr):pt(p),vec(a){}
		const_iterator(const const_iterator& ot){
			pt = ot.pt;
			vec = ot.vec;
		}
		const_iterator operator+(const int &n) const 
		{
			return const_iterator(pt + n, vec);
			//TODO
		}
		const_iterator operator-(const int &n) const 
		{
			return const_iterator(pt - n, vec);
			//TODO
		}
		
		int operator-(const const_iterator &rhs) const 
		{
			if(vec != rhs.vec) throw invalid_iterator();
			return pt - rhs.pt;
			//TODO
		}
		const_iterator& operator+=(const int &n) 
		{
			pt += n;
			return pt;
			//TODO
		}
		const_iterator& operator-=(const int &n) 
		{
			pt -= n;
			return pt;
			//TODO
		}
		
		const_iterator operator++(int) {
			const_iterator it(*this);
			++pt;
			return *this;
		}
		
		const_iterator& operator++() {
			++pt;
			return *this;
		}
		
		const_iterator operator--(int) {
			const_iterator it(*this);
			--pt;
			return *this;
		}

		const_iterator& operator--() {
			--pt;
			return *this;
		}

		const T& operator*() const{
			if(pt < vec -> arr || pt >= (vec -> arr) + (vec -> siz))
				throw invalid_iterator();
			return *pt;
		}

		bool operator==(const iterator &rhs) const {return pt == rhs.pt;}
		bool operator==(const const_iterator &rhs) const {return pt == rhs.pt;}

		bool operator!=(const iterator &rhs) const {return pt != rhs.pt;}
		bool operator!=(const const_iterator &rhs) const {return pt != rhs.pt;}

        bool operator<(const iterator &rhs) const {return pt < rhs.pt;}
		bool operator<(const const_iterator &rhs) const {return pt < rhs.pt;}
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {
		siz = 0; 
		arr = static_cast<T*>(operator new(sizeof(T)*2));
		mx_siz = 2;
	}
	vector(const int& sz){
		siz = mx_siz = sz;
		arr = static_cast<T*>(operator new(sizeof(T)* mx_siz));
	}
	vector(const int& sz, const T& val){
		siz = mx_siz = sz;
		arr = static_cast<T*>(operator new(sizeof(T)* mx_siz));
		for(int i = 0; i < sz; ++i)
			arr[i] = val;
	}
	vector(const vector &other) {
		siz = mx_siz = other.siz;
		arr = static_cast<T*>(operator new(sizeof(T) * mx_siz));
		for(int i = 0; i < siz; ++i)
			arr[i] = other.arr[i];
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for(int i = 0; i < siz; ++i) (arr + i) ->~T();
		operator delete(arr);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if(this == &other) return *this;
		for(int i = 0; i < siz; ++i) (arr + i) ->~T();
		operator delete(arr);
		siz = mx_siz = other.siz;
		arr = static_cast<T*>(operator new(sizeof(T) * mx_siz));
		for(int i = 0; i < siz; ++i)
			arr[i] = other.arr[i];
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if(pos < 0 || pos >= siz) throw index_out_of_bound();
		return arr[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos < 0 || pos >= siz) throw index_out_of_bound();
		return arr[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if(pos < 0 || pos >= siz) throw index_out_of_bound();
		return arr[pos];
	}
	const T & operator[](const size_t &pos) const {
		if(pos < 0 || pos >= siz) throw index_out_of_bound();
		return arr[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if(siz == 0) throw container_is_empty();
		return arr[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if(siz == 0) throw container_is_empty();
		return arr[siz - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {return iterator(arr, this);}
	const_iterator cbegin() const {return const_iterator(arr, this);}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {return iterator(arr + siz, this);}
	const_iterator cend() const {return const_iterator(arr + siz, this);}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {return siz == 0;}
	/**
	 * returns the number of elements
	 */
	size_t size() const {return siz;}
	/**
	 * clears the contents
	 */
	void clear() {
		for(int i = 0; i < siz; ++i) (arr + i)->~T();
		operator delete(arr);
		siz = 0; 
		arr = static_cast<T*>(operator new(sizeof(T)*2));
		mx_siz = 2;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		int cur = pos - begin();
		if(siz + 1 == mx_siz){
			mx_siz *= 2;
			T* now = arr;
			arr = static_cast<T*>(operator new(sizeof(T) * mx_siz));
			memcpy(arr, now, sizeof(T) * siz);
			operator delete(now);
		}
		memmove(arr + cur + 1, arr + cur, sizeof(T) * (siz - cur));
		++ siz;
		new(arr + cur) T(value);
		return iterator(arr + cur, this);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if(ind > siz) throw index_out_of_bound();
		if(siz + 1 == mx_siz){
			mx_siz *= 2;
			T* now = arr;
			arr = static_cast<T*>(operator new(sizeof(T) * mx_siz));
			memcpy(arr, now, sizeof(T) * siz);
			operator delete(now);
		}
		memmove(arr + ind + 1, arr + ind, sizeof(T) * (siz - ind));
		++siz;
		new(arr + ind) T(value);
		return iterator(arr + ind, this);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		int cur = pos - begin();
		(arr + cur) -> ~T();
		//for(int i = cur; i < siz - 1; ++i) arr[i] = arr[i + 1];
		memmove(arr + cur , arr + cur + 1, sizeof(T) * (siz - cur - 1));
		--siz;
		if(siz <= mx_siz/2 && mx_siz > 2){
			mx_siz /= 2;
			T *now = arr;
			arr = static_cast<T*>(operator new (sizeof(T) * mx_siz));
			memcpy(arr, now, sizeof(T) * siz);
			operator delete(now);
		}
		return iterator(arr + cur, this);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if(ind >= siz) throw index_out_of_bound();
		(arr + ind) -> ~T();
		//for(int i = ind; i < siz - 1; ++i) arr[i] = arr[i + 1];
		memmove(arr + ind , arr + ind + 1, sizeof(T) * (siz - ind - 1));
		if(siz <= mx_siz/2 && mx_siz > 2){
			mx_siz /= 2;
			T *now = arr;
			arr = static_cast<T*>(operator new (sizeof(T) * mx_siz));
			memcpy(arr, now, sizeof(T) * siz);
			operator delete(now);
		}
		return iterator(arr + ind, this);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if(siz + 1 == mx_siz){
			mx_siz *= 2;
			T* now = arr;
			arr = static_cast<T*>(operator new(sizeof(T) * mx_siz));
			memcpy(arr, now, sizeof(T) * siz);
			operator delete(now);
		}
		new(arr + (siz ++)) T(value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if(siz + 1 == 0) throw container_is_empty();
		--siz; (arr + siz) -> ~T();
		if(siz <= mx_siz/2 && mx_siz > 2){
			mx_siz /= 2;
			T *now = arr;
			arr = static_cast<T*>(operator new (sizeof(T) * mx_siz));
			memcpy(arr, now, sizeof(T) * siz);
			operator delete(now);
		}
	}
};


}

#endif
