#pragma once
#include <vector>
#include <initializer_list>

namespace fde{
    class SquareMatrix{
    private:
        std::size_t N;
        // data kept in 1d vector, each row after one before
        std::vector<double> values;
    public:

        // constructors
        SquareMatrix(
            std::initializer_list<std::initializer_list<double>> values
        );
        SquareMatrix(std::size_t N_) : N(N_), values(N_*N_, 0.0){};

        // static methods
        static SquareMatrix identity(std::size_t N);

        // dimensions
        const std::size_t dim() const;

        // element access
        double& operator()(std::size_t i, std::size_t j);
        const double operator()(std::size_t i, std::size_t j) const;

        double* data();
        const double* data() const;

        // operators
        SquareMatrix& operator*=(double other);
        SquareMatrix& operator+=(const SquareMatrix& other);
        SquareMatrix& operator-=(const SquareMatrix& other);
    };

    
    SquareMatrix operator+(SquareMatrix A, SquareMatrix B);
    SquareMatrix operator-(SquareMatrix A, SquareMatrix B);
    SquareMatrix operator*(SquareMatrix A, SquareMatrix B);
    SquareMatrix operator*(double scalar, SquareMatrix matrix);
    SquareMatrix operator*(SquareMatrix matrix, double scalar);
}
