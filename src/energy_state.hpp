#ifndef ENERGY_STATE
#define ENERGY_STATE

#include <chrono>
#include <map>
#include <string>

struct EnergyState
{
  std::chrono::time_point<std::chrono::system_clock> timestamp;
  std::map<std::string, unsigned long long> energies;
};

#endif