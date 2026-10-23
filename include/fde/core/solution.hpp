#pragma once
#include <fde/core/types.hpp>

namespace fde{
    struct Solution{
        Sequence t;
        // y values at t
        std::vector<Sequence> y;
        // f ([d/dt]^\alpha(y)) values at t
        std::vector<Sequence> f;

        Solution(std::size_t dim, std::size_t N);

        std::size_t size() const;
        std::size_t dimension() const;


        /*
        vector used explicitely (instead of Sequence) 
        to avoid confusion of whats being pushed
        */
         
        void push_back(
            double time, 
            const std::vector<double>& yn,
            const std::vector<double>& fn
        );

        
        void write_csv(
            std::string filename, 
            int precision 
                = std::numeric_limits<double>::max_digits10 // 17 digits
        ) const;
    };

}