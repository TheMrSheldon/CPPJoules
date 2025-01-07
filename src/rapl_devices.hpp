#ifndef RAPL_DEVICES
#define RAPL_DEVICES

#include <string>
#include <unordered_map>
#include <map>

class RAPLDevice final
{
#ifdef _WIN64
  bool initialized;
  HINSTANCE handler;
  using RAPLDeviceHandle = std::unordered_map<std::string, std::pair<int, int>>;
#elif __linux__
  std::string RAPL_API_PATH = "/sys/class/powercap/intel-rapl/";
  using RAPLDeviceHandle = std::unordered_map<std::string, std::string>;
#elif __APPLE__
  void *handler;
  using RAPLDeviceHandle = std::unordered_map<std::string, std::pair<int, int>>;
#endif
  std::string getName(std::string path);

public:
  RAPLDeviceHandle devices;
  std::unordered_map<std::string, std::string> max_energy_devices;
  std::map<std::string, unsigned long long> getEnergy();
  RAPLDevice();
};
#endif