#ifndef ENERGY_STATE
#define ENERGY_STATE

#include <chrono>
#include <map>
#include <string>

struct EnergyState
{
  std::chrono::high_resolution_clock::time_point timestamp;
  std::map<std::string, unsigned long long> energies;
};

#endif