#include <fde/solvers/gl_solver.hpp>
#include <fde/core/types.hpp>
#include <fde/core/newton_method.hpp>
#include <cmath>
#include <iostream>

namespace fde{

    void GLSolver::initialize(
        const System& system
    ){
        weights.resize(system.N+1);

        weights[0] = 1.0;
        for (std::size_t i = 1; i<=system.N; i++){
            weights[i] = 
            (
                1.0 - (1.0 + system.alpha) / i
            ) * weights[i-1];
        }
    }

    State GLSolver::step(
        std::size_t n,
        const System& system,
        const Solution& solution
    ){
        const double h = system.dt();
        const double t = h*n;
        // important to first input t=0, y0, f(y0) into solution
        const std::size_t dim = solution.dimension();
        const double alpha = system.alpha;
        const double a = std::pow(h, alpha);
        State c(dim);
        State yn1(dim);
        for (std::size_t d=0; d<dim; d++){

            const State& y = solution.y[d];
            yn1[d] = y.back();
            const double y0 = y[0];
            double cn = y0;
            // for j=n: y_0 - y_0 = 0, no need to go through it
            for (std::size_t j=1; j<n; j++)
                cn -= weights[j] * (y[n-j] - y0); // minus!
            c[d] = cn;
        }

        return newton_method(
            system, t, c, a, yn1
        );

    }

}
