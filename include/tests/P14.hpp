#pragma once
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#include <fde/fde.hpp>
#include <chrono>



double P14_single_time(
    fde::Solver& solver,
    std::size_t N,
    double lambda = 1.0,
    double alpha = 0.5,
    double T = 1.0
    
){

    fde::State y0 = {1.0};

    fde::RHS func = [lambda](
        double t,
        const fde::State& y,
        fde::State& result
    ){
        result = {-lambda * y[0]};
    };

    fde::Jacobian jac = [lambda](
        double t,
        const fde::State& y,
        fde::SquareMatrix& J
    ){
        J(0, 0) = -lambda;
    };

    fde::System system(func, jac, y0, alpha, T, N);

    solver.initialize(system);

    fde::Driver driver;


    auto start = std::chrono::steady_clock::now();

    fde::Solution solution = driver.solve(system, solver);

    auto stop = std::chrono::steady_clock::now();


    return 
        std::chrono::duration<double>(stop - start).count();
}

void P14(){
    namespace fs = std::filesystem;
    fs::path dir = std::filesystem::current_path();
    fs::path data_dir = dir / "data/P14";
    fs::create_directory(data_dir);


    fde::GLSolver gl;
    fde::L1Solver l1;
    fde::ABMSolver abm;
    fde::PTSolver pt;


    struct Method {
        std::string name;
        fde::Solver& solver;
    };

    std::vector<Method> methods = {
        {"gl", gl},
        {"l1", l1},
        {"abm", abm},
        {"pt", pt}
    };

    for (auto method : methods){
        std::ofstream file (
            data_dir / (method.name + ".csv")
        );
        file<<"N,time\n";
        for (std::size_t k=4; k<=10; k++){
            const std::size_t N = std::size_t{1} << k;
            const double time = 
                P14_single_time(method.solver, N);
            
            file << N << "," << time << "\n";
        }
    }

}