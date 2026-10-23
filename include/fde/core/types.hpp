#pragma once
#include <complex>
#include <limits>
#include <cmath>
#include <vector>
#include <functional>

namespace fde{
    constexpr double pi = M_PI;

    using Complex = std::complex<double>;

    using Sequence = std::vector<double>;

    using RHS = std::function<
        void(double t, const Sequence& y, Sequence& result)
    >;

    struct Problem{
        double alpha;
        std::vector<double> y0;
        RHS func;
    };

    Sequence uniform_timegrid(
        double t0,
        double T,
        std::size_t N
    );
}