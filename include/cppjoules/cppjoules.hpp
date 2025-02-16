#ifndef CPPJOULES_CPPJOULES
#define CPPJOULES_CPPJOULES

#include "cppjoules/defines.hpp"

#include <map>
#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <experimental/propagate_const>

namespace cppjoules
{
  namespace detail
  {
    class RAPLDevice;
    class NVMLDevice;
    class PCMDevice;
    class EnergyState;
  }

  enum class TrackerState
  {
    Uninitialized,
    Started,
    Stopped
  };

  enum Capability
  {
    CPU_PROFILE = 1 << 0,
    RAM_PROFILE = 1 << 1,
    GPU_PROFILE = 1 << 2
  };

  struct TrackerResults
  {
    std::chrono::milliseconds time;
    std::map<std::string, long long> energy;
  };

  /**
   * The energy tracker class. Contains the necessary
   * tools for tracking the energy.
   */
  class EXPOSE_DLL EnergyTracker final
  {
  private:
    template <typename T>
    using PImpl = std::experimental::propagate_const<std::unique_ptr<T>>;
    PImpl<detail::RAPLDevice> rapldevice;
    PImpl<detail::NVMLDevice> nvmldevice;
    PImpl<detail::PCMDevice> pcmdevice;
    std::vector<std::unique_ptr<detail::EnergyState>> energy_readings;
    TrackerState state;

  public:
    EnergyTracker();
    ~EnergyTracker();
    void start();
    void stop();
    TrackerResults calculate_energy() const noexcept;

    Capability getCapabilities();
  };
}
#endif