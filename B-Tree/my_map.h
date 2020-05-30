#pragma once
#include <algorithm>
#include <iostream>
#include <utility>
#include <type_traits>
#include <iterator>
#include <memory>
#include "my_btree.h"

const size_t MIN_NODE_DEGREE = 3;

template<class Key, class T, class Compare = std::less<Key>> 
class my_map
{
	template<bool is_const, bool reversed = false>
	class iterator_impl;

	struct value_type;
	class value_compare;

public:
	using key_type = Key;
	using mapped_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using key_compare = Compare;
	using reference = value_type&;
	using const_reference = const value_type&;

	using iterator = iterator_impl<false>;
	using const_iterator = iterator_impl<true>;
	using reverse_iterator = iterator_impl<false, true>;
	using const_reverse_iterator = iterator_impl<true, false>;

	my_map();
	my_map(my_map& other);
	my_map(my_map&& other);
	my_map(std::initializer_list<std::pair<Key, T>> init);
	~my_map() = default;
	
	my_map& operator=(const my_map& other);
	my_map& operator=(my_map&& other) noexcept;
	my_map& operator=(std::initializer_list<std::pair<Key, T>> ilist);
	
	T& at(const Key& key);
	const T& at(const Key& key) const;
	T& operator[](const Key& key);
	T& operator[](Key&& key);

	my_map operator+(my_map& s)&;
	
	[[nodiscard]] bool empty() const noexcept;
	size_type size() const noexcept;

	void clear() noexcept;
	void insert();
	template<class... Args>
	std::pair<iterator, bool> emplace(Args&&... args);
	template <class... Args>
	std::pair<iterator, bool> try_emplace(const key_type& key, Args&&... args);
	iterator erase(iterator pos);
	iterator erase(const_iterator pos);
	size_type erase(const key_type& key);

	void swap(my_map& other) noexcept;
	template<class OtherCompare>
	void merge(my_map<Key, T, OtherCompare> &other);

	size_type count(const Key& key) const;
	iterator find(const Key& key);
	const_iterator find(const Key& key) const;
	bool contains(const Key& key) const;

	iterator begin() noexcept {return _map->begin()}
	iterator end() noexcept { return _map->end() }
	const_iterator cbegin() const noexcept { return _map->cbegin() }
	const_iterator cend() const noexcept { return _map->cend() }
	const_iterator begin() const noexcept { return _map->begin() }
	const_iterator end() const noexcept { return _map->end() }

	reverse_iterator rbegin() noexcept { return _map->rbegin() }
	reverse_iterator rend() noexcept { return _map->rend() }
	const_reverse_iterator crbegin() const noexcept { return _map->crbegin() }
	const_reverse_iterator crend() const noexcept { return _map->crend() }
	const_reverse_iterator rbegin() const noexcept { return _map->rbegin() }
	const_reverse_iterator rend() const noexcept { return _map->rend() }

	void print() const noexcept;

	template<class T>
	auto serialize_imp(std::ostream& os, T const& obj, int)
		-> decltype(os << obj, void())
	{
		os << obj;
	}

	template<class T>
	auto serialize_imp(std::ostream& os, T const& obj, long)
		-> decltype(obj.stream(os), void())
	{
		obj.stream(os);
	}

	template<class T>
	auto serialize(std::ostream& os, T const& obj)
		-> decltype(serialize_imp(os, obj, 0), void())
	{
		serialize_imp(os, obj, 0);
	}



private:
	struct value_type
	{
		Key first;
		T second;

		value_type(const Key& key) {
			first = key;
		}

		value_type(Key&& key) {
			first = std::move(key);
		}

		value_type(const std::pair<Key, T>& pair) {
			first = pair.first;
			second = pair.second;
		}

	};

	class CompareValueType
		: public std::binary_function<value_type, value_type, bool>
	{
		Compare _compare;

	public:
		CompareValueType(Compare compare)
			: _compare(compare)
		{ }

		bool operator()(const value_type& first, const value_type& second) const
		{
			return compare(first.first, second.first);
		}
	};

	using my_map_impl = my_btree<value_type, CompareValueType, MIN_NODE_DEGREE>;
	my_map_impl* _map;
};

template<class Key, class T, class Compare>
inline my_map<Key, T, Compare>::my_map()
{
	_map = new my_map_impl{ };
}

template<class Key, class T, class Compare>
inline my_map<Key, T, Compare>::my_map(my_map& other)
{
	_map = new my_map_impl{ other._map };
}

template<class Key, class T, class Compare>
inline my_map<Key, T, Compare>::my_map(my_map&& other)
	: my_map{ }
{
	std::swap(_map, other.map);
}

template<class Key, class T, class Compare>
inline my_map<Key, T, Compare>::my_map(std::initializer_list<std::pair<Key, T>> init)
	: my_map{ }
{
	for (const auto& value : init) {
		if (contains(value)) {
			throw
		}
		_map->insert(value);
	}
}

template<class Key, class T, class Compare>
inline my_map<Key,T,Compare>& my_map<Key, T, Compare>::operator=(const my_map<Key, T, Compare>& other)
{
	_map = other._map;
	return *this;
}

template<class Key, class T, class Compare>
inline my_map<Key, T, Compare>& my_map<Key, T, Compare>::operator=(my_map&& other) noexcept
{
	std::swap(_map, other._map);
	return *this;
}

template<class Key, class T, class Compare>
inline my_map<Key, T, Compare>& my_map<Key, T, Compare>::operator=(std::initializer_list<std::pair<Key, T>> ilist)
{
	clear();
	for (const auto& value : init) {
		if (contains(value)) {
			throw 
		}
		_map->insert(value);
	}
	return *this;
}

template<class Key, class T, class Compare>
inline T& my_map<Key, T, Compare>::at(const Key& key)
{
	auto iter = _map->find(key);
	if (iter == _map->end()) {
		throw std::out_of_range("Does not contains the key.");
	}
	return *iter;
}

template<class Key, class T, class Compare>
inline const T& my_map<Key, T, Compare>::at(const Key& key) const
{
	auto iter = _map->find(key);
	if (iter == _map->end()) {
		throw std::out_of_range("Does not contains the key.");
	}
	return *iter;
}

template<class Key, class T, class Compare>
inline T& my_map<Key, T, Compare>::operator[](const Key& key)
{
	return at(key);
}

template<class Key, class T, class Compare>
inline T& my_map<Key, T, Compare>::operator[](Key&& key)
{
	return at(std::move(key));
}

template<class Key, class T, class Compare>
inline my_map<Key, T, Compare> my_map<Key, T, Compare>::operator+(my_map& s)&
{
	my_map<Key, T, Compare> result{ *this };
	result._map->concat(other._map);
	return result;
}

template<class Key, class T, class Compare>
inline bool my_map<Key, T, Compare>::empty() const noexcept
{
	return _map->empty();
}

template<class Key, class T, class Compare>
inline typename my_map<Key, T, Compare>::size_type my_map<Key, T, Compare>::size() const noexcept
{
	return _map->size();
}

template<class Key, class T, class Compare>
inline void my_map<Key, T, Compare>::clear() noexcept
{
	_map->clear();
}

template<class Key, class T, class Compare>
inline void my_map<Key, T, Compare>::swap(my_map& other) noexcept
{
	std::swap(_map, other->_map);
}

template<class Key, class T, class Compare>
inline typename my_map<Key, T, Compare>::size_type my_map<Key, T, Compare>::count(const Key& key) const
{
	return _map->count(createValue(key));
}

template<class Key, class T, class Compare>
inline bool my_map<Key, T, Compare>::contains(const Key& key) const
{
	return find(key) != end();
}

template<class Key, class T, class Compare>
template<class OtherCompare>
inline void my_map<Key, T, Compare>::merge(my_map<Key, T, OtherCompare>& other)
{
	for (const auto& value : other) {
		_map->insert(value);
	}
}

template<class Key, class T, class Compare>
inline void my_map<Key, T, Compare>::print() const noexcept
{
	_map->print();
}
