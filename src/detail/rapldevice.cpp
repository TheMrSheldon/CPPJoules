#include "rapldevice.hpp"

#include <fstream>
#include <iostream>
#include <chrono>
#include <memory>
#include <unordered_map>

#ifdef _WIN64

#define MSR_FUNC_FREQ 0
#define MSR_FUNC_POWER 1
#define MSR_FUNC_TEMP 2
#define MSR_FUNC_LIMIT 3

#define GETFUNC(lib, fn) GetProcAddress((lib), (fn))
typedef void (*powergadgetfunction_void)();
typedef bool (*powergadgetfunction_bool)();
typedef void (*powergadgetfunction_intp)(int *);
typedef void (*powergadgetfunction_int_intp)(int, int *);
typedef void (*powergadgetfunction_int_wchartp)(int, wchar_t *);
typedef void (*powergadgetfunction_int_int_doublep_intp)(int, int, double *, int *);
#endif

using cppjoules::detail::RAPLDevice;

#ifdef __linux__
const std::filesystem::path RAPLDevice::RaplBasePath{"/sys/class/powercap/intel-rapl/"};
#endif


RAPLDevice::RAPLDevice()
{
#ifdef __linux__

  for (uint32_t i = 0; true; i++)
  {
    std::string temp = "intel-rapl:" + std::to_string(i);
    auto basepath = RaplBasePath / temp;
    if (!std::filesystem::exists(basepath))
      break;
    
    int inner_id = 0;
    auto path = basepath / (temp + ":" + std::to_string(inner_id));

    std::unordered_map<std::string, std::string> device, max_energy_device;

    devices[getName(RaplBasePath / temp)] = RaplBasePath / temp / "energy_uj";
    max_energy_devices[getName(RaplBasePath / temp)] = RaplBasePath / temp / "max_energy_range_uj";

    while (std::filesystem::exists(path))
    {
      devices[getName(path) + "-" + std::to_string(i)] = path / "energy_uj";
      max_energy_device[getName(path) + "-" + std::to_string(i)] = path / "max_energy_range_uj";
      inner_id++;
      path = RaplBasePath / temp / (temp + ":" + std::to_string(inner_id));
    }
  }
#endif
/**
 * In windows we need to use the energy dll to get
 * the required functions
 */
#ifdef _WIN64
  initialized = false;
  char tpath[MAX_PATH];
  GetSystemDirectoryA(tpath, MAX_PATH);
  // Directly building might not work, the string might be lost before applying .c_str
  std::string fullpath = std::string(tpath) + std::string("\\EnergyLib64.dll");
  const char *path = fullpath.c_str();
  handler = LoadLibrary(path);
  if (!handler)
  {
    std::cout << "Power gadget could not be loaded" << std::endl;
    return;
  }
  powergadgetfunction_void _gadgetInit = reinterpret_cast<powergadgetfunction_void>(GETFUNC(handler, "IntelEnergyLibInitialize"));
  // powergadgetfunction_void _ReadSample = reinterpret_cast<powergadgetfunction_void>(GETFUNC(handler, "ReadSample"));
  _gadgetInit();

  powergadgetfunction_intp _GetNumMsrs = reinterpret_cast<powergadgetfunction_intp>(GETFUNC(handler, "GetNumMsrs"));
  powergadgetfunction_intp _GetNumNodes = reinterpret_cast<powergadgetfunction_intp>(GETFUNC(handler, "GetNumNodes"));
  int numMsrs = 0;
  _GetNumMsrs(&numMsrs);

  int numNodes = 0;
  _GetNumNodes(&numNodes);

  powergadgetfunction_int_intp _GetMsrFunc = reinterpret_cast<powergadgetfunction_int_intp>(GETFUNC(handler, "GetMsrFunc"));
  powergadgetfunction_int_wchartp _GetMsrName = reinterpret_cast<powergadgetfunction_int_wchartp>(GETFUNC(handler, "GetMsrName"));
  powergadgetfunction_bool _ReadSample = reinterpret_cast<powergadgetfunction_bool>(GETFUNC(handler, "ReadSample"));

  //_ReadSample();
  for (int i = 0; i < numNodes; i++)
  {
    for (int j = 0; j < numMsrs; j++)
    {
      int func;
      _GetMsrFunc(j, &func);
      if (func == MSR_FUNC_POWER)
      {
        std::unique_ptr<wchar_t[]> name = std::make_unique<wchar_t[]>(100);
        _GetMsrName(j, name.get());
        std::wstring namewstr = std::wstring(name.get());
        std::string namestr = std::string(namewstr.begin(), namewstr.end());
        namestr = namestr + "-" + std::to_string(i);
        devices[namestr] = {i, j};
      }
    }
  }

#endif
}

std::string RAPLDevice::getName(const std::filesystem::path &path) noexcept
{
  std::ifstream file_handler(path / "name");
  if (std::string name; getline(file_handler, name))
    return name;
  /** \todo handle error **/
  abort();
}

std::map<std::string, unsigned long long> RAPLDevice::getEnergy()
{
  std::map<std::string, unsigned long long> energies;
#ifdef __linux__
  for (const auto &[first, path] : devices)
  {
    std::ifstream file(path);

    if (!file)
    {
      std::cout << "Give read access to " << path << std::endl;
      return energies;
    }

    if (std::string line; getline(file, line))
    {
      energies[first] = std::stoll(line);
    }
    else
    {
      /** \todo handle error **/
    }
  }
#elif _WIN64
  if (!handler)
  {
    return energies;
  }
  powergadgetfunction_int_int_doublep_intp _GetPowerData = reinterpret_cast<powergadgetfunction_int_int_doublep_intp>(GETFUNC(handler, "GetPowerData"));
  powergadgetfunction_bool _ReadSample = reinterpret_cast<powergadgetfunction_bool>(GETFUNC(handler, "ReadSample"));
  _ReadSample();

  for (const auto &[first, second] : devices)
  {
    std::unique_ptr<double[]> data = std::make_unique<double[]>(10);
    int nData;
    if (initialized)
    {
      _GetPowerData(second.first, second.second, data.get(), &nData);
      energies[first] = static_cast<long long>(data.get()[1]);
    }
    else
    {
      energies[first] = 0;
    }
  }
  initialized = true;
#endif

  return energies;
}