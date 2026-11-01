#include <fde/solvers/gl_solver.hpp>
#include <fde/core/types.hpp>
#include <fde/core/newton_method.hpp>
#include <cmath>

namespace fde{

    void GLSolver::initialize(
        const System& system
    ){
        weights.resize(system.N()+1);

        weights[0] = 1.0;
        for (std::size_t i = 1; i<=system.N(); i++){
            weights[i] = 
            (
                1.0 - (1.0 + system.alpha()) / i
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
        const double alpha = system.alpha();
        const double a = std::pow(h, alpha);
        State c_n(dim);
        State y_nm1(dim);
        for (std::size_t d=0; d<dim; d++){

            const Sequence& y_d = solution.y[d];
            y_nm1[d] = y_d.back();
            const double y_d0 = y_d[0];
            double c_nd = y_d0;
            // for j=n: y_0 - y_0 = 0, no need to go through it
            for (std::size_t j=1; j<n; j++)
                c_nd -= weights[j] * (y_d[n-j] - y_d0);
            c_n[d] = c_nd;
        }

        return newton_method(
            system, t, c_n, a, y_nm1
        );

    }

}
