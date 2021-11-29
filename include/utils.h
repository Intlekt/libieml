#pragma once

#include <memory>
#include <functional>

#define STD_HASH_SHARED_PTR(T)                                  \
template<>                                                      \
struct hash<std::shared_ptr<T>> {                               \
    size_t operator()(const std::shared_ptr<T>& e) const {      \
        return std::hash<T>{}(*e);                              \
    }                                                           \
}


#define STD_EQUAL_TO_SHARED_PTR(T)                                                        \
template<>                                                                                   \
struct equal_to<std::shared_ptr<T>> {                                                                          \
    size_t operator()(const std::shared_ptr<T>& l, const std::shared_ptr<T>& r) const {      \
        return std::equal_to<T>{}(*l, *r);                                                   \
    }                                                                                        \
}

#define STD_LESS_SHARED_PTR(T)                                                            \
template<>                                                                                   \
struct less<std::shared_ptr<T>> {                                                                          \
    size_t operator()(const std::shared_ptr<T>& l, const std::shared_ptr<T>& r) const {      \
        return std::less<T>{}(*l, *r);                                                       \
    }                                                                                     \
}

// template<class T> 
// struct shared_ptr_hash {
//     size_t operator()(const std::shared_ptr<T>& e) const {
//         return std::hash<T>{}(*e);
//     }
// };

// template<class T> 
// struct shared_ptr_equal_to {
//     size_t operator()(const std::shared_ptr<T>& l, const std::shared_ptr<T>& r) const {
//         return std::equal_to<T>{}(*l, *r);
//     }
// };

// template<class T> 
// struct shared_ptr_less {
//     size_t operator()(const std::shared_ptr<T>& l, const std::shared_ptr<T>& r) const {
//         return std::less<T>{}(*l, *r);
//     }
// };
