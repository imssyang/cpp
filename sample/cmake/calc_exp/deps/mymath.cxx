#include <cmath>
#include "mymath.h"

#ifdef USE_MYMATH
#  include "mysqrt.h"
#endif

namespace mathfunctions {
    double sqrt(double x) {
#ifdef USE_MYMATH
        return detail::mysqrt(x);
#else
        return std::sqrt(x);
#endif
    }
}
