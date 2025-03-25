#include <cppjoules/cppjoules.hpp>

#include "energy_state.hpp"
#include "detail/rapldevice.hpp"
#include "detail/energydevice.hpp"

#include <cassert>
#include <fstream>

using cppjoules::EnergyTracker;
using cppjoules::TrackerResults;
using cppjoules::detail::EnergyState;
using cppjoules::detail::RAPLDevice;

EnergyTracker::EnergyTracker() : devices{}, state(TrackerState::Uninitialized)
{
  auto nvml = cppjoules::detail::createNVMLDevice();
  auto pcm = cppjoules::detail::createPCMDevice();
  auto rapl = cppjoules::detail::createRAPLDevice();
  rapldevice = static_cast<RAPLDevice *>(rapl.get()); /** \todo remove **/
  if (nvml != nullptr)
    devices.emplace_back(std::move(nvml));
  if (pcm != nullptr)
    devices.emplace_back(std::move(pcm));
  if (rapl != nullptr)
    devices.emplace_back(std::move(rapl));
}

EnergyTracker::EnergyTracker(EnergyTracker&&) = default;

EnergyTracker& EnergyTracker::operator=(EnergyTracker&&) = default;

EnergyTracker::~EnergyTracker() = default;

void EnergyTracker::start()
{
  assert(state != TrackerState::Started);
  auto start_time = std::chrono::high_resolution_clock::now();
  std::map<std::string, unsigned long long> start_energy;
  for (auto &device : devices)
  {
    auto energy = device->getEnergy();
    start_energy.insert(energy.begin(), energy.end());
  }
  energy_readings.emplace_back(std::make_unique<EnergyState>(EnergyState{/*.timestamp =*/start_time, /*.energies =*/start_energy}));
  state = TrackerState::Started;
}

void EnergyTracker::stop()
{
  assert(state == TrackerState::Started);
  if (state != TrackerState::Started)
    return;
  auto end_time = std::chrono::high_resolution_clock::now();
  std::map<std::string, unsigned long long> stopenergy;
  for (auto &device : devices)
  {
    auto energy = device->getEnergy();
    stopenergy.insert(energy.begin(), energy.end());
  }
  energy_readings.emplace_back(std::make_unique<EnergyState>(EnergyState{/*.timestamp =*/end_time, /*.energies =*/stopenergy}));
  state = TrackerState::Stopped;
}

TrackerResults EnergyTracker::calculate_energy() const noexcept
{
  assert(state == TrackerState::Stopped);
  if (state != TrackerState::Stopped)
    return {};

  TrackerResults results{/*.time =*/std::chrono::milliseconds{0}, /*.energy =*/{}};

  assert(energy_readings.size() % 2 == 0);
  for (auto i = 0u; i < energy_readings.size() - 1; i += 2)
  {
    auto &start = *(energy_readings[i]);
    auto &stop = *(energy_readings[i + 1]);

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
        if (rapldevice->max_energy_devices.count(first)) /** \todo move this code into rapl device **/
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

#include <numeric>

using cppjoules::Capability;
using cppjoules::detail::EnergyDevice;
using cppjoules::detail::PImpl;

static Capability aggCapability(Capability cap, const PImpl<EnergyDevice> &device)
{
  return static_cast<Capability>(cap | static_cast<Capability>(device->getCapabilities()));
}

Capability EnergyTracker::getCapabilities() const
{
  return std::accumulate(devices.begin(), devices.end(), Capability::NONE, aggCapability);
}