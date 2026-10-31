#include <fde/core/types.hpp>


namespace fde{

    State& operator+=(State& a, const State& b) {
        if (a.size() != b.size())
            throw std::invalid_argument("State dimensions mismatch");

        for (std::size_t i = 0; i < a.size(); ++i)
            a[i] += b[i];

        return a;
    }

    State operator+(State a, const State& b) {
        a += b;
        return a;
    }

    State& operator-=(State& a, const State& b) {
        if (a.size() != b.size())
            throw std::invalid_argument("State dimensions mismatch");

        for (std::size_t i = 0; i < a.size(); ++i)
            a[i] -= b[i];

        return a;
    }

    State operator-(State a, const State& b) {
        a -= b;
        return a;
    }

    State operator*(State a, double scalar) {
        for (double& x : a)
            x *= scalar;

        return a;
    }

    State operator*(double scalar, State a) {
        for (double& x : a)
            x *= scalar;

        return a;
    }

    double norm_inf(const State& st){
        double result = 0.0;
        for (auto val : st)
            result = std::max(result, std::abs(val));

        return result;
    }
}
