#pragma once
#include <fde/core/types.hpp>
#include <fde/core/solution.hpp>
#include <stdexcept>


namespace fde{
    State newton_method(
        const System& problem,
        double t,
        const State& c,
        double a,
        const State& initial_guess
    ){
        // TODO
        if (
            (c.size() != 1) or (initial_guess.size() != 1)
        )
            throw std::invalid_argument("newton_method: dim>1 not implemented yet");
        return {c[0] / (1+a)};
    }
} // namespace fde
