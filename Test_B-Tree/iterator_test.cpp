#include "pch.h"
#include "../B-Tree/my_btree.h"
#include <iostream>
TEST(Iterator, Forward) {
	my_btree<int> tree;
	for (int i = 0; i < 50; i++) {
		tree.insert(i);
	}
	int i = 0;
	for (const auto value : tree) {
		EXPECT_EQ(value, i);
	}
}