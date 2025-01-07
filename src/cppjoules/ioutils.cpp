#include <cppjoules/ioutils.hpp>

#include <cppJoules.hpp>

#include <iostream>
#include <fstream>

void cppjoules::io::dumpToConsole(const EnergyTracker &tracker) noexcept
{
    auto [time, energy] = tracker.calculate_energy();
    if (energy.empty())
        std::cout << "No Value to print" << std::endl;
    std::cout << "Time " << time.count() << " ms \n";
    for (auto energy : energy)
        std::cout << energy.first << " " << energy.second << "\n";
}

void cppjoules::io::dumpToCSV(const EnergyTracker &tracker, const std::filesystem::path &file) noexcept
{
    auto [time, energies] = tracker.calculate_energy();
    std::ofstream csv_file(file);
    csv_file << "Time" << ",";
    for (auto &[name, _] : energies)
        csv_file << name << ",";
    csv_file << "\n";
    csv_file << time.count() << ",";
    for (auto &[_, energy] : energies)
        csv_file << energy << ",";
    csv_file << "\n";
}