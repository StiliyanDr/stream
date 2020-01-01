#pragma once

#include <optional>
#include <functional>
#include <type_traits>
#include <utility>

template <class InputIterator>
class Stream
{
    using UnderlyingDerefType =
        decltype(*std::declval<InputIterator>());

public:
    using Item = std::conditional_t<
        std::is_lvalue_reference_v<UnderlyingDerefType>,
        std::reference_wrapper<std::remove_reference_t<UnderlyingDerefType>>,
        std::remove_reference_t<UnderlyingDerefType>
    >;

public:
    Stream(InputIterator begin, InputIterator end);
    Stream(Stream&& source);
    Stream& operator=(Stream&& rhs);

    std::optional<Item> take_one();

    template <class UnaryPredicate>
    std::optional<Item> take_if(UnaryPredicate p);

    template <class UnaryPredicate>
    std::optional<Item> take_unless(UnaryPredicate p);

    void drop_one();

    template <class UnaryPredicate>
    void drop_if(UnaryPredicate p);

    template <class UnaryPredicate>
    void drop_unless(UnaryPredicate p);

    void empty();
    bool is_empty() const;

private:
    Item extract_cached_item();
    void swap_contents_with(Stream& s);

private:
    InputIterator begin;
    InputIterator end;
    std::optional<Item> cached_item;
};

#include "Stream.hpp"
