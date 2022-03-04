#pragma once

#include <memory>
#include <functional>

#define STD_HASH_SHARED_PTR(T)   namespace std {                \
template<>                                                      \
struct hash<std::shared_ptr<T>> {                               \
    size_t operator()(const std::shared_ptr<T>& e) const {      \
        return std::hash<T>{}(*e);                              \
    }                                                           \
};}


#define STD_EQUAL_TO_SHARED_PTR(T)    namespace std {                                                       \
template<>                                                                                   \
struct equal_to<std::shared_ptr<T>> {                                                                          \
    size_t operator()(const std::shared_ptr<T>& l, const std::shared_ptr<T>& r) const {      \
        return *l == *r;                                                   \
    }                                                                                        \
};}

#define STD_LESS_SHARED_PTR(T)   namespace std {                                             \
template<>                                                                                   \
struct less<std::shared_ptr<T>> {                                                            \
    size_t operator()(const std::shared_ptr<T>& l, const std::shared_ptr<T>& r) const {      \
        return *l < *r;                                                                      \
    }                                                                                        \
};}


//https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
template <class T>
inline void hash_combine(size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

