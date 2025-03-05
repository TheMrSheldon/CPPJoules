#ifndef ENERGYSTATE_HPP
#define ENERGYSTATE_HPP

#include <chrono>
#include <map>
#include <string>

namespace cppjoules::detail
{
  struct EnergyState
  {
    std::chrono::high_resolution_clock::time_point timestamp;
    std::map<std::string, unsigned long long> energies;
  };
}

#endif