#ifndef CPPJOULES_DETAIL_UTILS_NULLBUFFER_HPP
#define CPPJOULES_DETAIL_UTILS_NULLBUFFER_HPP

#include <iostream>

namespace cppjoules::detail::utils
{
    class NullBuffer final : public std::streambuf
    {
    public:
        int_type overflow(int_type) override { return {}; }
    };
}

#endif