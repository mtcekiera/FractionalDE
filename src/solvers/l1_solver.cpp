#include <fde/solvers/l1_solver.hpp>
#include <fde/core/newton_method.hpp>
#include <cmath>

namespace fde{
    void L1Solver::initialize(const System& system){
        weights.resize(system.N()+1);
        const double exponent = 1.0 - system.alpha();
        weights[0] = 1.0;
        for (std::size_t m = 1; m<=system.N(); m++){
            const double m_d = static_cast<double> (m);
            weights[m] = 
                std::pow(m_d + 1.0, exponent) 
              - std::pow(m_d,       exponent);
        }
    }

    
    State L1Solver::step(
        std::size_t n,
        const System& system,
        const Solution& solution
    ){
        const double h = system.dt();
        const double t = h*n;
        const std::size_t dim = solution.dimension();
        const double alpha = system.alpha();
        const double a = std::tgamma(2 - alpha) * std::pow(h, alpha);
        State c_n(dim);
        State y_nm1(dim);
        for (std::size_t d=0; d<dim; d++){
            const Sequence& y = solution.y[d];
            const double y_nm1_d = y.back();
            y_nm1[d] = y_nm1_d;
            double c_nd = y_nm1_d;

            for (std::size_t m=1; m<n; m++){
                c_nd -= weights[m] * (y[n-m] - y[n-m-1]);
            }
            c_n[d] = c_nd;
        }

        return newton_method(
            system, t, c_n, a, y_nm1
        );
    }

}