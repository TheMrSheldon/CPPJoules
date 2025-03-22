#ifndef CPPJOULES_DETAIL_ENERGYDEVICE_HPP
#define CPPJOULES_DETAIL_ENERGYDEVICE_HPP

#include <map>
#include <string>
#include <memory>

namespace cppjoules::detail
{

  enum Capability
  {
    NONE = 0,
    CPU_PROFILE = 1 << 0,
    RAM_PROFILE = 1 << 1,
    GPU_PROFILE = 1 << 2
  };

  class EnergyDevice
  {
  public:
    virtual Capability getCapabilities() const = 0;
    virtual std::map<std::string, unsigned long long> getEnergy() = 0;
  };

  std::unique_ptr<EnergyDevice> createNVMLDevice();
  std::unique_ptr<EnergyDevice> createPCMDevice();
  std::unique_ptr<EnergyDevice> createRAPLDevice();
};

#endif