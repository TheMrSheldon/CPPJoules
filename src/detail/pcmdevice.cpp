#include "./energydevice.hpp"

#if CPPJOULES_PCM_ENABLED
#include <src/cpucounters.h>

using cppjoules::detail::Capability;
using cppjoules::detail::EnergyDevice;

class PCMDevice final : public EnergyDevice
{
private:
  pcm::PCM &pcm;
  pcm::SystemCounterState start;

public:
  PCMDevice(pcm::PCM &pcm) : pcm(pcm), start(pcm.getSystemCounterState()) {}

  Capability getCapabilities() const override
  {
    Capability cap = Capability::NONE;
    if (pcm.packageEnergyMetricsAvailable())
      cap = static_cast<Capability>(cap | Capability::CPU_PROFILE);
    if (pcm.dramEnergyMetricsAvailable())
      cap = static_cast<Capability>(cap | Capability::RAM_PROFILE);
    return cap;
  }

  std::map<std::string, unsigned long long> getEnergy() override
  {
    auto now = pcm.getSystemCounterState();
    std::map<std::string, unsigned long long> map;
    if (pcm.packageEnergyMetricsAvailable())
      map["core-0"] = pcm::getConsumedEnergy(start, now);
    if (pcm.dramEnergyMetricsAvailable())
      map["dram-0"] = pcm::getDRAMConsumedJoules(start, now);
    return map;
  }
};

std::unique_ptr<EnergyDevice> cppjoules::detail::createPCMDevice()
{
  auto &pcm = *pcm::PCM::getInstance();
  if (pcm.good())
    return std::make_unique<PCMDevice>(pcm);
  return nullptr;
}
#else
using cppjoules::detail::EnergyDevice;

std::unique_ptr<EnergyDevice> cppjoules::detail::createPCMDevice()
{
  return nullptr;
}
#endif