#pragma once
#include <iostream>

template <class T, class Compare, int min_degree>
class my_btree_node {
	template<class, class, int>
	friend class my_btree;

public:
	using value_type = T;

	my_btree_node(bool leaf);
	~my_btree_node();

	bool leaf() const noexcept;
	
	bool full() const noexcept;
	bool sufficientKeyCount() const noexcept;

	size_t keyCount() const noexcept;
	size_t childCount() const noexcept;

	size_t count(const T& key) const;

	void clear() noexcept;

	void print(int deepness) const noexcept;
	my_btree_node* search(const T& key) const noexcept;

	void insert(const T& key);
	void remove(const T& key);


private:
	int findKey(const T& key) const;

	void splitChild(int child_index, my_btree_node* node);

	void removeFromLeaf(int idx);
	void removeFromNonLeaf(int idx);

	T getPred(int idx);
	T getSucc(int idx);

	void fill(int idx);
	void borrowFromPrev(int idx);
	void borrowFromNext(int idx);
	void merge(int idx);

	int _leaf;
	int _key_count;

public: //zatim...

	my_btree_node<T, Compare, min_degree>* _children[2 * min_degree];
	my_btree_node<T, Compare, min_degree>* parent;
	size_t my_child_index;
	value_type _keys[2* min_degree - 1];

	Compare _compare;
};

template<class T, class Compare, int min_degree>
inline my_btree_node <T, Compare, min_degree> ::my_btree_node(bool leaf)
	: _leaf(leaf),
	_key_count(0),
	my_child_index(0)
{
	parent = nullptr;
}

template<class T, class Compare, int min_degree>
inline my_btree_node<T, Compare, min_degree>::~my_btree_node()
{

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
inline size_t my_btree_node<T, Compare, min_degree>::count(const T& key) const
{
	size_t total = 0;
	int idx = findKey(key);
	if (_keys[idx]  == key) {
		for (int i = idx; i < keyCount(); i++) {
			if (_keys[i] == key) {
				total++;
				if (i + 1 < childCount()) {
					total += _children[i + 1]->count(key);
				}
			}
		}
	}
	if (idx < childCount()) {
		total += _children[idx]->count(key);
	}
	return total;
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
	std::string indentation(deepness * 2, ' ');

	if (leaf()) {
		std::cout << indentation;
		for (int i = 0; i < keyCount(); i++) {
			std::cout << " " << _keys[i];
		}
		std::cout << std::endl;
	}
	else {
		for (int i = 0; i < keyCount(); i++) {
			_children[i]->print(deepness + 1);
			std::cout << indentation << _keys[i] << std::endl;
		}
		_children[keyCount()]->print(deepness + 1);
	}
}

template<class T, class Compare, int min_degree>
inline my_btree_node<T, Compare, min_degree>* my_btree_node<T, Compare, min_degree>::search(const T& key) const noexcept
{
	int i = 0;
	while (i < keyCount() && _compare(key, _keys[i])) {
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
inline void my_btree_node<T, Compare, min_degree>::insert(const T& key)
{
	int i = keyCount() - 1;

	if (leaf()) {
		while (i >= 0 && _compare(key, _keys[i])) {
			_keys[i + 1] = _keys[i];
			i--;
		}
		_keys[i + 1] = key;
		_key_count++;
	}
	else {
		while (i >= 0 && _compare(key, _keys[i])) {
			i--;
		}
		if (_children[i + 1]->full()) {
			splitChild(i + 1, _children[i + 1]);
			if (_compare(_keys[i + 1], key)) {
				i++;
			}
		}
		_children[i + 1]->insert(key);
	}
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::splitChild(int i, my_btree_node* node)
{
	my_btree_node<T, Compare, min_degree>* new_node = new my_btree_node<T, Compare, min_degree> (node->leaf());
	new_node->_key_count = min_degree - 1;

	for (int j = 0; j < min_degree -1; j++) {
		new_node->_keys[j] = node->_keys[j + min_degree];
	}

	if (!node->leaf()) {
		for (int j = 0; j < min_degree; j++) {
			new_node->_children[j] = node->_children[j + min_degree];
			new_node->_children[j]->parent = new_node;
			new_node->_children[j]->my_child_index = j;
		}
	}
	node->_key_count = min_degree - 1;

	for (int j = keyCount(); j >= i+1; j--) {
		_children[j + 1] = _children[j];
		_children[j + 1]->my_child_index = j + 1;
	}

	_children[i + 1] = new_node;
	_children[i + 1]->my_child_index = i + 1;
	_children[i + 1]->parent = this;
	
	for (int j = keyCount() - 1; j >= i; j--) {
		_keys[j + 1] = _keys[j];
	}
	_keys[i] = node->_keys[min_degree - 1];
	_key_count++;
}

template<class T, class Compare, int min_degree>
inline int my_btree_node<T, Compare, min_degree>::findKey(const T& key) const
{
	int idx = 0;
	while (idx < keyCount() && _compare(_keys[idx], key)) {
		idx++;
	}
	return idx;
}

template<class T, class Compare, int min_degree>
inline void my_btree_node<T, Compare, min_degree>::remove(const T& key)
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
	T key = _keys[idx];
	if (_children[idx]->keyCount() >= min_degree)
	{
		T pred = getPred(idx);
		_keys[idx] = pred;
		_children[idx]->remove(pred);
	}
	else if (_children[idx + 1]->keyCount() >= min_degree) {
		T pred = getSucc(idx);
		_keys[idx] = pred;
		_children[idx+1]->remove(pred);
	}
	else {
		merge(idx);
		_children[idx]->remove(key);
	}
}

template<class T, class Compare, int min_degree>
inline T my_btree_node<T, Compare, min_degree>::getPred(int idx)
{
	my_btree_node* curr = _children[idx];
	while (!curr->leaf()) {
		curr = curr->_children[curr->keyCount()];
	}
	return curr->_keys[curr->keyCount() - 1];
}

template<class T, class Compare, int min_degree>
inline T my_btree_node<T, Compare, min_degree>::getSucc(int idx)
{
	my_btree_node* curr = _children[idx+1];
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
			child->_children[i + 1] = child->_children[i];
		}
	}
	child->_keys[0] = _keys[idx - 1];
	if (!child->leaf()) {
		child->_children[0] = sibling->_children[sibling->keyCount()];
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
		child->_children[child->keyCount() + 1] = sibling->_children[0];
	}
	_keys[idx] = sibling->_keys[0];

	for (int i = 1; i < sibling->keyCount(); ++i) {
		sibling->_keys[i - 1] = sibling->_keys[i];
	}

	if (!sibling->leaf()) {
		for (int i = 1; i <= sibling->keyCount(); ++i) {
			sibling->_children[i - 1] = sibling->_children[i];
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

	if (!child->leaf())
	{
		for (int i = 0; i <= sibling->keyCount(); ++i) {
			child->_children[i + min_degree] = sibling->_children[i];
			child->_children[i + min_degree]->my_child_index = i + min_degree;
			child->_children[i + min_degree]->parent = child;
		}
	}

	for (int i = idx + 1; i < keyCount(); ++i) {
		_keys[i - 1] = _keys[i];
	}

	for (int i = idx + 2; i <= keyCount(); ++i) {
		_children[i - 1] = _children[i];
		_children[i - 1]->my_child_index = i - 1;
	}

	child->_key_count += (sibling->keyCount() + 1);
	_key_count--;

	delete sibling;
}
;
