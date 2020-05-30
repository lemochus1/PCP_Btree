#include "pch.h"
#include "../src/member_type.h"

using namespace testing;

GENERATE_MEMBER_TYPE(first)

struct A {
	int first;
};
struct B {
	int not_first;
};
struct C {
	A* first;
};
struct D {
	A* not_first;
};

TEST(MemberType, FundamentalTypes) {
	bool is_same = std::is_same_v<first_t<int>, void>;
	EXPECT_TRUE(is_same);
	is_same = std::is_same_v<first_t<std::string>, void>;
	EXPECT_TRUE(is_same);
}

TEST(MemberType, CustomClasses) {
	bool is_same = std::is_same_v<first_t<A>, int>;
	EXPECT_TRUE(is_same);
	is_same = std::is_same_v<first_t<C>, A*>;
	EXPECT_TRUE(is_same);
}