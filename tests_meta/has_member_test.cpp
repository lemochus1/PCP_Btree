#include "pch.h"

#include "../src/has_member.h"

using namespace testing;

GENERATE_HAS_MEMBER(first)

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

TEST(HasMember, StaticResult) {
	EXPECT_TRUE(has_first<A>::result);
	EXPECT_FALSE(has_first<B>::result);
	EXPECT_TRUE(has_first<C>::result);
	EXPECT_FALSE(has_first<D>::result);
}

TEST(HasMember, Using_v) {
	EXPECT_TRUE(has_first_v<A>);
	EXPECT_FALSE(has_first_v<B>);
	EXPECT_TRUE(has_first_v<C>);
	EXPECT_FALSE(has_first_v<D>);
}

TEST(HasMember, Pointer) {
	EXPECT_TRUE(has_first_v<A*>);
	EXPECT_FALSE(has_first_v<B*>);
	EXPECT_TRUE(has_first_v<C*>);
	EXPECT_FALSE(has_first_v<D*>);
}

TEST(HasMember, FundamentalTypes) {
	EXPECT_FALSE(has_first_v<int>);
	EXPECT_FALSE(has_first<int>::result);
	EXPECT_FALSE(has_first_v<std::string>);
	EXPECT_FALSE(has_first<std::string>::result);
}