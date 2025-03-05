#ifndef NVIDIA_DEVICES
#define NVIDIA_DEVICES

#ifdef __linux__
#include <dlfcn.h>
#define OPENLIB(libname) dlopen(libname, RTLD_LAZY)
#define PATH "libnvidia-ml.so.1"
#define GETFUNC(lib, func) dlsym((lib), (func))
using GPUHandle = void *;
#define CLOSELIB(handle) dlclose(handle)
#endif

#ifdef _WIN64
#include "windows.h"
#define CLOSELIB(handle)
using GPUHandle = HINSTANCE;
#define OPENLIB(libname) LoadLibrary((LPCSTR)libname)
#define GETFUNC(lib, fn) GetProcAddress((lib), (fn))
#endif

#ifdef __APPLE__
#include <dlfcn.h>
#define OPENLIB(libname) dlopen(libname, RTLD_LAZY)
#define PATH "../dependencies/IntelPowerGadget.framework/IntelPowerGadget"
#define GETFUNC(lib, func) dlsym((lib), (func))
using GPUHandle = void *;
#define CLOSELIB(handle) dlclose(handle)
#endif

#include <nvml/nvml.h>

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace cppjoules::detail
{
  class NVMLDevice final
  {
  private:
    std::vector<std::unique_ptr<nvmlDevice_t>> devices;
    std::unique_ptr<uint32_t> device_count;

    GPUHandle nvmlhandle;

  public:
    NVMLDevice();
    bool usable;
    std::map<std::string, unsigned long long> getEnergy();
  };
}
#endif