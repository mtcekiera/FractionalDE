#pragma once
#include <fde/solvers/solver.hpp>
#include <fde/core/driver.hpp>
#include <fde/core/types.hpp>
#include <filesystem>


void P12_single(
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


void P12(){
    std::filesystem::path dir = std::filesystem::current_path();
    std::filesystem::path data_dir = dir / "data/P12";

    std::filesystem::create_directory(data_dir);

    fde::GLSolver gl;
    fde::L1Solver l1;
    fde::ABMSolver abm;
    fde::PTSolver pt;

    P12_single(
        data_dir/"gl_test_a05.csv", gl, 0.5
    );
    P12_single(
        data_dir/"gl_test_a1.csv", gl, 1.0
    );
    P12_single(
        data_dir/"l1_test_a05.csv", l1, 0.5
    );
    P12_single(
        data_dir/"l1_test_a1.csv", l1, 1.0
    );
    P12_single(
        data_dir/"abm_test_a05.csv", abm, 0.5
    );
    P12_single(
        data_dir/"abm_test_a1.csv", abm, 1.0
    );
    P12_single(
        data_dir/"pt_test_a05.csv", pt, 0.5
    );
    P12_single(
        data_dir/"pt_test_a1.csv", pt, 1.0
    );
}
