#include "pch.h"
#include "../src/my_map.h"

using namespace testing;

TEST(Insert, UniqueValues) {
	my_map<int, int> map;
	size_t size = 50;
	for (int i = 0; i < size; i++) {
		map.insert(i, i);
	}
	EXPECT_EQ(map.size(), size);
}

TEST(Insert, RepeatedValues) {
	my_map<int, int> map;
	size_t size = 50;
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < size; i++) {
			map.insert(i, i);
		}
	}
	EXPECT_EQ(map.size(), size);
}

TEST(Insert, Contains) {
	my_map<int, int> map;
	int constant = 100;
	for (int i = 0; i < 30; i++) {
		map.insert(i, constant + i);
	}
	for (int i = 0; i < 30; i++) {
		EXPECT_TRUE(map.contains(i));
		EXPECT_FALSE(map.contains(constant + i));
	}
}

TEST(Insert, at) {
	my_map<int, int> map;
	int constant = 100;
	for (int i = 0; i < 30; i++) {
		map.insert(i, constant + i);
	}
	for (int i = 0; i < 30; i++) {
		EXPECT_EQ(map.at(i), constant + i);
	}
}