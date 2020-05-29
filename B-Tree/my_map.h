#pragma once
#include <algorithm>
#include <iostream>
#include <utility>
#include <type_traits>
#include <iterator>
#include <memory>
#include "my_btree.h"

template<class Key, class T, class Compare = std::less<Key>> 
class my_map
{
	template<bool is_const, bool reversed = false>
	class iterator_impl;
	
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<const Key, T>;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using key_compare = Compare;
	using reference = value_type&;
	using const_reference = const value_type&;

	using iterator = iterator_impl<false>;
	using const_iterator = iterator_impl<true>;
	using reverse_iterator = iterator_impl<false, true>;
	using const_reverse_iterator = iterator_impl<true, false>;
	//using node_type = 
	//using insert_return_type = 

	class value_compare;

	my_map();
	my_map(my_map& s);
	my_map(my_map&& s);
	~my_map();
	
	my_map& operator=(const my_map& other);
	my_map& operator=(my_map&& other) noexcept;
	my_map& operator=(std::initializer_list<value_type> ilist);

	allocator_type get_allocator() const noexcept;
	
	T& at(const Key& key);
	const T& at(const Key& key) const;
	T& operator[](const Key& key);
	T& operator[](Key&& key);

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;

	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crend() const noexcept;

	[[nodiscard]] bool empty() const noexcept;
	size_type size() const noexcept;
	size_type max_size() const noexcept;

	void clear() noexcept;
	void insert();//bude slozitejsi...
	template<class... Args>
	std::pair<iterator, bool> emplace(Args&&... args);
	template <class... Args>
	std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
	iterator erase(iterator pos);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	size_type erase(const key_type& key);

	void swap(my_map& other) noexcept;

	node_type extract(const_iterator position);
	node_type extract(const key_type& x);

	template<class C2>
	void merge(my_map<Key, T, C2, Allocator>& source);

	size_type count(const Key& key) const;

	template< class K >
	size_type count(const K& x) const;

	iterator find(const Key& key);
	const_iterator find(const Key& key) const;

	template< class K > 
	iterator find(const K& x);
	template< class K > 
	const_iterator find(const K& x) const;
	
	bool contains(const Key& key) const;
	template<class K> 
	bool contains(const K& x) const;

	std::pair<iterator, iterator> equal_range(const Key& key);
	std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

	template< class K >
	std::pair<iterator, iterator> equal_range(const K& x);

	template< class K >
	std::pair<const_iterator, const_iterator> equal_range(const K& x) const;

	iterator lower_bound(const Key& key);
	const_iterator lower_bound(const Key& key) const;

	iterator upper_bound(const Key& key);
	const_iterator upper_bound(const Key& key) const;

	key_compare key_comp() const;
	value_compare value_comp() const;


	void vložNaZaèátek(T data);
	void vložNaKonec(T data);
	bool prázdný() const;
	void odstraòPrvní();
	void vyprázdni();

	//template< class Key, class T, class Compare, class Alloc >
	//operator<=>(const std::map<Key, T, Compare, Alloc>& lhs,
	//	const std::map<Key, T, Compare, Alloc>& rhs);

	my_map operator+(my_map& s)&;
	my_map operator+(my_map&& s)&;
	my_map operator+(my_map& s)&&;
	my_map operator+(my_map&&)&&;

	void print() const noexcept;

private:
	template<class Compare>
	class CompareValueType
		: public std::binary_function<value_type, value_type, bool>
	{
		Compare _compare;

	public:
		CompareValueType(Compare compare)
			: _compare(compare)
		{}

		bool operator()(const value_type& first, const value_type& second) const
		{
			return compare(first.first, second.first);
		}
	};

	const size_t MIN_NODE_DEGREE = 5;
	using my_map_impl = my_btree<value_type, CompareValueType, MIN_NODE_DEGREE>;

	my_map_impl* _map;
};

template<class Key, class T, class Compare, class Allocator>
inline my_map<Key, T, Compare, Allocator>& my_map<Key, T, Compare, Allocator>::operator=(const my_map& s)
{
	//this->vyprázdni();
	//pøikopírujNaKonec(s);
	return *this;
}

template<class Key, class T, class Compare, class Allocator>
inline my_map<Key, T, Compare, Allocator>& my_map<Key, T, Compare, Allocator>::operator=(my_map&& s)
{
	//prohoï(s);
	return *this;
}

//template<class T>
//inline void my_map<Key, T, Compare, Allocator>::prohoï(my_map<T>&& s)
//{
//	std::swap(hlava, s.hlava);
//	std::swap(zarážka, s.zarážka);
//	std::swap(aktuální, s.aktuální);
//}

template<class Key, class T, class Compare, class Allocator>
inline my_map<Key, T, Compare, Allocator> my_map<Key, T, Compare, Allocator>::operator+(my_map& s)&
{
	//my_map<T> výsledek{ *this };
	//výsledek.pøikopírujNaKonec(s);
	//return výsledek;
}

template<class Key, class T, class Compare, class Allocator>
inline my_map<Key, T, Compare, Allocator> my_map<Key, T, Compare, Allocator>::operator+(my_map&& s)&
{
//	my_map<int> výsledek{ s };

	return my_map();
}

template<class Key, class T, class Compare, class Allocator>
template<bool is_const, bool reversed = false>
class my_map<Key, T, Compare, Allocator>::iterator_impl
{
	friend class my_map<T>;
	using node_pointer = std::conditional_t<is_const, const prvek<T>*, prvek<T>*>;
	node_pointer actual_node;

public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using reference = std::conditional_t<is_const, const T&, T&>;
	using pointer = std::conditional_t<is_const, const T*, T*>;

	iterator_impl(const iterator_impl& iter)
		: actual_node{ iter.actual_node }
	{}

	reference operator*() const
	{
		return actual_node->data;
	}

	pointer operator->() const
	{
		return &(actual_node->data);
	}

	iterator_impl& operator++()
	{
		if (actual_node->další)
		{
			actual_node = actual_node->další;
		}
		return *this;
	}

	iterator_impl operator++(int)
	{
		iterator_impl pom(*this);
		++* this;
		return pom;
	}

	bool operator==(const iterator_impl& iter)
	{
		return actual_node == iter.actual_node;
	}

	bool operator !=(const iterator_impl& iter)
	{
		return !(*this == iter);
	}

private:
	iterator_impl(node_pointer np = nullptr)
		: actual_node(np)
	{}

	node_pointer ukazatel_na_aktuální_prvek() const
	{
		return actual_node;
	}
};

