#pragma once
#include <fde/solvers/gl_solver.hpp>
#include <fde/core/driver.hpp>
#include <fde/core/types.hpp>

void test_gl(std::string filename, double alpha){
    double lambda = 2.0;
    alpha = alpha;
    
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
    double T = 1.0;
    std::size_t N = 1000;

    fde::System system(func, jac, y0, alpha, T, N);


    fde::GLSolver gl;
    gl.initialize(system);
    fde::Driver driver;

    fde::Solution solution = driver.solve(system, gl);
    solution.write_csv(filename);
}
