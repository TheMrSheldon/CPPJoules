#include <cppjoules/cppjoules.hpp>
#include <cppjoules/ioutils.hpp>

int fib(int n) {
    if (n <= 1)
        return n;
    return fib(n-1) + fib(n-2);
}

int main(int argc, char* argv[]) {
    cppjoules::EnergyTracker tracker;
    tracker.start();
    fib(20);
    tracker.stop();
    cppjoules::io::dumpToConsole(tracker);
    return 0;
}
