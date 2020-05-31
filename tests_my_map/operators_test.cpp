#include "pch.h"
#include "../src/my_map.h"

using namespace testing;

#include "pch.h"
#include "../src/my_map.h"

using namespace testing;

TEST(Operator, Access) {
	my_map<int, char> map;
	map.insert(0, 'a');
	map.insert(1, 'b');
	map.insert(2, 'c');
	map.insert(3, 'd');
	EXPECT_EQ(map[1], 'b');
	EXPECT_EQ(map[2], 'c');
	EXPECT_EQ(map[3], 'd');
}

TEST(Operator, Insert) {
	my_map<int, char> map;
	map.insert(0, 'a');
	map.insert(1, 'b');
	map.insert(2, 'c');
	map.insert(3, 'd');
	map[4] = 'e';
	map[5] = 'f';
	map[6] = 'g';
	EXPECT_EQ(map.at(1), 'b');
	EXPECT_EQ(map.at(4), 'e');
	map[1] = 'h';
	EXPECT_EQ(map.at(1), 'h');
}