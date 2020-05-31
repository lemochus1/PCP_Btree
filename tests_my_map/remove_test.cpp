#include "pch.h"
#include "../src/my_map.h"

using namespace testing;

TEST(Remove, Key) {
	my_map<int, int> map;
	int size = 30;
	for (int i = 0; i < size; i++) {
		map.insert(i, i);
	}
	for (int i = 0; i < size; i++) {
		map.erase(i);
		EXPECT_FALSE(map.contains(i));
		map.insert(i, i);
	}
}

TEST(Remove, Clear) {
	my_map<int, int> map;
	int size = 23;
	for (int i = 0; i < size; i++) {
		map.insert(i, i);
	}
	EXPECT_EQ(map.size(), size);
	map.clear();
	EXPECT_EQ(map.size(), 0);
}