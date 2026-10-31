#include <fde/core/square_matrix.hpp>
#include <stdexcept>

namespace fde{
    // constructors
    SquareMatrix::SquareMatrix(
        std::initializer_list<std::initializer_list<double>> values
    ) : N(values.size()), values(N * N) {

        std::size_t i = 0;

        for (const auto& row : values) {
            if (row.size() != N)
                throw std::invalid_argument("Matrix must be square");

            std::size_t j = 0;

            for (double value : row) {
                (*this)(i, j) = value;
                ++j;
            }

            ++i;
        }
    }

    // static methods
    SquareMatrix SquareMatrix::identity(std::size_t N){
        SquareMatrix id(N);
        for (std::size_t i = 0; i<N; i++)
            id(i,i) = 1.0;
        return id;        
    }

    // dimensions
    const std::size_t SquareMatrix::dim() const{
        return N;
    }

    // element access
    double& SquareMatrix::operator()(std::size_t i, std::size_t j){
        return values[i*N + j];
    }
    const double SquareMatrix::operator()(std::size_t i, std::size_t j) const{
        return values[i*N + j];
    }


    double* SquareMatrix::data(){
        return values.data();
    }
    const double* SquareMatrix::data() const{
        return values.data();
    }



    // addition
        SquareMatrix& SquareMatrix::operator+=(const SquareMatrix& other){
        if (N != other.N)
            throw std::invalid_argument(
                "SquareMatrix: matrices must be "
                "of the same dimensions for addition!");
        for (std::size_t i=0; i<N*N; i++)
            values[i] += other.values[i];
        return *this;
    }
    SquareMatrix operator+(SquareMatrix A, SquareMatrix B){
        A += B;
        return A;
    }

    // subtraction
    SquareMatrix& SquareMatrix::operator-=(const SquareMatrix& other){
        if (N != other.N)
            throw std::invalid_argument(
                "SquareMatrix: matrices must be "
                "of the same dimensions for addition!");
        for (std::size_t i=0; i<N*N; i++)
            values[i] -= other.values[i];
        return *this;
    }
    SquareMatrix operator-(SquareMatrix A, SquareMatrix B){
        A -= B;
        return A;
    }



    // matrix multiplication
    SquareMatrix operator*(SquareMatrix A, SquareMatrix B){
        const std::size_t dim = A.dim();
        if (dim != B.dim())
            throw std::invalid_argument(
                "SquareMatrix: matrices must be "
                "of the same dimensions for multiplication!");
        
        SquareMatrix output(dim);
        for (std::size_t i=0; i<dim; i++){
            for (std::size_t j=0; j<dim; j++){
                double sum = 0.0;
                for (std::size_t k=0; k<dim; k++){
                    sum += A(i, k) * B(k, j);
                }
                output(i,j) = sum;
            }
        }
        return output;
    }



    // scalar multiplication
    SquareMatrix& SquareMatrix::operator*=(double scalar){
        for (auto& value : values)
            value *= scalar;
        return *this;
    }    
    SquareMatrix operator*(double scalar, SquareMatrix matrix){
        matrix *= scalar;
        return matrix;
    }
    SquareMatrix operator*(SquareMatrix matrix, double scalar){
        matrix *= scalar;
        return matrix;
    }
}