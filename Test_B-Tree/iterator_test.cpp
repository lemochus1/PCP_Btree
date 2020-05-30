#include "pch.h"
#include "../B-Tree/my_btree.h"
#include <iostream>

using namespace testing;
TEST(Iterator, Forward) {
	my_btree<int> tree;
	for (int i = 0; i < 50; i++) {
		tree.insert(i);
	}
	int i = 0;
	for (const auto value : tree) {
		EXPECT_EQ(value, i) << value;
		i++;
	}
}

TEST(Iterator, Reversed) {
	my_btree<int> tree;
	for (int i = 0; i < 21; i++) {
		tree.insert(i);
	}
	int i = 20;
	tree.print();
	for (my_btree<int>::reverse_iterator it = tree.rbegin(); it != tree.rend(); ++it)
	{
		EXPECT_EQ(*it, i) << *it;
		i--;
	}
}