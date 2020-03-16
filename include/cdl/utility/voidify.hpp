// voidify.hpp
//
// Free Function Implementation:
//  - cdl::utility::voidify()

#pragma once

#include <tuple>
#include <memory>

namespace cdl::utility
{
    template <typename... Args, typename Lambda>
    std::pair<void(*)(void*, Args...), std::unique_ptr<void, void(*)(void*)>>
    voidify(Lambda&& l)
    {
        using Func = std::decay_t<Lambda>;

        auto data = std::unique_ptr<void, void(*)(void*)>{
            new Func(std::forward<Lambda>(l)),
            +[](void* ptr){ delete (Func*)ptr; }
        };

        return {
            +[](void* v, Args... args) -> void
            {
                Func* f = static_cast<Func*>(v);
                (*f)(std::forward<Args>(args)...);
            },
            std::move(data)
        };
    }
}