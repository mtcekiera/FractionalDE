#pragma once
#include <fde/solvers/solver.hpp>
#include <fde/core/driver.hpp>
#include <fde/core/types.hpp>

void test_solver_mittag_leffler(
    std::string filename, 
    fde::Solver& solver,
    double alpha,
    double lambda = 1.0,
    double T = 1.0,
    std::size_t N = 1000
){
    
    fde::State y0 = {1.0};


    fde::RHS func = [lambda](
        double t, 
        const fde::State& y,
        fde::State& result
    ){
        result = {-lambda*y[0]};
    };

    fde::Jacobian jac = [lambda](
        double t, const fde::State& y, fde::SquareMatrix& J
    ){
        J(0,0) = -lambda;
    };

    fde::System system(func, jac, y0, alpha, T, N);

    solver.initialize(system);

    fde::Driver driver;

    fde::Solution solution = driver.solve(system, solver);

    solution.write_csv(filename);
}
