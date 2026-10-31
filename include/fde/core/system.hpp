#pragma once
#include <fde/core/types.hpp>

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace fde {

    class System {
    private:
        RHS func_;
        Jacobian jac_;
        State y0_;
        double alpha_;
        double T_;
        std::size_t N_;

    public:
        System(
            RHS func,
            Jacobian jac,
            State y0,
            double alpha,
            double T,
            std::size_t N
        );

        // Constructor without Jacobian
        System(
            RHS func,
            State y0,
            double alpha,
            double T,
            std::size_t N
        );

        // Function evaluation
        void func(double t, const State& y, State& dydt) const;

        // Jacobian evaluation
        void jac(double t, const State& y, SquareMatrix& J) const;

        bool has_jacobian() const;

        // Getters
        const State& y0() const;
        double alpha() const;
        double T() const;
        std::size_t N() const;
        
        std::size_t dim() const;

        double dt() const;
    };

}

