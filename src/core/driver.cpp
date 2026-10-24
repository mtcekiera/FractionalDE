#include <fde/core/driver.hpp>

namespace fde{
    Solution Driver::solve(
        const System& system,
        Solver& solver
    ) const {
        const std::size_t dim = system.y0.size();
        // const std::size_t N = system.N;
        Solution solution(dim, system.N);

        const double dt = system.T / static_cast<double>(system.N);

        State yn = system.y0;
        State fn(dim);
        system.func(0.0, system.y0, fn);

        // inputing the first step
        solution.push_back(0.0, yn, fn);

        for (std::size_t n = 1; n<=system.N; n++){
            const double t = dt * n;
            yn = solver.step(n, system, solution);
            system.func(t, yn, fn);

            solution.push_back(t, yn, fn);
        }
        return solution;
    }
}