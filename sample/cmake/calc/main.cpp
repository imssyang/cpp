#include <cmath>
#include <iostream>
#include <string>
#include "config.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0]
                  << " version " << CALC_VERSION_MAJOR
                  << "." << CALC_VERSION_MINOR
                  << " number" << std::endl;
        return 0;
    }

    const double inputValue = std::stod(argv[1]);
    const double outputValue = sqrt(inputValue);
    std::cout << "The square root of " << inputValue
              << " is " << outputValue
              << std::endl;
    return 0;
}
