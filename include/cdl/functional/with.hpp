// with.hpp
// Free Function Implementation: with()
//
// A basic implementation of the maybe monad.

#pragma once

#include <optional>

namespace cdl::functional
{
    // add_optionality_t
    // primary template
    template <typename T>
    struct add_optionality_t { using type = std::optional<T>; };

    // add_optionality_t
    // specialization for optional of T; prevents nesting of optionality
    template <typename T>
    struct add_optionality_t<std::optional<T>> { using type = std::optional<T>; };

    template <typename T>
    using add_optionality = typename add_optionality_t<T>::type;

    // wdl::functional::with()
    //
    // Accept optional value as argument; if value is 
    // present, apply provided function to value and return
    // the result, otherwise return default constructed optional.

    template <typename T, typename Func>
    auto with(
        std::optional<T> const& opt, 
        Func const& fn
        ) -> add_optionality<decltype(fn(*opt))>
    {
        if (*opt)
        {
            return fn(*opt);
        }
        else
        {
            return {};
        }
    }

    // wdl::functional::operator|
    //
    // Alternative syntax for maybe monad implementation.

    template <typename T, typename Func>
    auto operator |(
        std::optional<T> const& opt, 
        Func const& fn
        ) -> add_optionality<decltype(fn(*opt))>
    {
        if (*opt)
        {
            return fn(*opt);
        }
        else
        {
            return {};
        }
    }
}