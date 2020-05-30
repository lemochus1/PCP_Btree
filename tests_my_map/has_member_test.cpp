#include "pch.h"
#include <type_traits>
#include "../B-Tree/has_member.h"
#include "../B-Tree/member_type.h"

using namespace testing;

GENERATE_HAS_MEMBER(first)
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