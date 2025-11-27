#ifndef CPPJOULES_DETAIL_UTILS_RDBUFGUARD_HPP
#define CPPJOULES_DETAIL_UTILS_RDBUFGUARD_HPP

#include <iostream>

namespace cppjoules::detail::utils
{
    class RdbufGuard final
    {
    private:
        std::ostream &stream;
        std::streambuf *oldbuf;

        RdbufGuard() = delete;
        RdbufGuard(const RdbufGuard &other) = delete;
        RdbufGuard(RdbufGuard &&other) = delete;
        RdbufGuard &operator=(const RdbufGuard &other) = delete;
        RdbufGuard &operator=(RdbufGuard &&other) = delete;

    public:
        RdbufGuard(std::ostream &stream, std::streambuf *buf) : stream(stream), oldbuf(stream.rdbuf(buf))
        {
        }
        ~RdbufGuard()
        {
            stream.rdbuf(oldbuf);
        }
    };
}

#endif