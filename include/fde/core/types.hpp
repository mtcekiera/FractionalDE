#pragma once
#include <complex>
#include <limits>
#include <cmath>
#include <vector>
#include <functional>

namespace fde{
    constexpr double pi = M_PI;

    using Complex = std::complex<double>;

    /*
        both Sequence and State are just std::vectors of doubles
        they are used in order to distinguish the (N+1)-dimensional 
        vector (sequence) of time evolution from the (dim)-dimensional 
        vector (state) of the system at n-th step
    */
    // Sequence as in the entire system evolution
    using Sequence = std::vector<double>;

    // State as in current system state at n-th step
    using State = std::vector<double>;

    using RHS = std::function<
        void(double t, const State& y, State& result) 
        // result saved to State& result variable
    >;

    // system of equations
    struct System{
        double alpha;
        State y0;
        RHS func;
        double T;
        std::size_t N;

        Sequence uniform_timegrid() const;
        double dt() const;
    };

    Sequence uniform_timegrid(
        double t0,
        double T,
        std::size_t N
    );
}