#include <fde/core/newton_method.hpp>
#include <Eigen/Dense>
#include <iostream>
namespace fde{

    void solve_linear(
        const SquareMatrix& A,
        const State& b,
        State& x
    ){
        std::size_t dim = b.size();

        if (dim != A.dim())
            throw std::runtime_error("solve_linear: b and A dimensions do not match");

        x.resize(dim);
        // dynamic sizes, stored row next to row in memory
        using EigenMatrix = Eigen::Matrix<
            double,
            Eigen::Dynamic,
            Eigen::Dynamic,
            Eigen::RowMajor
        >;

        // mapping to avoid copying
        Eigen::Map<const EigenMatrix> A_matrix(
            A.data(), dim, dim
        );
        Eigen::Map<const Eigen::VectorXd> b_vector(b.data(), dim);

        // full pivot for now in case of degenerate matrices
        Eigen::FullPivLU<Eigen::MatrixXd> LU(A_matrix);
        if(!LU.isInvertible())
            throw std::runtime_error("solve_linear: matrix is not invertible");
        
        Eigen::VectorXd solution = LU.solve(b_vector);

        for (std::size_t i=0; i<dim; i++)
            x[i] = solution[i];
    }




    State newton_method(
        const System& system,
        double t,
        const State& c,
        double a,
        const State& yn,
        const double atol,
        const double rtol,
        const int max_iter
    ){

        const std::size_t dim = yn.size();

        if (dim != c.size())
            throw std::invalid_argument("newton_method: yn and c sizes do not match");

        SquareMatrix J_f(dim);
        system.jac(t, yn, J_f);

        SquareMatrix identity = SquareMatrix::identity(dim);
        SquareMatrix J_R = identity - a*J_f;

        // initial guess
        State y = yn;

        State f(dim);
        State R(dim);
        State b(dim);

        for (std::size_t k=0; k<=max_iter; k++){
            system.func(t, y, f);

            R = y - c - a*f;

            double norm_R = norm_inf(R);
            double norm_y = norm_inf(y);
            
            if (!std::isfinite(norm_R))
                throw std::runtime_error("newton_method: non-finite norm_R");

            if (norm_R <= atol + rtol * norm_y)
                return y;

            if (k == max_iter)
                break;

            system.jac(t, y, J_f);
            J_R = identity - a*J_f;

            b = (-1)*R;


            State delta_y;
            solve_linear(J_R, b, delta_y);

            double norm_delta_y = norm_inf(delta_y);
            if (!std::isfinite(norm_delta_y))
                throw std::runtime_error("newton_method: non-infinite delta_y");

            y += delta_y;
        }

        throw std::runtime_error("newton_method: did not converge");

    }
}