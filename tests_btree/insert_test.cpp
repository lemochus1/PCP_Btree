#include "pch.h"
#include "../src/my_btree.h"

using namespace testing;
TEST(Insert, UniqueValues) {
	my_btree<int> tree;
	for (int i = 0; i < 50; i++) {
		tree.insert(i);
	}
	EXPECT_TRUE(true);
}

TEST(Insert, RepeatedValues) {
	my_btree<int> tree;
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < 50; i++) {
			tree.insert(i);
		}
	}
	EXPECT_TRUE(true);
}

TEST(Insert, Print) {
	my_btree<int> tree;
	for (int i = 0; i < 50; i++) {
		tree.insert(i);
	}
	tree.print();
	EXPECT_TRUE(true);
}

TEST(Insert, Size) {
	int size = 40;
	my_btree<int> tree;
	for (int i = 0; i < 40; i++) {
		tree.insert(i);
	}
	EXPECT_EQ(tree.size(), size);
	for (int i = 0; i < size; i++) {
		tree.insert(0);
	}
	EXPECT_EQ(tree.size(), 2 * size);
}

TEST(Insert, Contains) {
	my_btree<int> tree;
	for (int i = 0; i < 30; i++) {
		tree.insert(i);
	}
	for (int i = 0; i < 30; i++) {
		EXPECT_TRUE(tree.contains(i));
	}
}

TEST(Insert, Count) {
	my_btree<int> tree;
	for (int i = 0; i < 10; i++) {
		tree.insert(i);
	}
	for (int i = 0; i < 10; i++) {
		EXPECT_EQ(tree.count(i), 1);
	}
	int value = 5;
	int repeat = 10;
	for (int i = 0; i < repeat; i++) {
		tree.insert(value);
	}
	EXPECT_EQ(tree.count(value), repeat + 1);
}
