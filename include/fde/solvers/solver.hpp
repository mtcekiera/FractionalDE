#pragma once
#include <fde/core/types.hpp>
#include <fde/core/solution.hpp>
#include <fde/core/system.hpp>

namespace fde{
    // abstract class for inheritence
    class Solver{
    public:
        virtual ~Solver() = default;
        virtual void initialize(
            const System& system
        ) = 0;
        virtual State step(
            const System& system,
            const Solution& solution 
            // solution serves as history
        ) = 0;
    };
}
