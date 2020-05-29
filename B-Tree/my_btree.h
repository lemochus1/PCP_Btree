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

	template<bool is_const, bool reversed=false>
	class iterator_impl;

public:
	using iterator = iterator_impl<true>;
	using const_iterator = iterator_impl<false>;
	
	using reverse_iterator = iterator_impl<true, true>;
	using const_reverse_iterator = iterator_impl<false, true>;
	
	my_btree();
	my_btree(my_btree& other);
	my_btree(my_btree&& other);
	~my_btree();

	[[nodiscard]] bool empty() const noexcept;
	[[nodiscard]] size_t size() const noexcept;

	void insert(const T& key);
	void remove(const T& key);

	void erase(const T& key);
	void clear() noexcept;

	[[nodiscard]] size_t count(const T& key) const;

	[[nodiscard]] bool contains(const T& key) const;
	
	void print() const noexcept;



	iterator begin() noexcept
	{
		return iterator(findBegin(), 0);
	}

	iterator end() noexcept
	{
		return iterator();
	}

	const_iterator cbegin() const noexcept
	{
		return const_iterator(findBegin(), 0);
	}

	const_iterator cend() const noexcept 
	{
		return const_iterator();
	}

	const_iterator begin() const noexcept
	{
		return const_iterator(findBegin(), 0);
	}

	const_iterator end() const noexcept
	{
		return const_iterator();
	}

	reverse_iterator rbegin() noexcept
	{
		auto last_node = findEnd();
		size_t last_index = 0;
		if (last_node) {
			last_index = last_node->keyCount() - 1;
		}
		return reverse_iterator(last_node, last_index);
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator();
	}

	const_reverse_iterator rbegin() const noexcept
	{
		auto last_node = findEnd();
		size_t last_index = 0;
		if (last_node) {
			last_index = last_node->keyCount() - 1;
		}
		return const_reverse_iterator(last_node, last_index);
	}

	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator();
	}

	const_reverse_iterator crbegin() const noexcept
	{
		auto last_node = findEnd();
		size_t last_index = 0;
		if (last_node) {
			last_index = last_node->keyCount() - 1;
		}
		return const_reverse_iterator(last_node, last_index);
	}

	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator();
	}

private:
	my_btree_node<T, Compare, MIN_DEGREE>* _root;

	size_t _size;
	Compare _compare;//mel by byt static... pak v map z toho udelat nested...


	my_btree_node<T, Compare, MIN_DEGREE>* findBegin();
	my_btree_node<T, Compare, MIN_DEGREE>* findEnd();
	
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
inline my_btree_node<T, Compare, MIN_DEGREE>* my_btree<T, Compare, MIN_DEGREE>::findEnd()
{
	if (empty()) {
		return nullptr;
	}

	auto current_node = _root;
	while (!current_node->leaf()) {
		current_node = current_node->_children[current_node->keyCount()];
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
			s->placeChild(_root, 0);

			s->splitChild(0, _root);
			int i = 0;
			if (_compare(s->_keys[0], key)) {
				i++;
			}
			s->_children[i]->insert(key);
			_root = s;
			_root->my_child_index = 0;
			_root->parent = nullptr;
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
template<bool is_const, bool reversed=false>
class my_btree<T, Compare, MIN_DEGREE>::iterator_impl
{
	friend class my_btree<T, Compare, MIN_DEGREE>;

	using node_pointer = std::conditional_t<is_const, const my_btree_node<T, Compare, MIN_DEGREE>*, my_btree_node<T, Compare, MIN_DEGREE>*>;

	node_pointer current_node;
	int current_index;

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
			move_leaf_node<reversed>();
		}
		else {
			move_nonleaf_node<reversed>();
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
		current_index(index)
	{
	}

	node_pointer get_current_node() const
	{
		return current_node;
	}

	template<bool reversed>
	std::enable_if_t<!reversed> move_leaf_node()
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

	template<bool reversed>
	std::enable_if_t<reversed> move_leaf_node()
	{
		if (current_index > 0) {
			current_index--;
			return;
		}

		while (current_node->parent) {
			current_index = current_node->my_child_index - 1;
			current_node = current_node->parent;
			if (current_index >= 0) {
				std::cout << "k" << current_index;
				return;
			}
		}
		std::cout << "nullptr";
		current_node = nullptr;
		current_index = 0;
	}

	template<bool reversed>
	std::enable_if_t<!reversed> move_nonleaf_node()
	{
		current_node = current_node->_children[current_index + 1];
		current_index = 0;
		while (!current_node->leaf()) {
			current_node = current_node->_children[current_index];
		}
	}

	template<bool reversed>
	std::enable_if_t<reversed> move_nonleaf_node()
	{
		current_node = current_node->_children[current_index];
		current_index = current_node->keyCount() - 1;
		while (!current_node->leaf()) {
			current_node = current_node->_children[current_index + 1];
			current_index = current_node->keyCount() - 1;
		}
	}
};
