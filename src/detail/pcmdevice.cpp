#include "pcmdevice.hpp"

#include <src/cpucounters.h>

#include <iostream> /** \todo remove **/

using cppjoules::detail::PCMDevice;

PCMDevice::PCMDevice(pcm::PCM &pcm) : pcm(pcm) {
    std::cout << pcm.getCPUBrandString() << std::endl;
    auto state1 = pcm.getSystemCounterState();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto state2 = pcm.getSystemCounterState();
    std::cout << "Energy:        " << pcm::getConsumedEnergy(state1, state2) << std::endl;
    std::cout << "Energy (DRAM): " << pcm::getDRAMConsumedJoules(state1, state2) << std::endl;
}

std::unique_ptr<PCMDevice> PCMDevice::getPCMDevice()
{
    auto &pcm = *pcm::PCM::getInstance();
    if (pcm.good())
        return std::make_unique<PCMDevice>(pcm);
    return nullptr;
}