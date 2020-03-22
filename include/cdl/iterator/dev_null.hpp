// dev_null.hpp
//
// Class Implementation: cdl::iterator::dev_null_iterator

#pragma once

#include <iterator>

namespace cdl::iterator
{
    class dev_null_iterator
    {
    public:
        using iterator_category = std::output_iterator_tag;

        dev_null_iterator& operator++() { return *this; }
        dev_null_iterator& operator*() { return *this; }

        template <typename T>
        dev_null_iterator& operator=(T const&)
        {
            return *this;
        } 
    };
}