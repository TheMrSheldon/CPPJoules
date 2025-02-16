#include "pcmdevice.hpp"

#include <src/cpucounters.h>

#include <iostream> /** \todo remove **/

using cppjoules::detail::PCMDevice;

PCMDevice::PCMDevice(pcm::PCM &pcm) : pcm(pcm) {
    std::cout << pcm.getCPUBrandString() << std::endl;
    auto state1 = pcm.getSystemCounterState();
    auto state2 = pcm.getSystemCounterState();
    pcm::getConsumedEnergy(state1, state2);
    pcm::getDRAMConsumedJoules(state1, state2);
}

std::unique_ptr<PCMDevice> PCMDevice::getPCMDevice()
{
    auto &pcm = *pcm::PCM::getInstance();
    if (pcm.good())
        return std::make_unique<PCMDevice>(pcm);
    return nullptr;
}