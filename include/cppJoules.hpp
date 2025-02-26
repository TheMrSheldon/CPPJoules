#ifndef CPP_JOULES
#define CPP_JOULES

#include "cppjoules/defines.hpp"

#include <map>
#include <string>
#include <chrono>
#include <vector>
#include <memory>

#ifndef _MSC_VER
#include <experimental/propagate_const>
#endif

/**
 * Forward Declarations
 */

class RAPLDevice;
class NVMLDevice;
struct EnergyState;

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
#ifdef _MSC_VER
  std::unique_ptr<RAPLDevice> rapldevice;
  std::unique_ptr<NVMLDevice> nvmldevice;
#else
  std::experimental::propagate_const<std::unique_ptr<RAPLDevice>> rapldevice;
  std::experimental::propagate_const<std::unique_ptr<NVMLDevice>> nvmldevice;
#endif
  std::vector<std::unique_ptr<EnergyState>> energy_readings;
  TrackerState state;

public:
  EnergyTracker();
  ~EnergyTracker();
  void start();
  void stop();
  TrackerResults calculate_energy() const noexcept;
};
#endif