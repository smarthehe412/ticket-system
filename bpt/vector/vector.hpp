#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"
#include "utility.hpp"
#include <cstdlib>

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
	int now_size,reserved_size;
	T* A;
	void double_space()
	{
		T* tA=(T*)malloc(sizeof(T)*(reserved_size*2));
		memcpy(tA,A,sizeof(T)*reserved_size);
		free(A);
		reserved_size<<=1;
		A=tA;
	}
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
		T* start;
		int pos;
		const vector<T> *pVec;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator(const vector<T> *pVec,int pos): pVec(pVec),start(pVec->A),pos(pos) {}
		int getpos() const
		{
			return pos;
		}
		iterator operator+(const int &n) const 
		{
			//TODO
			return iterator(pVec,pos+n);
		}
		iterator operator-(const int &n) const 
		{
			//TODO
			return iterator(pVec,pos-n);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			//TODO
			if(start!=rhs.start) throw invalid_iterator();
			return pos-rhs.pos;
		}
		int operator-(const const_iterator &rhs) const 
		{
			//TODO
			if(start!=rhs.start) throw invalid_iterator();
			return pos-rhs.pos;
		}
		iterator& operator+=(const int &n) 
		{
			//TODO
			pos+=n;
			return *this;
		}
		iterator& operator-=(const int &n) 
		{
			//TODO
			pos-=n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			return iterator(pVec,pos++);
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++()
		{
			pos++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			return iterator(pVec,pos--);
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--()
		{
			pos--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			if(start!=pVec->A) throw runtime_error();
			if(pos<0||pos>=pVec->now_size) throw runtime_error();
			return start[pos]; 
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {return start==rhs.start&&pos==rhs.pos;}
		bool operator==(const const_iterator &rhs) const {return start==rhs.start&&pos==rhs.pos;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return start!=rhs.start||pos!=rhs.pos;}
		bool operator!=(const const_iterator &rhs) const {return start!=rhs.start||pos!=rhs.pos;}
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
		T* start;
		int pos;
		const vector<T> *pVec;
	
	public:
		const_iterator(const vector<T> *pVec,int pos): pVec(pVec),start(pVec->A),pos(pos) {}
		const_iterator operator+(const int &n) const 
		{
			//TODO
			return const_iterator(pVec,pos+n);
		}
		const_iterator operator-(const int &n) const 
		{
			//TODO
			return const_iterator(pVec,pos-n);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			//TODO
			if(start!=rhs.start) throw invalid_iterator();
			return pos-rhs.pos;
		}
		int operator-(const const_iterator &rhs) const 
		{
			//TODO
			if(start!=rhs.start) throw invalid_iterator();
			return pos-rhs.pos;
		}
		const_iterator& operator+=(const int &n) 
		{
			//TODO
			pos+=n;
			return *this;
		}
		const_iterator& operator-=(const int &n) 
		{
			//TODO
			pos-=n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int)
		{
			return const_iterator(pVec,pos++);
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++()
		{
			pos++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
			return const_iterator(pVec,pos--);
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--()
		{
			pos--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const
		{
			if(start!=pVec->A) throw runtime_error();
			if(pos<0||pos>=pVec->now_size) throw runtime_error();
			return start[pos]; 
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {return start==rhs.start&&pos==rhs.pos;}
		bool operator==(const const_iterator &rhs) const {return start==rhs.start&&pos==rhs.pos;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return start!=rhs.start||pos!=rhs.pos;}
		bool operator!=(const const_iterator &rhs) const {return start!=rhs.start||pos!=rhs.pos;}
		/*TODO*/

	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector()
	{
		now_size=0;
		reserved_size=1;
		A=(T*)malloc(sizeof(T));
	}
	vector(const vector &other)
	{
		now_size=other.now_size;
		reserved_size=other.reserved_size;
		A=(T*)malloc(sizeof(T)*reserved_size);
		memcpy(A,other.A,sizeof(T)*reserved_size);
	}
	/**
	 * TODO Destructor
	 */
	~vector()
	{
		for(int i=0;i<now_size;i++) (A+i)->~T();
		free(A);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other)
	{
		if(this==&other) return *this;
		for(int i=0;i<reserved_size;i++) (A+i)->~T();
		free(A);
		now_size=other.now_size;
		reserved_size=other.reserved_size;
		A=(T*)malloc(sizeof(T)*reserved_size);
		memcpy(A,other.A,sizeof(T)*reserved_size);
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos)
	{
		if(pos<0||pos>=now_size) throw index_out_of_bound();
		return A[pos];
	}
	const T & at(const size_t &pos) const
	{
		if(pos<0||pos>=now_size) throw index_out_of_bound();
		return A[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos)
	{
		if(pos<0||pos>=now_size) throw index_out_of_bound();
		return A[pos];
	}
	const T & operator[](const size_t &pos) const
	{
		if(pos<0||pos>=now_size) throw index_out_of_bound();
		return A[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const
	{
		if(now_size==0) throw container_is_empty();
		return A[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const
	{
		if(now_size==0) throw container_is_empty();
		return A[now_size-1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin()
	{
		return iterator(this,0);
	}
	const_iterator cbegin() const
	{
		return const_iterator(this,0);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end()
	{
		return iterator(this,now_size);
	}
	const_iterator cend() const
	{
		return const_iterator(this,now_size);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const
	{
		if(now_size==0) return true;
		return false;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const
	{
		return now_size;
	}
	/**
	 * clears the contents
	 */
	void clear()
	{
		for(int i=0;i<reserved_size;i++) (A+i)->~T();
		free(A);
		now_size=0;
		reserved_size=1;
		A=(T*)malloc(sizeof(T));
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value)
	{
		int tp=pos.getpos();
		if(tp<0||tp>now_size) throw runtime_error();
		if(now_size+1==reserved_size) double_space();
		memmove(A+tp+1,A+tp,sizeof(T)*(now_size-tp));
		new (A+tp) T (value);
		now_size++;
		return iterator(this,tp);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value)
	{
		int tp=ind;
		if(tp<0||tp>now_size) throw index_out_of_bound();
		if(now_size+1==reserved_size) double_space();
		memmove(A+tp+1,A+tp,sizeof(T)*(now_size-tp));
		new (A+tp) T (value);
		now_size++;
		return iterator(this,tp);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos)
	{
		int tp=pos.getpos();
		if(tp<0||tp>=now_size) throw runtime_error();
		(A+tp)->~T();
		memmove(A+tp,A+tp+1,sizeof(T)*(now_size-tp-1));
		(A+now_size-1)->~T();
		now_size--;
		return iterator(this,tp);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind)
	{
		int tp=ind;
		if(tp<0||tp>=now_size) throw index_out_of_bound();
		(A+tp)->~T();
		memmove(A+tp,A+tp+1,sizeof(T)*(now_size-tp-1));
		(A+now_size-1)->~T();
		now_size--;
		return iterator(this,tp);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value)
	{
		if(now_size+1==reserved_size) double_space();
		new (A+now_size) T (value);
		now_size++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back()
	{
		if(now_size==0) throw container_is_empty();
		(A+now_size-1)->~T();
		now_size--;
	}
};


}

#endif