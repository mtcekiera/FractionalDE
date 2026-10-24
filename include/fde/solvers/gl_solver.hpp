#pragma once
#include <fde/solvers/solver.hpp>

namespace fde{


    class GLSolver : public Solver{
    private:
        std::vector<double> weights;
    public:
        void initialize(
            const System& system
        ) override;

        State step(
            std::size_t n,
            const System& system,
            const Solution& solution
        ) override;
    };


}