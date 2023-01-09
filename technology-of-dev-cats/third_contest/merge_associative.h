#pragma once

#include <map>
#include <unordered_map>
#include <iostream>
#include <set>
#include <unordered_set>

template <typename T>
struct is_set<std::set<T>> {
    static constexpr bool value = true;
};

template <typename T>
struct is_set {
    static constexpr bool value = false;
};

template <typename T>
struct is_multiset<std::multiset<T>> {
    static constexpr bool value = true;
};

template <typename T>
struct is_multiset {
    static constexpr bool value = false;
};

template <typename T>
struct is_unordered_set<std::unordered_set<T>> {
    static constexpr bool value = true;
};

template <typename T>
struct is_unordered_set {
    static constexpr bool value = false;
};

template <typename T>
struct is_unordered_multiset<std::unordered_multiset<T>> {
    static constexpr bool value = true;
};

template <typename T, typename A>
struct is_multimap<std::multimap<T, A>> {
    static constexpr bool value = true;
};

template <typename T, typename A>
struct is_map<std::map<T, A>> {
    static constexpr bool value = true;
};

template <typename T>
struct is_map {
    static constexpr bool value = false;
};


template <typename T>
struct is_unordered_multiset {
    static constexpr bool value = false;
};

template <typename T>
struct is_multimap {
    static constexpr bool value = false;
};

template <typename T, typename A>
struct is_unordered_map<std::unordered_map<T, A>> {
    static constexpr bool value = true;
};

template <typename T>
struct is_unordered_map {
    static constexpr bool value = false;
};

template <typename T>
struct is_unordered_multimap {
    static constexpr bool value = false;
};

template <typename T, typename A>
struct is_unordered_multimap<std::unordered_multimap<T, A>> {
    static constexpr bool value = true;
};

template <typename T, typename A>
struct are_associative_containers {
    static constexpr bool value =
            (is_set<T>::value
            || is_unordered_set<T>::value
            || is_map<T>::value
            || is_unordered_map<T>::value
            || is_multiset<T>::value
            || is_unordered_multiset<T>::value
            || is_multimap<T>::value
            ||
             is_unordered_multimap<T>::value) &&
            (is_set<A>::value
            || is_unordered_set<A>::value
            || is_map<A>::value
            || is_unordered_map<A>::value
            || is_multiset<A>::value
            || is_unordered_multiset<A>::value || is_multimap<A>::value ||
             is_unordered_multimap<A>::value);
};

template <typename A>
struct is_some_map {
    static constexpr bool value =
            is_map<A>::value
            || is_unordered_map<A>::value
            || is_multimap<A>::value ||
            is_unordered_multimap<A>::value;
};

template <typename A>
struct is_multi {
    static constexpr bool value =
            is_multiset<A>::value || is_unordered_multiset<A>::value ||
            is_multimap<A>::value || is_unordered_multimap<A>::value;
};

template <typename A>
struct is_some_set {
    static constexpr bool value =
            is_set<A>::value
            || is_unordered_set<A>::value
            || is_multiset<A>::value ||
            is_unordered_multiset<A>::value;
};

template <class, typename = void>
struct map_attribute : std::false_type {};

template <class Map>
struct map_attribute<Map, std::void_t<typename Map::mapped_type>>
        : std::true_type {};

template <template <typename...> class Map1, typename A, typename B,
        template <typename...> class Map2, typename C, typename D,
        std::enable_if_t<map_attribute<Map1<A, B>>::value, int> = 0>
bool compare(Map1<A, B> &&, const Map2<C, D> &&) {
    return std::is_same<std::remove_cv_t<A>, std::remove_cv_t<C>>::value &&
           std::is_same<std::remove_cv_t<B>, std::remove_cv_t<D>>::value;
}

template <template <typename...> class Set1, typename A,
        template <typename...> class Set2, typename B,
        std::enable_if_t<!map_attribute<Set1<A>>::value, int> = 0>
bool compare(Set1<A> &&, const Set2<B> &&) {
    return std::is_same<std::remove_cv_t<A>, std::remove_cv_t<B>>::value;
}

template <class C1, class C2>
bool MergeAssociative(C1 *c1, const C2 &c2) {
    if constexpr ((are_associative_containers<C1, C2>::value) &&
                  ((is_some_map<C1>::value && is_some_map<C2>::value) ||
                   (is_some_set<C1>::value && is_some_set<C2>::value))) {
        if ((is_multi<C1>::value) ||
            ((!is_multi<C1>::value) && (!is_multi<C2>::value))) {
            if (compare(std::remove_reference_t<C1>(*c1),
                        std::remove_reference_t<C2>(c2))) {
                for (auto el : c2) {
                    c1->insert(el);
                }
                return false;
            }
        }
    }
    return true;
}
