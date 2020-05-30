#pragma once

#define GENERATE_HAS_MEMBER(member)                                               \
                                                                                  \
template <class T, class Enabler = void>                                          \
class has_##member                                                                \
{                                                                                 \
private:                                                                          \
    using Yes = char[1];                                                          \
    using No = char[2];                                                           \
    using P = std::remove_pointer_t<T>;                                           \
    struct Fallback { int member; };                                              \
    struct Derived : P, Fallback { };                                             \
                                                                                  \
    template <class U>                                                            \
    static No& test (decltype(U::member)*);                                       \
    template <class U>                                                            \
    static Yes& test (U*);                                                        \
                                                                                  \
public:                                                                           \
    static bool const result = sizeof(test<Derived>(nullptr)) == sizeof(Yes);     \
};                                                                                \
template <class T>                                                                \
class has_##member<T, std::enable_if_t<std::is_fundamental_v<T>>>                 \
{                                                                                 \
public:                                                                           \
    static bool const result = false;                                             \
};                                                                                \
template<class T>                                                                 \
constexpr bool has_##member##_v = has_##member<T>::result;                        \
