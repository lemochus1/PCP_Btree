#pragma once
#include <algorithm>
#include <iostream>
#include <utility>
#include <type_traits>
#include <iterator>
#include <memory>
#include <cassert>
#include <stdexcept> 

#include "my_btree_node.h"
#include "has_member.h"
#include "member_type.h"

#define assertm(exp, msg) assert(((void)msg, exp))

template<class T, class Compare = std::less<T>, int MIN_DEGREE = 3 >
class my_btree {

	template<bool is_const, bool reversed=false>
	class iterator_impl;

	using node_type = my_btree_node<T, Compare, MIN_DEGREE>;

public:
	using value_type = T;

	using iterator = iterator_impl<true>;
	using const_iterator = iterator_impl<false>;
	
	using reverse_iterator = iterator_impl<true, true>;
	using const_reverse_iterator = iterator_impl<false, true>;
	
	my_btree();
	my_btree(my_btree& other);
	my_btree(my_btree&& other);
	my_btree(std::initializer_list<value_type> init);
	~my_btree();

	[[nodiscard]] bool empty() const noexcept;
	[[nodiscard]] size_t size() const noexcept;

	void insert(const value_type& key);

	void remove(const value_type& key);
	void erase(const value_type& key);
	void clear() noexcept;

	[[nodiscard]] size_t count(const value_type& key) const;
	[[nodiscard]] bool contains(const value_type& key) const;
	
	[[nodiscard]] iterator find(const value_type& key);
	[[nodiscard]] const_iterator find(const value_type& key) const;

	void concat(my_btree<T, Compare, MIN_DEGREE>& other);
	void swap(my_btree<T, Compare, MIN_DEGREE>&& other);

	void print() const noexcept;

	my_btree& operator=(const my_btree& other);
	my_btree& operator=(my_btree&& other);
	my_btree operator+(my_btree& other)&;

	iterator begin() noexcept { return iterator{ findBegin(), 0 }; }
	iterator end() noexcept { return iterator{ }; }
	const_iterator cbegin() const noexcept { return const_iterator{ findBegin(), 0 }; }
	const_iterator cend() const noexcept { return const_iterator{ }; }
	const_iterator begin() const noexcept { cbegin(); }
	const_iterator end() const noexcept { cend(); }
	reverse_iterator rbegin() noexcept;
	reverse_iterator rend() noexcept { return reverse_iterator{ }; }
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator{ }; }
	const_reverse_iterator rbegin() const noexcept { return crbegin();}
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator{ }; }

private:
	node_type* _root;

	size_t _size;
	Compare _compare;//mel by byt static... pak v map z toho udelat nested...

	node_type* findBegin() const noexcept;
	node_type* findEnd() const noexcept;
	std::pair<node_type*, int> findReversedBegin() const noexcept;
};

template<class T, class Compare, int MIN_DEGREE>
typename typename my_btree<T, Compare, MIN_DEGREE>::node_type* my_btree<T, Compare, MIN_DEGREE>::findBegin() const noexcept
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
inline typename my_btree<T, Compare, MIN_DEGREE>::node_type* my_btree<T, Compare, MIN_DEGREE>::findEnd() const noexcept
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
inline void my_btree<T, Compare, MIN_DEGREE>::concat(my_btree<T, Compare, MIN_DEGREE>& other)
{
	for (const auto& key : other) {
		insert(key);
	}
}

template<class T, class Compare, int MIN_DEGREE>
inline void my_btree<T, Compare, MIN_DEGREE>::swap(my_btree<T, Compare, MIN_DEGREE>&& other)
{
	std::swap(_root, other._root);
	std::swap(_size, other._size);
}

template<class T, class Compare, int MIN_DEGREE>
inline std::pair<typename my_btree<T, Compare, MIN_DEGREE>::node_type*, int> my_btree<T, Compare, MIN_DEGREE>::findReversedBegin() const noexcept
{
	auto last_node = findEnd();
	size_t last_index = 0;
	if (last_node) {
		last_index = last_node->keyCount() - 1;
	}
	return { last_node, last_index };
}

template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE>::my_btree()
	: _size{ 0 }
{
	_root = nullptr;
}

template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE>::my_btree(my_btree& other)
	: my_btree{ }
{
	concat(other);
}

template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE>::my_btree(my_btree&& other)
	: my_btree{ }
{
	swap(std::move(other));
}

template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE>::my_btree(std::initializer_list<T> init)
	: my_btree{ }
{
	for (const auto& value : init) {
		insert(value);
	}
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
	if (!empty()) {
		_root->clear();
		delete _root;
		_root = nullptr;
		_size = 0;
	}
}

template<class T, class Compare, int MIN_DEGREE>
inline size_t my_btree<T, Compare, MIN_DEGREE>::count(const value_type& key) const
{
	if (empty()) {
		return 0;
	}
	return _root->count(key);
}

template<class T, class Compare, int MIN_DEGREE>
inline bool my_btree<T, Compare, MIN_DEGREE>::contains(const value_type& key) const
{
	return count(key) > 0;
}

template<class T, class Compare, int MIN_DEGREE>
inline typename my_btree<T, Compare, MIN_DEGREE>::iterator my_btree<T, Compare, MIN_DEGREE>::find(const value_type& key)
{
	if (empty()) {
		return iterator{ };
	}
	auto [node, idx] = _root->find(key);
	return iterator{ node, idx };
}

template<class T, class Compare, int MIN_DEGREE>
inline typename my_btree<T, Compare, MIN_DEGREE>::const_iterator my_btree<T, Compare, MIN_DEGREE>::find(const value_type& key) const
{
	if (empty()) {
		return const_iterator{ };
	}
	auto [node, idx] = _root->find(key);
	return const_iterator{ node, idx };
}

template<class T, class Compare, int MIN_DEGREE>
inline void my_btree<T, Compare, MIN_DEGREE>::print() const noexcept
{
	if (!empty()) {
		_root->print(0);
	}
}

template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE>& my_btree<T, Compare, MIN_DEGREE>::operator=(const my_btree<T, Compare, MIN_DEGREE>& other)
{
	clear();
	concat(other);
	return *this;
}

template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE>& my_btree<T, Compare, MIN_DEGREE>::operator=(my_btree<T, Compare, MIN_DEGREE>&& other)
{
	swap(other);
	return *this;
}

template<class T, class Compare, int MIN_DEGREE>
inline my_btree<T, Compare, MIN_DEGREE> my_btree<T, Compare, MIN_DEGREE>::operator+(my_btree<T, Compare, MIN_DEGREE>& other)&
{
	my_btree<T, Compare, MIN_DEGREE> result{ *this };
	result.concat(other);
	return result;
}

template<class T, class Compare, int MIN_DEGREE>
inline typename my_btree<T,Compare,MIN_DEGREE>::reverse_iterator my_btree<T, Compare, MIN_DEGREE>::rbegin() noexcept
{
	auto [node, index] = findReversedBegin();
	return reverse_iterator{ node, index };
}

template<class T, class Compare, int MIN_DEGREE>
inline typename my_btree<T, Compare, MIN_DEGREE>::const_reverse_iterator my_btree<T, Compare, MIN_DEGREE>::crbegin() const noexcept
{
	auto [node, index] = findReversedBegin();
	return const_reverse_iterator{ node, index };
}


template<class T, class Compare, int MIN_DEGREE>
inline void my_btree<T, Compare, MIN_DEGREE>::insert(const value_type& key)
{
	if (_root) {
		if (_root->full()) {
			my_btree_node<T, Compare, MIN_DEGREE>* s = new my_btree_node<T, Compare, MIN_DEGREE>{ false };
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
inline void my_btree<T, Compare, MIN_DEGREE>::remove(const value_type& key)
{
	if (empty() || !contains(key)) {
		throw std::out_of_range("Tried to remove a key from an empty my_btree.");
	}
	_root->remove(key);
	if (_root->_key_count == 0) {
		node_type* tmp = _root;
		if (_root->leaf()) {
			_root = nullptr;
		}
		else {
			_root = _root->_children[0];
		}
		delete tmp;
	}
	_size--;
}

template<class T, class Compare, int MIN_DEGREE>
inline void my_btree<T, Compare, MIN_DEGREE>::erase(const value_type& key)
{
	do {
		remove(key);
	} while (contains(key));
}

GENERATE_HAS_MEMBER(first)
GENERATE_MEMBER_TYPE(first)

template<class T, class Compare, int MIN_DEGREE>
template<bool is_const, bool reversed>
class my_btree<T, Compare, MIN_DEGREE>::iterator_impl
{
	friend class my_btree<T, Compare, MIN_DEGREE>;

	using node_pointer = std::conditional_t<is_const, const my_btree_node<T, Compare, MIN_DEGREE>*, my_btree_node<T, Compare, MIN_DEGREE>*>;

	node_pointer _current_node;
	int _current_index;

public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = std::conditional_t<has_first_v<T>, first_t<T>, T>;
	using difference_type = std::ptrdiff_t;
	using reference = std::conditional_t<is_const, const value_type&, value_type&>;
	using pointer = std::conditional_t<is_const, const value_type*, value_type*>;

	iterator_impl(const iterator_impl& iter)
		: _current_node{ iter._current_node },
		_current_index{ iter._current_index }
	{ }

	reference operator*() const 
	{ 
		return *operator->();
		//if constexpr (has_first_v<T>) {
		//	if constexpr (std::is_pointer_v<T>) {
		//		return _current_node->_keys[_current_index]->first;
		//	}
		//	else {
		//		return _current_node->_keys[_current_index].first;
		//	}
		//}
		//else {
		//	return _current_node->_keys[_current_index];
		//}
	}
	pointer operator->() const 
	{ 
		if constexpr (has_first_v<T>) {
			if constexpr (std::is_pointer_v<T>) {
				return &(_current_node->_keys[_current_index]->first);
			}
			else {
				return &(_current_node->_keys[_current_index].first);
			}
		}
		else {
			return &(_current_node->_keys[_current_index]);
		}
	}

	iterator_impl& operator++() { move_self(); return *this; }
	iterator_impl operator++(int)
	{
		iterator_impl pom(*this);
		++* this;
		return pom;
	}

	bool operator==(const iterator_impl& iter)
	{
		return _current_node == iter._current_node && _current_index == iter._current_index;
	}
	bool operator !=(const iterator_impl& iter) { return !(*this == iter); }

private:
	iterator_impl(node_pointer np = nullptr, int index = 0)
		: _current_node{ np },
		_current_index{ index }
	{ }

	node_pointer get_current_node() const { return _current_node; }

	void move_self() {
		move_self<reversed>();
	}

	template<bool reversed>
	std::enable_if_t<!reversed> move_self()
	{
		if (!_current_node->leaf()) {
			_current_node = _current_node->_children[_current_index + 1];
			_current_index = 0;
			while (!_current_node->leaf()) {
				_current_node = _current_node->_children[_current_index];
			}
		}
		else {
			if (_current_index < _current_node->keyCount() - 1) {
				_current_index++;
				return;
			}
			while (_current_node->parent) {
				_current_index = _current_node->my_child_index;
				_current_node = _current_node->parent;
				if (_current_index < _current_node->keyCount()) {
					return;
				}
			}
			_current_node = nullptr;
			_current_index = 0;
		}
	}

	template<bool reversed>
	std::enable_if_t<reversed> move_self()
	{
		if (!_current_node->leaf()) {
			_current_node = _current_node->_children[_current_index];
			_current_index = _current_node->keyCount() - 1;
			while (!_current_node->leaf()) {
				_current_node = _current_node->_children[_current_index + 1];
				_current_index = _current_node->keyCount() - 1;
			}
		}
		else {
			if (_current_index > 0) {
				_current_index--;
				return;
			}
			while (_current_node->parent) {
				_current_index = _current_node->my_child_index - 1;
				_current_node = _current_node->parent;
				if (_current_index >= 0) {
					return;
				}
			}
			_current_node = nullptr;
			_current_index = 0;
		}
	}
};
