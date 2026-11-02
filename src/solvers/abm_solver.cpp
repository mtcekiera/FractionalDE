#include <fde/solvers/abm_solver.hpp>
#include <fde/core/newton_method.hpp>
#include <cmath>

namespace fde{
    void ABMSolver::initialize(const System& system){
        
        const double alpha = system.alpha();
        const double m_gamma_alpha_plus2 = 1.0 / std::tgamma(alpha + 2.0);
        
        p_weights.resize(system.N()+1);
        A_weights.resize(system.N()+1);
        Abar_weights.resize(system.N()+1);
    

        p_weights[0] = 0.0;
        A_weights[0] = m_gamma_alpha_plus2;
        Abar_weights[0] = A_weights[0];

        for (std::size_t k = 1; k<=system.N(); k++){
            const double k_d = static_cast<double> (k);

            p_weights[k] = 
                std::pow(k_d, alpha) - 
                std::pow(k_d - 1.0, alpha);

            A_weights[k] = 
                m_gamma_alpha_plus2 * (
                std::pow(k_d - 1.0, alpha + 1.0) -
                2.0 * std::pow(k_d, alpha + 1.0) +
                std::pow(k_d + 1.0, alpha + 1.0)
                );
            
            Abar_weights[k] = 
                m_gamma_alpha_plus2 * (
                    std::pow(k_d - 1.0, alpha + 1.0) - 
                    std::pow(k_d, alpha) * (k_d - alpha - 1)
                );
        }
    }

    
    State ABMSolver::step(
        const System& system,
        const Solution& solution
    ){
        std::size_t n = solution.last_n() + 1;
        const double h = system.dt();
        const double t = h*n;
        const std::size_t dim = solution.dimension();
        const double alpha = system.alpha();
        const double h_pow_alpha = std::pow(h, alpha);
        State y_n_predict(dim);
        
        for (std::size_t d=0; d<dim; d++){
            const Sequence& y = solution.get_y(d);
            const Sequence& f = solution.get_f(d);

            double predictor_sum = 0.0;
            for (std::size_t j=0; j<n; j++)
                predictor_sum += p_weights[n-j] * f[j];
            const double y_n_predict_d = 
                y[0] + h_pow_alpha / std::tgamma(alpha+1) * predictor_sum;
            
            y_n_predict[d] = y_n_predict_d;

        }

        State f_n_predict(dim);
        system.func(t, y_n_predict, f_n_predict);

        State y_n(dim);

        for (std::size_t d=0; d<dim; d++){
            const Sequence& y = solution.get_y(d);
            const Sequence& f = solution.get_f(d);
            double corrector_sum = 0.0;
            for (std::size_t j=1; j<n; j++)
                corrector_sum += A_weights[n-j] * f[j];
            const double corrector = 
                Abar_weights[n] * f[0] +
                corrector_sum +
                A_weights[0] * f_n_predict[d];

            y_n[d] = y[0] + h_pow_alpha * corrector;
        }

        return y_n;

    }

}