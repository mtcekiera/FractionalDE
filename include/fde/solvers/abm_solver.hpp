#pragma once

#include <fde/solvers/solver.hpp>

namespace fde{

    class ABMSolver : public Solver{
    private:
        std::vector<double> p_weights;
        std::vector<double> A_weights;
        std::vector<double> Abar_weights;
    public:
        void initialize(
            const System& system
        ) override;

        State step(
            const System& system,
            const Solution& solution
        ) override;
    };


}