#include <cppjoules/cppjoules.hpp>

#include "rapl_devices.hpp"
#include "nvidia_devices.hpp"
#include "energy_state.hpp"
#include "cppjoules/detail/pcmdevice.hpp"

#include <cassert>
#include <fstream>

using cppjoules::EnergyTracker;
using cppjoules::TrackerResults;
using cppjoules::detail::EnergyState;
using cppjoules::detail::NVMLDevice;
using cppjoules::detail::PCMDevice;
using cppjoules::detail::RAPLDevice;

EnergyTracker::EnergyTracker() : rapldevice(std::make_unique<RAPLDevice>()), nvmldevice(std::make_unique<NVMLDevice>()), pcmdevice(PCMDevice::getPCMDevice()), state(TrackerState::Uninitialized)
{
}

EnergyTracker::~EnergyTracker() = default;

void EnergyTracker::start()
{
  /**
   * Getting both the RAPL and NVML Energies
   */
  assert(state != TrackerState::Started);
  auto start_time = std::chrono::high_resolution_clock::now();
  std::map<std::string, unsigned long long> start_energy;
  auto rapl_energy = rapldevice->getEnergy();
  start_energy.insert(rapl_energy.begin(), rapl_energy.end());

  std::map<std::string, unsigned long long> nvidia_energy;
  /**
   * Check if nvml can be used
   */
  if (nvmldevice->usable)
  {
    nvidia_energy = nvmldevice->getEnergy();
    start_energy.insert(nvidia_energy.begin(), nvidia_energy.end());
  }
  energy_readings.emplace_back(std::make_unique<EnergyState>(EnergyState{/*.timestamp =*/ start_time, /*.energies =*/ start_energy}));
  state = TrackerState::Started;
}

void EnergyTracker::stop()
{
  assert(state == TrackerState::Started);
  if (state != TrackerState::Started)
    return;
  auto end_time = std::chrono::high_resolution_clock::now();
  auto stop_energy = rapldevice->getEnergy();
  std::map<std::string, unsigned long long> nvidia_energy;

  if (nvmldevice->usable)
  {
    nvidia_energy = nvmldevice->getEnergy();
    stop_energy.insert(nvidia_energy.begin(), nvidia_energy.end());
  }
  energy_readings.emplace_back(std::make_unique<EnergyState>(EnergyState{/*.timestamp =*/ end_time, /*.energies =*/ stop_energy}));
  state = TrackerState::Stopped;
}

TrackerResults EnergyTracker::calculate_energy() const noexcept
{
  assert(state == TrackerState::Stopped);
  if (state != TrackerState::Stopped)
    return {};

  TrackerResults results{/*.time =*/ std::chrono::milliseconds{0}, /*.energy =*/ {}};

  assert(energy_readings.size() % 2 == 0);
  for (auto i = 0u; i < energy_readings.size() - 1; i += 2)
  {
    auto &start = *energy_readings[i];
    auto &stop = *energy_readings[i + 1];

    assert(start.energies.size() == stop.energies.size());

    results.time += std::chrono::duration_cast<std::chrono::milliseconds>(stop.timestamp - start.timestamp);
    for (const auto &[first, second] : start.energies)
    {
      /**
       * If the energy counter had reset, we have to add the max energy
       */
      if (stop.energies[first] < second)
      {
#ifdef __linux__
        if (rapldevice->max_energy_devices.count(first))
        {
          std::ifstream stream(rapldevice->max_energy_devices.at(first));
          if (std::string line; stream && getline(stream, line))
          {
            long long energy = std::stoll(line);
            results.energy[first] += stop.energies[first] - second + energy;
          }
          else
          {
            /** \todo handle error **/
          }
        }
#endif
      }
      else
      {
        results.energy[first] += (stop.energies[first] - second);
      }
    }
  }
  return results;
}