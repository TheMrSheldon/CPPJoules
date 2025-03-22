#ifndef CPPJOULES_CPPJOULES
#define CPPJOULES_CPPJOULES

#include "defines.hpp"

#include <map>
#include <string>
#include <chrono>
#include <vector>
#include <memory>

#ifndef _MSC_VER
#include <experimental/propagate_const>
#endif

namespace cppjoules
{
  namespace detail
  {
    class RAPLDevice; /** \todo remove **/
    class EnergyDevice;
    struct EnergyState;

#ifndef _MSC_VER
    template <typename T>
    using PImpl = std::experimental::propagate_const<std::unique_ptr<T>>;
#else
    template <typename T>
    using PImpl = std::unique_ptr<T>;
#endif
  }

  enum class TrackerState
  {
    Uninitialized,
    Started,
    Stopped
  };

  enum Capability
  {
    NONE = 0,
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
    detail::RAPLDevice *rapldevice; /** \todo remove **/
    std::vector<detail::PImpl<detail::EnergyDevice>> devices;
    std::vector<std::unique_ptr<detail::EnergyState>> energy_readings;
    TrackerState state;

  public:
    EnergyTracker();
    ~EnergyTracker();
    void start();
    void stop();
    TrackerResults calculate_energy() const noexcept;

    Capability getCapabilities() const;
  };
}
#endif