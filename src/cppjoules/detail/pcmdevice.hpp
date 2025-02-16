#ifndef CPPJOULES_DETAIL_PCMDEVICE_HPP
#define CPPJOULES_DETAIL_PCMDEVICE_HPP

#include <memory>

namespace pcm
{
    class PCM;
}

namespace cppjoules::detail
{

    class PCMDevice final
    {
    private:
        pcm::PCM &pcm;

    public:
        PCMDevice(pcm::PCM &pcm);

        static std::unique_ptr<PCMDevice> getPCMDevice();
    };

};

#endif