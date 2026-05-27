#ifndef SJTU_VECTOR_H
#define SJTU_VECTOR_H

#include "exceptions.h"

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
    int sz, max_sz;
    T* a;
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
        friend class const_iterator;
        friend class vector;
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
        vector *head;
        int pos = 0;
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
        iterator (vector *a, int pos) : head(a), pos(pos) {}
        iterator (const const_iterator &x) {
            head = x.head;
            pos = x.pos;
        }
        iterator (const iterator &x) {
            head = x.head;
            pos = x.pos;
        }
		iterator operator+(const int &n) const
		{
			//TODO
            iterator ret(head, pos + n);
            return ret;
		}
		iterator operator-(const int &n) const
		{
			//TODO
            iterator ret(head, pos - n);
            return ret;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			//TODO
            if (head != rhs.head) throw invalid_iterator();
            return pos - rhs.pos;
		}
		iterator& operator+=(const int &n)
		{
			//TODO
            pos += n;
            return *this;
		}
		iterator& operator-=(const int &n)
		{
			//TODO
            pos -= n;
            return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
            iterator tmp(head, pos);
            pos++;
            return tmp;
        }
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
            pos++;
            return *this;
        }
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            iterator tmp(head, pos);
            pos--;
            return tmp;
        }
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
            pos--;
            return *this;
        }
		/**
		 * TODO *it
		 */
		T& operator*() const{
            return head->a[pos];
        }
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
            return head == rhs.head && pos == rhs.pos;
        }
		bool operator==(const const_iterator &rhs) const {
            return head == rhs.head && pos == rhs.pos;
        }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            return head != rhs.head || pos != rhs.pos;
        }
		bool operator!=(const const_iterator &rhs) const {
            return head != rhs.head || pos != rhs.pos;
        }
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
        friend class iterator;
        friend class vector;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
        const_iterator (const vector* head, int pos) : head(head), pos(pos) {}
        const_iterator (const const_iterator &x) {
            head = x.head;
            pos = x.pos;
        }
        const_iterator (const iterator &x) {
            head = x.head;
            pos = x.pos;
        }
		const_iterator operator+(const int &n) const
		{
			//TODO
            const_iterator ret(head, pos + n);
            return ret;
		}
		const_iterator operator-(const int &n) const
		{
			//TODO
            const_iterator ret(head, pos - n);
            return ret;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			//TODO
            if (head != rhs.head) throw invalid_iterator();
            return pos - rhs.pos;
		}
		const_iterator& operator+=(const int &n)
		{
			//TODO
            pos += n;
            return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			//TODO
            pos -= n;
            return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
            const_iterator tmp(head, pos);
            pos++;
            return tmp;
        }
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
            pos++;
            return *this;
        }
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
            const_iterator tmp(head, pos);
            pos--;
            return tmp;
        }
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
            pos--;
            return *this;
        }
		/**
		 * TODO *it
		 */
		const T& operator*() const{
            return head->a[pos];
        }
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
            return head == rhs.head && pos == rhs.pos;
        }
		bool operator==(const const_iterator &rhs) const {
            return head == rhs.head && pos == rhs.pos;
        }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            return head != rhs.head || pos != rhs.pos;
        }
		bool operator!=(const const_iterator &rhs) const {
            return head != rhs.head || pos != rhs.pos;
        }
	private:
		/*TODO*/
        const vector* head = nullptr;
        int pos = 0;
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {
        max_sz = 16; sz = 0;
        a = static_cast<T*>(operator new[](max_sz * sizeof(T)));
    }
	vector(const vector &other) {
        sz = other.sz;
        max_sz = other.max_sz;
        a = static_cast<T*>(operator new[](max_sz * sizeof(T)));
        for (int i = 0; i < sz; i++) new (a + i) T(other.a[i]);
    }
	/**
	 * TODO Destructor
	 */
	~vector() {
        for (int i = 0; i < sz; i++) a[i].~T();
        operator delete [](a);
    }
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
        if (this == &other) return *this;
        for (int i = 0; i < sz; i++) a[i].~T();
        operator delete [](a);
        sz = other.sz;
        max_sz = other.max_sz;
        a = static_cast<T*>(operator new[](max_sz * sizeof(T)));
        for (int i = 0; i < sz; i++) new (a + i) T(other.a[i]);
        return *this;
    }
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
        if (pos >= sz || pos < 0) throw index_out_of_bound();
        return a[pos];
    }
	const T & at(const size_t &pos) const {
        if (pos >= sz || pos < 0) throw index_out_of_bound();
        return a[pos];
    }
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
        if (pos >= sz || pos < 0) throw index_out_of_bound();
        return a[pos];
    }
	const T & operator[](const size_t &pos) const {
        if (pos >= sz || pos < 0) throw index_out_of_bound();
        return a[pos];
    }
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
        if (!sz) throw container_is_empty();
        return a[0];
    }
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
        if (!sz) throw container_is_empty();
        return a[sz - 1];
    }
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
        return iterator(this, 0);
    }
	const_iterator begin() const {
        return const_iterator(this, 0);
    }
	const_iterator cbegin() const {
        return const_iterator(this, 0);
    }
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
        return iterator(this, sz);
    }
	const_iterator end() const {
        return const_iterator(this, sz);
    }
	const_iterator cend() const {
        return const_iterator(this, sz);
    }
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
        return sz == 0;
    }
	/**
	 * returns the number of elements
	 */
	size_t size() const {
        return sz;
    }
	/**
	 * clears the contents
	 */
	void clear() {
        for (int i = 0; i < sz; ++i) a[i].~T();
        sz = 0;
    }
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
    void doublespace() {
        max_sz <<= 1;
        T *b = a;
        a = static_cast<T*>(operator new[](max_sz * sizeof(T)));
        for (int i = 0; i < sz; i++) {
            new (a + i) T(b[i]);
            b[i].~T();
        }
        operator delete [](b);
    }
	iterator insert(iterator pos, const T &value) {
        if (pos.pos > sz || pos.pos < 0) throw index_out_of_bound();
        int x = pos.pos;
        if (sz + 1 > max_sz) doublespace();
        for (int i = sz - 1; i >= x; i--) {
            new (a + i + 1) T(std::move(a[i]));
            a[i].~T();
        }
        new (a + x) T(value);
        sz++;
        return iterator(this, x);
    }
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
        int x = ind;
        if (x > sz || x < 0) throw index_out_of_bound();
        if (sz + 1 > max_sz) doublespace();
        for (int i = sz - 1; i >= x; i--) {
            new (a + i + 1) T(std::move(a[i]));
            a[i].~T();
        }
        new (a + x) T(value);
        sz++;
        return iterator(this, x);
    }
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
        int x = pos.pos;
        if (x >= sz) throw index_out_of_bound();
        for (int i = x; i < sz - 1; i++) {
            new (a + i) T(std::move(a[i + 1]));
            a[i + 1].~T();
        }
        sz--;
        return iterator(this, x);
    }
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
        if (ind >= sz) throw index_out_of_bound();
        int x = ind;
        for (int i = x; i < sz - 1; i++) {
            new (a + i) T(std::move(a[i + 1]));
            a[i + 1].~T();
        }
        sz--;
        return iterator(this, x);
    }
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
        if (sz + 1 > max_sz) doublespace();
        new(a + sz) T(value);
        sz++;
    }
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
        if (sz == 0) throw container_is_empty();
        a[sz - 1].~T();
        sz--;
    }
};


}

#endif