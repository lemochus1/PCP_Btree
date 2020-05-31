#pragma once
#include <iostream>

template <class T, class Compare, int min_degree>
class my_btree_node {
	template<class, class, int>
	friend class my_btree;

public:
	using value_type = T;

	my_btree_node(bool leaf);
	~my_btree_node() = default;

	bool leaf() const noexcept;
	bool full() const noexcept;
	bool sufficientKeyCount() const noexcept;

	size_t keyCount() const noexcept;
	size_t childCount() const noexcept;
	size_t count(const value_type& key) const;
	std::pair<my_btree_node*, int> find(const value_type& key) noexcept;
	std::pair<const my_btree_node*, int> find(const value_type& key) const noexcept;

	void insert(const value_type& key);

	void remove(const value_type& key);
	void clear() noexcept;

private:
	int findKey(const value_type& key) const;

	void splitChild(int child_index);

	void removeFromLeaf(int idx);
	void removeFromNonLeaf(int idx);

	value_type getPreviousKey(int idx);
	value_type getNextKey(int idx);

	void fill(int idx);
	void borrowFromPrev(int idx);
	void borrowFromNext(int idx);
	void merge(int idx);

	void placeChild(my_btree_node* node, int idx);

	void print(int deepness) const noexcept;
	my_btree_node* search(const value_type& key) noexcept;
	const my_btree_node* search(const value_type& key) const noexcept;

	bool _leaf;
	size_t _key_count;

	my_btree_node<T, Compare, min_degree>* _children[2 * min_degree];
	value_type _keys[2* min_degree - 1];

	my_btree_node<T, Compare, min_degree>* parent;
	size_t my_child_index;

	static Compare compare;
};

template<class T, class Compare, int min_degree>
Compare my_btree_node<T, Compare, min_degree>::compare = Compare();

template<class T, class Compare, int min_degree>
inline my_btree_node<T, Compare, min_degree>::my_btree_node(bool leaf)
	: _leaf{ leaf },
	_key_count{ 0 },
	my_child_index{ 0 }
{
	parent = nullptr;
}

template<class T, class Compare, int min_degree>
inline bool my_btree_node<T, Compare, min_degree>::leaf() const noexcept
{
	return _leaf;
}

template<class T, class Compare, int min_degree>
inline bool my_btree_node<T, Compare, min_degree>::full() const noexcept
{
	return keyCount() == 2 * min_degree - 1;
}

template<class T, class Compare, int min_degree>
inline bool my_btree_node<T, Compare, min_degree>::sufficientKeyCount() const noexcept
{
	return keyCount() >= min_degree - 1;
}

template<class T, class Compare, int min_degree>
inline size_t my_btree_node<T, Compare, min_degree>::keyCount() const noexcept
{
	return _key_count;
}

template<class T, class Compare, int min_degree>
inline size_t my_btree_node<T, Compare, min_degree>::childCount() const noexcept
{
	return (leaf() || keyCount() == 0) ? 0 : keyCount() + 1;
}

template<class T, class Compare, int min_degree>
inline size_t my_btree_node<T, Compare, min_degree>::count(const value_type& key) const
{
	size_t total = 0;
	int idx = findKey(key);
	if (idx < childCount()) {
		total += _children[idx]->count(key);
	}
	for (int i = idx; i < keyCount(); i++) {
		if (_keys[i] == key) {
			total++;
			if (i + 1 < childCount()) {
				total += _children[i + 1]->count(key);
			}
		}
	}
	return total;
}

template<class T, class Compare, int min_degree>
inline std::pair<const my_btree_node<T, Compare, min_degree>*, int> my_btree_node<T, Compare, min_degree>::find(const value_type& key) const noexcept
{
	auto node = search(key);
	if (!node) {
		return { nullptr, 0 };
	}
	int idx = node->findKey(key);
	if (idx < node->keyCount()) {
		return { node, idx };
	}
	return { nullptr, 0 };
}


template<class T, class Compare, int min_degree>
inline std::pair<my_btree_node<T, Compare, min_degree>*, int> my_btree_node<T, Compare, min_degree>::find(const value_type& key) noexcept
{
	auto node = search(key);
	if (!node) {
		return { nullptr, 0 };
	}
	int idx = node->findKey(key);
	if (idx < node->keyCount()) {
		return { node, idx };
	}
	return { nullptr, 0 };
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::clear() noexcept
{
	for (int i = 0; i < childCount(); i++) {
		_children[i]->clear();
		delete _children[i];
	}
	_key_count = 0;
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::print(int deepness) const noexcept
{
	std::string indentation(2 * deepness, ' ');

	if (leaf()) {
		std::cout << indentation;
		for (int i = 0; i < keyCount(); i++) {
			std::cout << " " << _keys[i];
		}
		std::cout << "'" << my_child_index << "'"<< std::endl;
	}
	else {
		for (int i = 0; i < keyCount(); i++) {
			_children[i]->print(deepness + 1);
			std::cout << indentation << _keys[i] << std::endl;
		}
		_children[keyCount()]->print(deepness + 1);
		std::cout << indentation << "'" << my_child_index << "'" << std::endl;
	}
}
template<class T, class Compare, int min_degree>
inline my_btree_node<T, Compare, min_degree>* my_btree_node<T, Compare, min_degree>::search(const value_type& key) noexcept
{
	int i = 0;
	while (i < keyCount() && compare(_keys[i], key)) {
		i++;
	}
	if (_keys[i] == key) {
		return this;
	}
	if (leaf()) {
		return nullptr;
	}
	return _children[i]->search(key);
}

template<class T, class Compare, int min_degree>
inline const my_btree_node<T, Compare, min_degree>* my_btree_node<T, Compare, min_degree>::search(const value_type& key) const noexcept
{
	int i = 0;
	while (i < keyCount() && compare(_keys[i], key)) {
		i++;
	}
	if (_keys[i] == key) {
		return this;
	}
	if (leaf()) {
		return nullptr;
	}
	return _children[i]->search(key);
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::insert(const value_type& key)
{
	int i = keyCount() - 1;

	if (leaf()) {
		while (i >= 0 && compare(key, _keys[i])) {
			_keys[i + 1] = _keys[i];
			i--;
		}
		_keys[i + 1] = key;
		_key_count++;
	}
	else {
		while (i >= 0 && compare(key, _keys[i])) {
			i--;
		}
		if (_children[i + 1]->full()) {
			splitChild(i + 1);
			if (compare(_keys[i + 1], key)) {
				i++;
			}
		}
		_children[i + 1]->insert(key);
	}
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::splitChild(int child_index)
{
	my_btree_node* node = _children[child_index];
	my_btree_node<T, Compare, min_degree>* new_node = new my_btree_node<T, Compare, min_degree> (node->leaf());
	new_node->_key_count = min_degree - 1;

	for (int j = 0; j < min_degree -1; j++) {
		new_node->_keys[j] = node->_keys[j + min_degree];
	}

	if (!node->leaf()) {
		for (int j = 0; j < min_degree; j++) {
			new_node->placeChild(node->_children[j + min_degree], j);
		}
	}
	node->_key_count = min_degree - 1;

	for (int j = keyCount(); j >= child_index+1; j--) {
		placeChild(_children[j], j + 1);
	}

	placeChild(new_node, child_index + 1);
	
	for (int j = keyCount() - 1; j >= child_index; j--) {
		_keys[j + 1] = _keys[j];
	}
	_keys[child_index] = node->_keys[min_degree - 1];
	_key_count++;
}

template<class T, class Compare, int min_degree>
inline int my_btree_node<T, Compare, min_degree>::findKey(const value_type& key) const
{
	int idx = 0;
	while (idx < keyCount() && compare(_keys[idx], key)) {
		idx++;
	}
	return idx;
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::remove(const value_type& key)
{
	int idx = findKey(key);
	if (idx < keyCount() && _keys[idx] == key) {
		if (leaf()) {
			removeFromLeaf(idx);
		}
		else {
			removeFromNonLeaf(idx);
		}
	}
	else if (!leaf()) {
		bool last_kid = (idx == keyCount());

		if (_children[idx]->keyCount() < min_degree) {
			fill(idx);
		}
		if (last_kid && idx > keyCount()) {
			_children[idx - 1]->remove(key);
		}
		else {
			_children[idx]->remove(key);
		}
	}
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::removeFromLeaf(int idx)
{
	for (int i = idx + 1; i < keyCount(); i++) {
		_keys[i - 1] = _keys[i];
	}
	_key_count--;
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::removeFromNonLeaf(int idx)
{
	value_type key = _keys[idx];
	if (_children[idx]->keyCount() >= min_degree)
	{
		value_type prev = getPreviousKey(idx);
		_keys[idx] = prev;
		_children[idx]->remove(prev);
	}
	else if (_children[idx + 1]->keyCount() >= min_degree) {
		value_type next = getNextKey(idx);
		_keys[idx] = next;
		_children[idx + 1]->remove(next);
	}
	else {
		merge(idx);
		_children[idx]->remove(key);
	}
}

template<class T, class Compare, int min_degree>
inline T my_btree_node<T, Compare, min_degree>::getPreviousKey(int idx)
{
	my_btree_node* curr = _children[idx];
	while (!curr->leaf()) {
		curr = curr->_children[curr->keyCount()];
	}
	return curr->_keys[curr->keyCount() - 1];
}

template<class T, class Compare, int min_degree>
inline T my_btree_node<T, Compare, min_degree>::getNextKey(int idx)
{
	my_btree_node* curr = _children[idx + 1];
	while (!curr->leaf()) {
		curr = curr->_children[0];
	}
	return curr->_keys[0];
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::fill(int idx)
{
	if (idx != 0 && _children[idx - 1]->keyCount() >= min_degree) {
		borrowFromPrev(idx);
	}
	else if (idx != keyCount() && _children[idx + 1]->keyCount() >= min_degree) {
		borrowFromNext(idx);
	}
	else {
		if (idx != keyCount()) {
			merge(idx);
		}
		else {
			merge(idx - 1);
		}
	}
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::borrowFromPrev(int idx)
{
	my_btree_node* child = _children[idx];
	my_btree_node* sibling = _children[idx - 1];

	for (int i = child->keyCount() - 1; i >= 0; --i) {
		child->_keys[i + 1] = child->_keys[i];
	}
	if (!child->leaf()) {
		for (int i = child->keyCount(); i >= 0; --i) {
			child->placeChild(child->_children[i], i + 1);
		}
	}
	child->_keys[0] = _keys[idx - 1];
	if (!child->leaf()) {
		child->placeChild(sibling->_children[sibling->keyCount()], 0);
	}

	_keys[idx-1] = sibling->_keys[sibling->keyCount()-1];
	child->_key_count += 1;
	sibling->_key_count -= 1;
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::borrowFromNext(int idx)
{
	my_btree_node* child = _children[idx];
	my_btree_node* sibling = _children[idx + 1];

	child->_keys[child->keyCount()] = _keys[idx];

	if (!child->leaf()) {
		child->placeChild(sibling->_children[0], child->keyCount() + 1);
	}
	_keys[idx] = sibling->_keys[0];

	for (int i = 1; i < sibling->keyCount(); ++i) {
		sibling->_keys[i - 1] = sibling->_keys[i];
	}

	if (!sibling->leaf()) {
		for (int i = 1; i <= sibling->keyCount(); ++i) {
			sibling->placeChild(sibling->_children[i], i - 1);
		}
	}
	child->_key_count += 1;
	sibling->_key_count -= 1;
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::merge(int idx)
{
	my_btree_node* child = _children[idx];
	my_btree_node* sibling = _children[idx + 1];

	child->_keys[min_degree - 1] = _keys[idx];

	for (int i = 0; i < sibling->keyCount(); ++i) {
		child->_keys[i + min_degree] = sibling->_keys[i];
	}

	if (!child->leaf()) {
		for (int i = 0; i <= sibling->keyCount(); ++i) {
			child->placeChild(sibling->_children[i], i + min_degree);
		}
	}

	for (int i = idx + 1; i < keyCount(); ++i) {
		_keys[i - 1] = _keys[i];
	}

	for (int i = idx + 2; i <= keyCount(); ++i) {
		placeChild(_children[i], i - 1);
	}

	child->_key_count += (sibling->keyCount() + 1);
	_key_count--;

	delete sibling;
}
template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::placeChild(my_btree_node<T, Compare, min_degree>* node, int idx)
{
	node->parent = this;
	_children[idx] = node;
	_children[idx]->my_child_index = idx;
}
;
