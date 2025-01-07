#ifndef CPPJOULES_IOUTILS_HPP
#define CPPJOULES_IOUTILS_HPP

#include "defines.hpp"

#include <filesystem>

class EnergyTracker;

namespace cppjoules::io
{
    EXPOSE_DLL void dumpToConsole(const EnergyTracker &tracker) noexcept;
    EXPOSE_DLL void dumpToCSV(const EnergyTracker &tracker, const std::filesystem::path &file) noexcept;
}

#endif