#include "pch.h"
#include "../B-Tree/my_btree.h"

using namespace testing;
TEST(Remove, UniqueValues) {
	my_btree<int> tree;
	int size = 30;
	for (int i = 0; i < size; i++) {
		tree.insert(i);
	}
	for (int i = 0; i < size; i++) {
		tree.remove(i);
		EXPECT_FALSE(tree.contains(i));
		tree.insert(i);
	}
}

TEST(Remove, RepeatedValues) {
	my_btree<int> tree;
	int repeatCount = 9;
	for (int j = 0; j < repeatCount; j++) {
		for (int i = 0; i < 12; i++) {
			tree.insert(i);
		}
	}
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < repeatCount; j++) {
			tree.remove(i);
			EXPECT_EQ(repeatCount - j - 1, tree.count(i));
		}
	}
}

TEST(Remove, Clear) {
	int size = 23;
	my_btree<int> tree;
	for (int i = 0; i < size; i++) {
		tree.insert(i);
	}
	EXPECT_EQ(tree.size(), size);
	tree.clear();
	EXPECT_EQ(tree.size(), 0);
}

TEST(Remove, Empty) {
	my_btree<int> tree;
	int size = 18;
	for (int i = 0; i < size; i++) {
		tree.insert(i);
	}
	for (int i = 0; i < size; i++) {
		tree.remove(i);
	}
	EXPECT_TRUE(tree.empty());
}
