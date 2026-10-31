#include <fde/core/system.hpp>
#include <stdexcept>

namespace fde{

    System::System(
            RHS func,
            Jacobian jac,
            State y0,
            double alpha,
            double T,
            std::size_t N
        ) : func_(std::move(func)),
            jac_(std::move(jac)),
            y0_(std::move(y0)),
            alpha_(alpha),
            T_(T),
            N_(N)
        {
            if (!func_)
                throw std::invalid_argument("RHS function is required");

            if (y0_.empty())
                throw std::invalid_argument("Initial state cannot be empty");

            if (!std::isfinite(alpha_) || alpha_ <= 0.0)
                throw std::invalid_argument("Invalid fractional order");

            if (!std::isfinite(T_) || T_ <= 0.0)
                throw std::invalid_argument("Invalid final time");

            if (N_ == 0)
                throw std::invalid_argument("N must be positive");
        }

    System::System(
            RHS func,
            State y0,
            double alpha,
            double T,
            std::size_t N
        ) : System(
            std::move(func), {}, std::move(y0), alpha, T, N
        ) {}

    void System::func(double t, const State& y, State& dydt) const {
        func_(t, y, dydt);
    }

    // Jacobian evaluation
    void System::jac(double t, const State& y, SquareMatrix& J) const {
        if (!jac_)
            throw std::runtime_error("Jacobian not provided");

        jac_(t, y, J);
    }

    bool System::has_jacobian() const {
        return static_cast<bool>(jac_);
    }

    // Getters
    const State& System::y0() const {   return y0_;     }
    double System::alpha() const {      return alpha_;  }
    double System::T() const {          return T_;      }
    std::size_t System::N() const {     return N_;      }
    std::size_t System::dim() const {   return y0_.size(); }

    double System::dt() const {
        return T_ / static_cast<double>(N_);
    }

}
    