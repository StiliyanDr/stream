#pragma once

#include <type_traits>
#include <utility>

namespace utils
{
    template <class UnaryPredicate>
    auto negation_of(UnaryPredicate p)
    {
        return [p](auto && x)
        {
            return !p(std::forward<decltype(x)>(x));
        };
    }

    template <class T>
    using ConditionalReference = std::conditional_t<
        !std::is_nothrow_move_assignable_v<T> && std::is_copy_assignable_v<T>,
        const T&,
        T&&
    >;

    template <class T>
    inline constexpr ConditionalReference<T>
    move_assign_if_noexcept(T& x) noexcept
    {
        return static_cast<ConditionalReference<T>>(x);
    }

}
