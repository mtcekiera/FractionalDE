#pragma once
#include <complex>
#include <limits>
#include <cmath>
#include <vector>
#include <functional>

#include <fde/core/square_matrix.hpp>

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

    // additional operators
    State& operator+=(State& a, const State& b);
    State operator+(State a, const State& b);
    State& operator-=(State& a, const State& b);
    State operator-(State a, const State& b);

    State operator*(State a, double scalar);
    State operator*(double scalar, State a);

    // infinity norm
    double norm_inf(const State& st);

    /* 
        here RHS and jacobian do not return anything, thus error
        management must be handled inside of them
    */
    using RHS = std::function<
        // result saved to State& result variable
        void(double t, const State& y, State& result) 
    >;
    
    /*
        jacobian used only for the newtons method, thus it
        does not require dfdt
    */
    using Jacobian = std::function<
        void(double t, const State& y, SquareMatrix& J)
    >;


}