#pragma once
#include <fde/core/types.hpp>
#include <fde/core/solution.hpp>
#include <fde/solvers/solver.hpp>

namespace fde{

    class Driver{
    public:
        Solution solve(
            const System& system,
            Solver& solver
        ) const;
    };

}
