#pragma once
#include <fde/core/types.hpp>

namespace fde{
    class Solution{
    private:
        Sequence t;
        // y values at t
        std::vector<Sequence> y;
        // f ([d/dt]^\alpha(y)) values at t
        std::vector<Sequence> f;

    public:
        Solution(std::size_t dim, std::size_t N);

        Sequence get_t() const;

        Sequence get_y(std::size_t d) const;
        Sequence get_f(std::size_t d) const;

        std::vector<Sequence> get_y() const;
        std::vector<Sequence> get_f() const;

        std::size_t size() const;
        std::size_t last_n() const;
        std::size_t dimension() const;

        void push_back(
            double time, 
            const State& yn,
            const State& fn
        );

        
        void write_csv(
            std::string filename, 
            int precision 
                = std::numeric_limits<double>::max_digits10 // 17 digits
        ) const;
    };

}