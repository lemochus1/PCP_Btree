#pragma once

#define GENERATE_MEMBER_TYPE(member)                                              \
                                                                                  \
template <class T, class Enabler = void>                                          \
class member##_type                                                               \
{                                                                                 \
public:                                                                           \
    using type = decltype(std::declval<T>().##member);                            \
};                                                                                \
template <class T>                                                                \
class member##_type<T, std::enable_if_t<std::is_fundamental_v<T>>>                \
{                                                                                 \
public:                                                                           \
    using type = void;                                                            \
};                                                                                \
template <class T>                                                                \
class member##_type<T, std::enable_if_t<std::is_same_v<T, std::string>>>          \
{                                                                                 \
public:                                                                           \
    using type = void;                                                            \
};                                                                                \
template<class T>                                                                 \
using member##_t = typename member##_type<T>::type;                               \
