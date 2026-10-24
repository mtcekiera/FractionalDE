#pragma once
#include <fde/core/types.hpp>
#include <fde/core/solution.hpp>

namespace fde{
    // abstract class for inheritence
    class Solver{
    public:
        virtual ~Solver() = default;
        virtual void initialize(
            const System& system
        ) = 0;
        virtual State step(
            std::size_t n,
            const System& system,
            const Solution& solution 
            // solution serves as history
        ) = 0;
    };
}
