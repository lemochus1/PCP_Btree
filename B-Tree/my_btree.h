#pragma once
#include <algorithm>
#include <iostream>
#include <utility>
#include <type_traits>
#include <iterator>
#include <memory>
#include <cassert>

#include "my_btree_node.h"

#define assertm(exp, msg) assert(((void)msg, exp))

template<class T, class Compare = std::less<T>, int MIN_DEGREE = 3 >
class my_btree {

	template<bool is_const>
	class iterator_impl;

public:
	using const_iterator = iterator_impl<false>;
	using iterator = iterator_impl<true>;

	my_btree();
	~my_btree();

	[[nodiscard]] bool empty() const noexcept;
	size_t size() const noexcept;

	void clear() noexcept;
	void erase(const T& key);

	size_t count(const T& key) const;

	bool contains(const T& key) const;
	
	void print() const noexcept;

	void insert(const T& key);
	void remove(const T& key);


	const_iterator begin()
	{
		return const_iterator(findBegin(), 0);
	}

	const_iterator end()
	{
		return const_iterator();
	}

private:
	my_btree_node<T, Compare, MIN_DEGREE>* _root;

	size_t _size;
	Compare _compare;//mel by byt static...


	my_btree_node<T, Compare, MIN_DEGREE>* findBegin();
	
};

template<class T, class Compare, int MIN_DEGREE>
my_btree_node<T, Compare, MIN_DEGREE>* my_btree<T, Compare, MIN_DEGREE>::findBegin()
{
	if (empty()) {
		return nullptr;
	}

	auto current_node = _root;
	while (!current_node->leaf()) {
		current_node = current_node->_children[0];
	}
	return current_node;
}


template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE>::my_btree()
	: _size(0)
{
	_root = nullptr;
}

template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE>::~my_btree()
{
	clear();
}

template<class T, class Compare, int MIN_DEGREE>
inline bool my_btree<T, Compare, MIN_DEGREE>::empty() const noexcept
{
	assertm((_root == nullptr) == (_size == 0), "Root == nullptr and size == 0 are not equal.");
	return _root == nullptr;
}

template<class T, class Compare, int MIN_DEGREE>
inline size_t my_btree<T, Compare, MIN_DEGREE>::size() const noexcept
{
	return _size;
}

template<class T, class Compare, int MIN_DEGREE>
inline void my_btree<T, Compare, MIN_DEGREE>::clear() noexcept
{
	if (_root) {
		_root->clear();
		delete _root;
		_root = nullptr;
		_size = 0;
	}
}

template<class T, class Compare, int MIN_DEGREE>
inline size_t my_btree<T, Compare, MIN_DEGREE>::count(const T& key) const
{
	if (_root) {
		return _root->count(key);
	}
	return 0;
}

template<class T, class Compare, int MIN_DEGREE>
inline bool my_btree<T, Compare, MIN_DEGREE>::contains(const T& key) const
{
	return count(key) > 0;
}

template<class T, class Compare, int MIN_DEGREE>
inline void my_btree<T, Compare, MIN_DEGREE>::print() const noexcept
{
	if (_root) {
		_root->print(0);
	}
}

template<class T, class Compare, int MIN_DEGREE>
inline void my_btree<T, Compare, MIN_DEGREE>::insert(const T& key)
{
	if (_root) {
		if (_root->full()) {
			my_btree_node<T, Compare, MIN_DEGREE>* s = new my_btree_node<T, Compare, MIN_DEGREE>(false);
			s->_children[0] = _root;
			s->splitChild(0, _root);
			int i = 0;
			if (_compare(s->_keys[0], key)) {
				i++;
			}
			s->_children[i]->insert(key);
			_root = s;
		}
		else {
			_root->insert(key);
		}
	}
	else {
		_root = new my_btree_node<T, Compare, MIN_DEGREE>(true);
		_root->_keys[0] = key;
		_root->_key_count = 1;
	}
	_size++;
}

template<class T, class Compare, int MIN_DEGREE>
inline void my_btree<T, Compare, MIN_DEGREE>::remove(const T& key)
{
	if (!_root) {
		return;
	}
	_root->remove(key);
	if (_root->_key_count == 0) {
		my_btree_node<T, Compare, MIN_DEGREE>* tmp = _root;
		if (_root->leaf()) {
			_root = nullptr;
		}
		else {
			_root = _root->_children[0];
		}
		delete tmp;
	}
	_size--;//error, co kdyz tu vubec neni...
}


template<class T, class Compare, int MIN_DEGREE>
template<bool is_const>
class my_btree<T, Compare, MIN_DEGREE>::iterator_impl
{
	friend class my_btree<T, Compare, MIN_DEGREE>;

	using node_pointer = std::conditional_t<is_const, const my_btree_node<T, Compare, MIN_DEGREE>*, my_btree_node<T, Compare, MIN_DEGREE>*>;

	node_pointer current_node;
	size_t current_index;

public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using reference = std::conditional_t<is_const, const T&, T&>;
	using pointer = std::conditional_t<is_const, const T*, T*>;

	iterator_impl(const iterator_impl& iter)
		: current_node{ iter.current_node },
		current_index{ iter.current_index }
	{}

	reference operator*() const
	{
		return current_node->_keys[current_index];
	}

	pointer operator->() const
	{
		return &(current_node->_keys[current_index]);
	}

	iterator_impl& operator++()
	{
		if (current_node->leaf()) {
			move_leaf_node();
		}
		else {
			move_nonleaf_node();
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
		return current_node == iter.current_node && current_index == iter.current_index;
	}

	bool operator !=(const iterator_impl& iter)
	{
		return !(*this == iter);
	}

private:
	iterator_impl(node_pointer np = nullptr, size_t index = 0)
		: current_node(np),
		current_index(0)
	{
	}

	node_pointer get_current_node() const
	{
		return current_node;
	}

	void move_leaf_node()
	{
		if (current_index < current_node->keyCount() - 1) {
			current_index++;
			return;
		}

		while (current_node->parent) {
			current_index = current_node->my_child_index;
			current_node = current_node->parent;
			if (current_index < current_node->keyCount()) {
				return;
			}
		}
		current_node = nullptr;
		current_index = 0;
	}

	void move_nonleaf_node() 
	{
		current_node = current_node->_children[current_index + 1];
		current_index = 0;
		while (!current_node->leaf()) {
			current_node = current_node->_children[current_index];
		}
	}
};
