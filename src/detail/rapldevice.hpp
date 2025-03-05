#ifndef RAPL_DEVICES
#define RAPL_DEVICES

#include <string>
#include <unordered_map>
#include <map>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

namespace cppjoules::detail
{
  class RAPLDevice final
  {
#ifdef _WIN64
    bool initialized;
    HINSTANCE handler;
    using RAPLDeviceHandle = std::unordered_map<std::string, std::pair<int, int>>;
#elif __linux__
    static const std::filesystem::path RaplBasePath;
    using RAPLDeviceHandle = std::unordered_map<std::string, std::string>;
#elif __APPLE__
    void *handler;
    using RAPLDeviceHandle = std::unordered_map<std::string, std::pair<int, int>>;
#endif
    /**
     * Helper function to get the name of the domain
     * given the path of the domain
     */
    static std::string getName(const std::filesystem::path &path) noexcept;

  public:
    RAPLDeviceHandle devices;
    std::unordered_map<std::string, std::string> max_energy_devices;
    /**
     * Function to get the energy counter values from Powercap in linux
     * For windows, uses the functions exposed by the energy dll
     */
    std::map<std::string, unsigned long long> getEnergy();

    /**
     * Initialization of RAPL using the powercap interface
     * All the possible domains are searched for and
     * the domains that are present are added for measurement
     * Requires read access to all energy_uj files of accessible
     * domains
     */
    RAPLDevice();
  };
};
#endif