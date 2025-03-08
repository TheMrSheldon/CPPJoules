#ifndef CPPJOULES_DETAIL_PCMDEVICE_HPP
#define CPPJOULES_DETAIL_PCMDEVICE_HPP

#include <memory>

namespace pcm
{
    class PCM;
}

namespace cppjoules::detail
{

    /**
     * @brief 
     * @details
     * Using PCM without root: https://github.com/intel/pcm/?tab=readme-ov-file#executing-pcm-tools-under-non-root-user-on-linux
     */
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