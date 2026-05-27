//
// Created by HTY on 2026/5/25.
//

#ifndef TICKET_SYSTEM_2026_VECTOR_H
#define TICKET_SYSTEM_2026_VECTOR_H

#include "exception.h"
#include<iostream>
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
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
    void *buffer; // 只分配内存不构造
    T *data;
    int total_size = 10;
    int used_size = 0;
    class const_iterator;
    class iterator;
    // 同步修改迭代器
    void DoubleSize(iterator &it) {
        int idx = it - begin();
    	//std::cerr << idx << std::endl;
        void *new_buffer = operator new [](sizeof(T) * 2 * total_size);
        T *new_data = static_cast<T*>(new_buffer);
        for (int i = 0; i < used_size; i++) {
            new (new_data + i) T(data[i]);
        }
        total_size *= 2;
    	for (int i = 0; i < used_size; i++) data[i].~T();
        operator delete [](buffer);
        buffer = new_buffer;
        data = new_data;
        it = iterator(data + idx);
    }

    void DoubleSize() {
        void *new_buffer = operator new [] (sizeof(T) * 2 * total_size);
        T *new_data = static_cast<T*>(new_buffer);
        for (int i = 0; i < used_size; i++) {
            new (new_data + i) T(data[i]);
        }
        total_size *= 2;
    	for (int i = 0; i < used_size; i++) data[i].~T();
        operator delete [](buffer);
        buffer = new_buffer;
        data = new_data;
    }
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */

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
	    pointer p = nullptr;
    	T *storage_;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
	        iterator(){}
	        iterator(pointer it,T *storage = nullptr) : p(it), storage_(storage){}
		iterator operator+(const int &n) const
		{
			//TODO
		    iterator ans(p + n,storage_);
		    return ans;
		}
		iterator operator-(const int &n) const
		{
			//TODO
		    iterator ans(p - n,storage_);
		    return ans;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			//TODO
	            if (storage_ != rhs.storage_) throw invalid_iterator();
	            int cnt = 0;
	            iterator tmp = *this;
	            while (rhs != tmp) {
	                tmp--;
	                cnt++;
	            }
	            return cnt;
		}
		iterator& operator+=(const int &n)
		{
			//TODO
		    p += n;
		    return *this;
		}
		iterator& operator-=(const int &n)
		{
			//TODO
		    p -= n;
		    return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    iterator tmp = *this;
		    ++p;
		    return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
		    ++p;
		    return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
		    iterator tmp = *this;
		    --p;
		    return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
		    --p;
		    return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {
		    return *p;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
		    if (p == rhs.p) return true;
		    else return false;
		}
		bool operator==(const const_iterator &rhs) const {
		    if (p == rhs.p) return true;
		    else return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
		    if (p == rhs.p) return false;
		    else return true;
		}
		bool operator!=(const const_iterator &rhs) const {
		    if (p == rhs.p) return false;
		    else return true;
		}
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
	    /**
             * TODO add data members
             *   just add whatever you want.
             */
	    pointer p = nullptr;
	    T *storage_;
	public:
	    /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
	    const_iterator(){}
	    const_iterator(pointer it,T *storage) : p(it), storage_(storage){}
	    const_iterator operator+(const int &n) const
	    {
	        //TODO
	        const_iterator ans(p + n,storage_);
	        return ans;
	    }
	    const_iterator operator-(const int &n) const
	    {
	        //TODO
	        const_iterator ans(p - n,storage_);
	        return ans;
	    }
	    // return the distance between two iterators,
	    // if these two iterators point to different vectors, throw invaild_iterator.
	    int operator-(const const_iterator &rhs) const
	    {
	        //TODO
	        if (storage_ != rhs.storage_) throw invalid_iterator();
	        int cnt = 0;
	        const_iterator tmp = *this;
	        while (rhs != tmp) {
	            tmp--;
	            cnt++;
	        }
	        return cnt;
	    }
	    const_iterator& operator+=(const int &n)
	    {
	        //TODO
	        p += n;
	        return *this;
	    }
	    const_iterator& operator-=(const int &n)
	    {
	        //TODO
	        p -= n;
	        return *this;
	    }
	    /**
             * TODO iter++
             */
	    const_iterator operator++(int) {
	        const_iterator tmp = *this;
	        ++p;
	        return tmp;
	    }
	    /**
             * TODO ++iter
             */
	    const_iterator& operator++() {
	        ++p;
	        return *this;
	    }
	    /**
             * TODO iter--
             */
	    const_iterator operator--(int) {
	        const_iterator tmp = *this;
	        --p;
	        return tmp;
	    }
	    /**
             * TODO --iter
             */
	    const_iterator& operator--() {
	        --p;
	        return *this;
	    }
	    /**
             * TODO *it
             */
	    T& operator*() const {
	        return *p;
	    }
	    /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
	    bool operator==(const iterator &rhs) const {
	        if (p == rhs.p) return true;
	        else return false;
	    }
	    bool operator==(const const_iterator &rhs) const {
	        if (p == rhs.p) return true;
	        else return false;
	    }
	    /**
             * some other operator for iterator.
             */
	    bool operator!=(const iterator &rhs) const {
	        if (p == rhs.p) return false;
	        else return true;
	    }
	    bool operator!=(const const_iterator &rhs) const {
	        if (p == rhs.p) return false;
	        else return true;
	    }

	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
    /** Rmk:如果模板类没有构造函数 operator new/operator delete 只申请/释放空间，不绑定构造和析构函数，申请到的空间是由void*类型指针指向，buffer和data实际上指向同一块连续内存空间
     * 需要手动调用T类型的构造函数 placement new: new (address) T(value)
	 * 手动析构T: data[i].~T()
	 */
	vector() {
	    total_size = 10;
	    used_size = 0;
	    buffer = operator new [](sizeof(T) * total_size);
	    data = static_cast<T*>(buffer);
	}
	vector(const vector &other) {
	    buffer = operator new [](sizeof(T) * other.total_size);
	    total_size = other.total_size;
	    used_size = other.used_size;
	    data = static_cast<T*>(buffer);
	    for (int i = 0; i < used_size; i++) {
	        new (data + i) T(other.data[i]);
	    }
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for (int i = 0; i < used_size; i++) data[i].~T();
	    operator delete [](buffer);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
	    if (&other == this) return *this;
		for (int i = 0; i < used_size; i++) data[i].~T();
	    operator delete [](buffer);
	    buffer = operator new [](sizeof(T) * other.total_size);
	    total_size = other.total_size;
	    used_size = other.used_size;
	    data = static_cast<T*>(buffer);
	    for (int i = 0; i < used_size; i++) {
	        new (data + i) T(other.data[i]);
	    }
	    return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
	    if (pos < 0 || pos >= used_size) throw index_out_of_bound();
	    return data[pos];
	}
	const T & at(const size_t &pos) const {
	    if (pos < 0 || pos >= used_size) throw index_out_of_bound();
	    return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
	    if (pos < 0 || pos >= used_size) throw index_out_of_bound();
	    return data[pos];
	}
	const T & operator[](const size_t &pos) const {
	    if (pos < 0 || pos >= used_size) throw index_out_of_bound();
	    return data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
	    if (used_size == 0) throw container_is_empty();
	    return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
	    if (used_size == 0) throw container_is_empty();
	    return data[used_size - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
	    iterator beg(data,data);
	    return beg;
	}
	const_iterator begin() const {
	    const_iterator beg(data,data);
	    return beg;
	}
	const_iterator cbegin() const {
	    const_iterator beg(data,data);
	    return beg;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
	    iterator en(data + used_size,data);
	    return en;
	}
	const_iterator end() const {
	    const_iterator en(data + used_size,data);
	    return en;
	}
	const_iterator cend() const {
	    const_iterator en(data + used_size,data);
	    return en;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
	    if (used_size == 0) return true;
	    else return false;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
	    return used_size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (int i = 0; i < used_size; i++) data[i].~T();
	    used_size = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
	    if (used_size == total_size) DoubleSize(pos);
		new (data + used_size) T(data[used_size - 1]);
	    for (auto it = end() - 1; it != pos; it--) {
	    	//std::cerr << *it << std::endl;
	        *it = *(it - 1);
	    }
	    *pos = value;
	    used_size++;
	    return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
	    if (ind > used_size) throw index_out_of_bound();
	    if (used_size == total_size) DoubleSize();
		if (ind != used_size) new (data + used_size) T(*(end() - 1));
	    for (int i = used_size - 1; i > ind; i--) {
	        data[i] = data[i - 1];
	    }
	    new (data + ind) T(value);
	    iterator it(data + ind,data);
	    used_size++;
	    return it;
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
	    for (auto it = pos; it != end(); it++) {
	        *it = *(it + 1);
	    }
		data[used_size - 1].~T();
	    used_size--;
	    return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
	    if (ind >= used_size) throw index_out_of_bound();
	    for (int i = ind; i < used_size - 1; i++) {
	        data[i] = data[i + 1];
	    }
		data[used_size - 1].~T();
	    used_size--;
	    return iterator(data + ind,data);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
	    if (used_size == total_size) DoubleSize();
		new (data + used_size) T(value);
	    used_size++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
	    if (used_size == 0) throw container_is_empty();
		data[used_size - 1].~T();
	    used_size--;
	}
};


}


// 需要显式调用T的析构函数 因为buffer析构时不会自动析构T
#endif //TICKET_SYSTEM_2026_VECTOR_H