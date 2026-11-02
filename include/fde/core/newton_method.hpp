#pragma once
#include <fde/core/types.hpp>
#include <fde/core/solution.hpp>
#include <fde/core/system.hpp>

#include <stdexcept>


namespace fde{

    void solve_linear(
        const SquareMatrix& A,
        const State& b,
        State& x
    );

    State newton_method(
        const System& system,
        double t,
        const State& c,
        double a,
        const State& y_nm1,
        const double atol = 1e-10,
        const double rtol = 1e-8,
        const int max_iter = 20
    );
} // namespace fde
