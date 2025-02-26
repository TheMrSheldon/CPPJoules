#ifndef ENERGY_STATE
#define ENERGY_STATE

#include <chrono>
#include <map>
#include <string>

struct EnergyState
{
  std::chrono::steady_clock::time_point timestamp;
  std::map<std::string, unsigned long long> energies;
};

#endif