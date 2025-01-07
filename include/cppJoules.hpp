#ifndef CPP_JOULES
#define CPP_JOULES

#include "cppjoules/defines.hpp"

#include <map>
#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <experimental/propagate_const>

/**
 * Forward Declarations
 */

class RAPLDevice;
class NVMLDevice;
class EnergyState;

enum class TrackerState
{
  Uninitialized,
  Started,
  Stopped
};

struct TrackerResults
{
  std::chrono::milliseconds time;
  std::map<std::string, long long> energy;
};

class EXPOSE_DLL EnergyTracker final
{
private:
  /**
   * The energy tracker class. Contains the necessary
   * tools for tracking the energy.
   */
  std::experimental::propagate_const<std::unique_ptr<RAPLDevice>> rapldevice;
  std::experimental::propagate_const<std::unique_ptr<NVMLDevice>> nvmldevice;
  std::vector<std::unique_ptr<EnergyState>> energy_readings;
  TrackerState state;

public:
  EnergyTracker();
  void start();
  void stop();
  TrackerResults calculate_energy() const noexcept;
};
#endif