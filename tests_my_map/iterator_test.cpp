#include "pch.h"
#include "../src/my_map.h"

using namespace testing;

TEST(Iterator, Forward) {
	my_map<int, int> map;
	int constant = 100;
	for (int i = 0; i < 50; i++) {
		map.insert(i, i + constant);
	}
	int i = 0;
	for (const auto& value : map) {
		EXPECT_EQ(value, i + constant);
		i++;
	}
}

TEST(Iterator, Reversed) {
	my_map<int, int> map;
	int constant = 100;
	int size = 21;
	for (int i = 0; i < size; i++) {
		map.insert(i, i + constant);
	}
	int i = size - 1;
	for (my_map<int, int>::reverse_iterator it = map.rbegin(); it != map.rend(); ++it) {
		EXPECT_EQ(*it, i + constant);
		i--;
	}
}

TEST(Iterator, DifferentType) {
	my_map<char, int> map;
	map.insert('a', 0);
	map.insert('b', 1);
	map.insert('c', 2);
	map.insert('d', 3);

	int i = 0;
	for (const auto& value : map) {
		EXPECT_EQ(value, i);
		i++;
	}
}